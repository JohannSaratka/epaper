/*
 * epaper.c
 *
 *  Created on: 10.02.2020
 *      Author: johann
 */

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>

#include "hal.h"
#include "epaper.h"

#define DC_PIN BIT5 // DC P2.5 Digital Out
#define RST_PIN BIT4 // RST P2.4 Digital Out
#define BUSY_PIN BIT3 // BUSY P2.3 Digital In

#define DISPLAY_WIDTH (200U)
#define DISPLAY_HEIGHT (200U)

#if (DISPLAY_WIDTH % 8U == 0)
#define DISPLAY_NUM_BYTES ((DISPLAY_WIDTH / 8U ) * DISPLAY_HEIGHT)
#else
#define DISPLAY_NUM_BYTES ((DISPLAY_WIDTH / 8U + 1U) * DISPLAY_HEIGHT)
#endif

static void sendCommand(uint8_t val);
static void sendData(uint8_t val);
static void epd_reset(void);
static void epd_setWindow(uint16_t Xstart, uint16_t Ystart,
		uint16_t Xend, uint16_t Yend);
static void epd_setCursor(uint16_t Xstart, uint16_t Ystart);
static void epd_turnOnDisplay(void);


static const uint8_t lut_full_update[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

static const uint8_t lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void epd_init(bool useFullUpdate)
{
	const uint8_t * lut;
	if(useFullUpdate)
	{
		lut = lut_full_update;
	}
	else
	{
		lut = lut_partial_update;
	}

	// init hardware
	gpio_setDirectionOutput(PORT2, DC_PIN + RST_PIN);
	gpio_setDirectionInput(PORT2, BUSY_PIN);
	// Reset the EPD driver IC
	epd_reset();

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
	sendCommand(0x32);
	for (uint16_t i = 0; i < 30; i++)
	{
		sendData(lut[i]);
	}
}

static void epd_reset(void)
{
	bcm_delay(200);
	gpio_setOutputLow(PORT2, RST_PIN);
	bcm_delay(200);
	gpio_setOutputHigh(PORT2, RST_PIN);
	bcm_delay(200);
}

void epd_exit(void)
{
	gpio_setOutputLow(PORT2, DC_PIN + RST_PIN);
}

static void epd_setWindow(uint16_t Xstart, uint16_t Ystart,
		uint16_t Xend, uint16_t Yend)
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

static void epd_setCursor(uint16_t Xstart, uint16_t Ystart)
{
    sendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    sendData((Xstart >> 3) & 0xFF);

    sendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    sendData(Ystart & 0xFF);
    sendData((Ystart >> 8) & 0xFF);
}

static void epd_turnOnDisplay(void)
{
    sendCommand(0x22); // DISPLAY_UPDATE_CONTROL_2
    sendData(0xC4);
    sendCommand(0x20); // MASTER_ACTIVATION
    sendCommand(0xFF); // TERMINATE_FRAME_READ_WRITE

    while(gpio_getInputPin(PORT2, BUSY_PIN) == BUSY_PIN){
    	bcm_delay(100);
    }
}

void epd_clear(void)
{
    epd_setWindow(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

	epd_setCursor(0, 0);
	sendCommand(0x24);
	for (uint16_t i = 0; i < DISPLAY_NUM_BYTES; i++) {
		sendData(0XFF);
	}

    epd_turnOnDisplay();
}

void epd_display(uint8_t *image)
{
    epd_setWindow(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

    epd_setCursor(0, 0);
	sendCommand(0x24);
	for (uint16_t i = 0; i < DISPLAY_NUM_BYTES; i++) {
		sendData(image[i]);
	}
    epd_turnOnDisplay();
}

void epd_sleep(void)
{
    sendCommand(0x10);
    sendData(0x01);
}

static void sendCommand(uint8_t val)
{
	gpio_setOutputLow(PORT2, DC_PIN);
	usci_sendSPI(val);
}

static void sendData(uint8_t val)
{
	gpio_setOutputHigh(PORT2, DC_PIN);
	usci_sendSPI(val);
}
