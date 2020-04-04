/*
 * epaper.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_EPAPER_H_
#define SRC_EPAPER_EPAPER_H_


#include <cstdint>

#define DISPLAY_WIDTH (200U)
#define DISPLAY_HEIGHT (200U)

class EPD
{

public:
	EPD();
	~EPD();
	void init(bool useFullUpdate);
	void exit();
	void clear();
	void display(uint8_t *image);
	void displayImage(uint8_t *image_buffer, uint16_t x, uint16_t y,
			uint16_t image_width, uint16_t image_height);
	void turnOnDisplay();
	void sleep();

private:
	void sendCommand(uint8_t val);
	void sendData(uint8_t val);
	void reset();
	void setWindow(uint16_t Xstart, uint16_t Ystart,
			uint16_t Xend, uint16_t Yend);
	void setCursor(uint16_t Xstart, uint16_t Ystart);
};

#endif /* SRC_EPAPER_EPAPER_H_ */
