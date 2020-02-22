/*
 * epaper.c
 *
 *  Created on: 10.02.2020
 *      Author: johann
 */

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>

#define DIN_PIN BIT7 // DIN P1.7 SPI MOSI
#define CLK_PIN BIT5 // CLK	| P1.5 SPI SCK
#define CS_PIN BIT0 // CS P2.0 SPI CS
#define DC_PIN BIT5 // DC P2.5 Digital Out
#define RST_PIN BIT4 // RST P2.4 Digital Out
#define BUSY_PIN BIT3 // BUSY P2.3 Digital In

#define CYCLES_PER_MS 2047
#define LED1 BIT0 //
#define SOMI_PIN BIT6 //

#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

static const uint8_t lut_full_update[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

#if 0
static const uint8_t lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

void delay(uint16_t timeout_ms)
{
	uint16_t i;
	uint16_t j;
	for(i = 0 ;i < timeout_ms; i++)
	{
		j = CYCLES_PER_MS;
		while(j--)
		{
			__nop();
		}
	}
}

typedef struct port_regs
{
	uint8_t volatile PxIN;
	uint8_t volatile PxOUT;
	uint8_t volatile PxDIR;
	uint8_t volatile PxIFG;
	uint8_t volatile PxIES;
	uint8_t volatile PxIE;
	uint8_t volatile PxSEL;
	uint8_t volatile PxREN;
	uint8_t volatile reserved[25];
	uint8_t volatile PxSEL2;
}port_regs;

port_regs *const PORT1 = (port_regs *) &P1IN;
port_regs *const PORT2 = (port_regs *) &P2IN;

void setOutputHigh(port_regs *port, uint8_t pin)
{
	port->PxOUT |= pin;
}

void setOutputLow(port_regs *port, uint8_t pin)
{
	port->PxOUT &= ~pin;
}

uint8_t getInputPin(port_regs *port, uint8_t pin)
{
	return (port->PxIN & pin);
}


void spiInit()
{
	UCB0CTL1 |= UCSWRST;
	// init usci registers
	// 8Bit, MSB first, Master, mode 0 (3wire), Phase 0 Polarity 0
	UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCMODE_0 + UCSYNC;

	UCB0CTL1 |= UCSSEL_2; // SMCLK
	UCB0BR0 |= 0x04;// clock source division factor
	UCB0BR1 = 0x00;
	UCA0MCTL = 0; // No modulation
	UCB0CTL1 &= ~UCSWRST;
	// enable interrupts
}

void portInit()
{
	// configure ports
	P1SEL |= DIN_PIN + SOMI_PIN + CLK_PIN;
	P1SEL2 |= DIN_PIN + SOMI_PIN + CLK_PIN;
	P1DIR |= LED1;

	P2DIR |= DC_PIN + RST_PIN + CS_PIN;
	setOutputHigh(PORT2, DC_PIN + RST_PIN + CS_PIN);
}

void spiSend(uint8_t val)
{
	setOutputLow(PORT2, CS_PIN);
	UCB0TXBUF = val;
	while(UCB0STAT & UCBUSY);
	setOutputHigh(PORT2, CS_PIN);
}

void sendCommand(uint8_t val)
{
	setOutputLow(PORT2, DC_PIN);
	spiSend(val);
}

void sendData(uint8_t val)
{
	setOutputHigh(PORT2, DC_PIN);
	spiSend(val);
}

void reset(void)
{
	delay(200);
	setOutputLow(PORT2, RST_PIN);
	delay(200);
	setOutputHigh(PORT2, RST_PIN);
	delay(200);
}

void setWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    sendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    sendData((Xstart >> 3) & 0xFF);
    sendData((Xend >> 3) & 0xFF);

    sendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    sendData(Ystart & 0xFF);
    sendData((Ystart >> 8) & 0xFF);
    sendData(Yend & 0xFF);
    sendData((Yend >> 8) & 0xFF);
}

void setCursor(uint16_t Xstart, uint16_t Ystart)
{
    sendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    sendData((Xstart >> 3) & 0xFF);

    sendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    sendData(Ystart & 0xFF);
    sendData((Ystart >> 8) & 0xFF);
}

void turnOnDisplay(void)
{
    sendCommand(0x22); // DISPLAY_UPDATE_CONTROL_2
    sendData(0xC4);
    sendCommand(0x20); // MASTER_ACTIVATION
    sendCommand(0xFF); // TERMINATE_FRAME_READ_WRITE

    while(getInputPin(PORT2, BUSY_PIN) == BUSY_PIN){
    	delay(100);
    }
}

void clear(void)
{
    uint16_t Width;
    uint16_t Height;
    Width = (DISPLAY_WIDTH % 8 == 0)? (DISPLAY_WIDTH / 8 ): (DISPLAY_WIDTH / 8 + 1);
    Height = DISPLAY_HEIGHT;

    setWindow(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

	setCursor(0, 0);
	sendCommand(0x24);
	for (uint16_t i = 0; i < Width * Height; i++) {
		sendData(0XFF);
	}

    turnOnDisplay();
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop WDT
	// Set frequency
	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	portInit();
	spiInit();
	// Power On
	// (Apply VCI)
	// Reset the EPD driver IC
	reset();

	// Driver Output Control
	sendCommand(0x01);
	sendData((DISPLAY_HEIGHT-1) & 0xFF);
	sendData(((DISPLAY_HEIGHT-1)>>8) & 0xFF);
	sendData(0x00);

	// Booster Soft Start Control
	sendCommand(0x0C);
	sendData(0xD7);
	sendData(0xD6);
	sendData(0x9D);

	// WRITE_VCOM_REGISTER
	sendCommand(0x2C);
	sendData(0xA8); // VCOM 7C

	sendCommand(0x3A); // SET_DUMMY_LINE_PERIOD
	sendData(0x1A); // 4 dummy lines per gate

	sendCommand(0x3B); // SET_GATE_TIME
	sendData(0x08); // 2us per line

	// Data entry mode setting
	sendCommand(0x11);
	sendData(0x03); // X increment; Y increment

	//set the look-up table register
	uint8_t const *lut = lut_full_update;
	sendCommand(0x32);
	for (uint16_t i = 0; i < 30; i++)
	{
		sendData(lut[i]);
	}
	// Define the display size and the RAM address

	// Turn on oscillator clock and DC/DC & regulator to generate the driving voltage

	// Get temperature from external temperature sensor, then, load a waveform from OTP or code to LUT.

	// Clear the EPD
	clear();
	clear(); // TODO is twice really necessary?

	// Load image data and display the image on EPD

	// Turn off oscillator clock and DC/DC & regulator

	//Power Down or Enter into deep sleep mode
	while(1)
	{
		PORT1->PxOUT ^= LED1;
		delay(250);
	}
	return 0;
}


