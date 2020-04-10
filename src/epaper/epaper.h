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
	void init(const bool useFullUpdate)const;
	void exit()const;
	void clear()const;
	void display(uint8_t *const image)const;
	void displayImage(const uint8_t *const image_buffer,
			uint16_t x, const uint16_t y,
			uint16_t image_width, const uint16_t image_height) const;
	void turnOnDisplay()const;
	void sleep()const;

private:
	void sendCommand(const uint8_t val) const;
	void sendData(const uint8_t val) const;
	void reset() const;
	void setWindow(const uint16_t Xstart, const uint16_t Ystart,
			const uint16_t Xend, const uint16_t Yend) const;
	void setCursor(const uint16_t Xstart, const uint16_t Ystart) const;
};

#endif /* SRC_EPAPER_EPAPER_H_ */
