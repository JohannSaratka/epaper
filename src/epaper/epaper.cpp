/*
 * epaper.c
 *
 *  Created on: 10.02.2020
 *      Author: johann
 */

#include <cstdint>
#include <msp430.h>

#include "hal.h"
#include "epaper.h"

#define DC_PIN BIT5 // DC P2.5 Digital Out
#define RST_PIN BIT4 // RST P2.4 Digital Out
#define BUSY_PIN BIT3 // BUSY P2.3 Digital In

#if (DISPLAY_WIDTH % 8U == 0)
#define DISPLAY_NUM_BYTES ((DISPLAY_WIDTH / 8U ) * DISPLAY_HEIGHT)
#else
#define DISPLAY_NUM_BYTES ((DISPLAY_WIDTH / 8U + 1U) * DISPLAY_HEIGHT)
#endif

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

EPD::~EPD()
{

};

EPD::EPD()
{

};

void EPD::init(const bool useFullUpdate) const
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
	sendCommand(0x32);
	for (uint16_t i = 0; i < 30; i++)
	{
		sendData(lut[i]);
	}
}

/** Module reset.
 *
 * Often used to awaken the module in deep sleep, \see EPD::sleep().
 */
void EPD::reset() const
{
	bcm_delay(200);
	gpio_setOutputLow(PORT2, RST_PIN);
	bcm_delay(200);
	gpio_setOutputHigh(PORT2, RST_PIN);
	bcm_delay(200);
}

/** Specify the memory area for data R/W
 * SetMemoryArea
 */
void EPD::setWindow(const uint16_t Xstart, const uint16_t Ystart,
		const uint16_t Xend, const uint16_t Yend) const
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
/** Specify the start point for data R/W
 *  SetMemoryPointer
 */
void EPD::setCursor(const uint16_t Xstart, const uint16_t Ystart) const
{
    sendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    sendData((Xstart >> 3) & 0xFF);

    sendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    sendData(Ystart & 0xFF);
    sendData((Ystart >> 8) & 0xFF);
}
/** Update the display.
 *
 * There are two memory areas embedded in the e-paper display
 * This function will switch the active area while
 * EPD::display and EPD::clear always operate on the other memory area.
*/
void EPD::turnOnDisplay() const
{
    sendCommand(0x22); // DISPLAY_UPDATE_CONTROL_2
    sendData(0xC4);
    sendCommand(0x20); // MASTER_ACTIVATION
    sendCommand(0xFF); // TERMINATE_FRAME_READ_WRITE

    while(gpio_getInputPin(PORT2, BUSY_PIN) == BUSY_PIN){
    	bcm_delay(100);
    }
}

/** Clear the frame memory with the specified color.
 *
 * This will update the display.
 */
void EPD::clear()const
{
    EPD::setWindow(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

	EPD::setCursor(0, 0);
	sendCommand(0x24);
	for (uint16_t i = 0; i < DISPLAY_NUM_BYTES; i++) {
		sendData(0XFF);
	}

    EPD::turnOnDisplay();
}

/** Put an image buffer to the frame memory.
 *
 * This will update the display with data from image pointer.
 * It will read DISPLAY_WIDTH * DISPLAY_HEIGHT bytes of data.
 */
void EPD::display(uint8_t *const image) const
{
	if(image == nullptr)
	{
		return;
	}
    EPD::setWindow(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

    EPD::setCursor(0, 0);
	sendCommand(0x24);
	for (uint16_t i = 0; i < DISPLAY_NUM_BYTES; i++) {
		sendData(image[i]);
	}
    EPD::turnOnDisplay();
}

/** Put an arbitrary size image buffer to the frame memory.
 *
 * This will update the display with data from image pointer.
 * It will read image_width * image_height bytes of data and
 * display it at x/y coordinates.
 */
void EPD::displayImage(const uint8_t *const image_buffer,
		const uint16_t x, const uint16_t y,
		uint16_t image_width, const uint16_t image_height) const
{
    int x_end;
    int y_end;

    if (image_buffer == nullptr)
    {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    uint16_t x_base = x & 0xF8;
    image_width &= 0xF8;

    if (x_base + image_width >= DISPLAY_WIDTH)
    {
        x_end = DISPLAY_WIDTH - 1;
    }
    else
    {
        x_end = x_base + image_width - 1;
    }
    if (y + image_height >= DISPLAY_HEIGHT)
    {
        y_end = DISPLAY_HEIGHT - 1;
    }
    else
    {
        y_end = y + image_height - 1;
    }
    setWindow(x_base, y, x_end, y_end);
    setCursor(x_base, y);
    sendCommand(0x24);
    /* send the tile data */
    for (uint16_t j = 0; j < y_end - y + 1; j++)
    {
        for (uint16_t i = 0; i < (x_end - x_base + 1) / 8; i++)
        {
            sendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}

void EPD::setByte(const uint16_t x, const uint16_t y, const uint8_t value) const
{
    uint16_t x_base = x & 0xF8;

    setWindow(x_base, y, x_base + 1, y + 1);
    setCursor(x_base, y);
    sendCommand(0x24);
    /* send the tile data */
    sendData(value);

}

/** Enter deep sleep
 *
 * After this command is transmitted, the chip would enter the
 * deep-sleep mode to save power. The deep sleep mode would
 * return to standby by hardware reset. You can use EPD::init()
 * to awaken
 */
void EPD::sleep() const
{
    sendCommand(0x10);
    sendData(0x01);

    gpio_setOutputLow(PORT2, DC_PIN + RST_PIN);
}

/**
 * Basic function for sending commands
 */
void EPD::sendCommand(const uint8_t val) const
{
	gpio_setOutputLow(PORT2, DC_PIN);
	usci_sendSPI(val);
}

/**
 *  Basic function for sending data
 */
void EPD::sendData(const uint8_t val) const
{
	gpio_setOutputHigh(PORT2, DC_PIN);
	usci_sendSPI(val);
}
