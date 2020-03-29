/*
 * epaper.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_EPAPER_H_
#define SRC_EPAPER_EPAPER_H_

#define DISPLAY_WIDTH (200U)
#define DISPLAY_HEIGHT (200U)

void epd_init(bool useFullUpdate);
void epd_exit(void);
void epd_clear(void);
void epd_display(uint8_t *image);
void epd_displayImage(uint8_t *image_buffer, uint16_t x, uint16_t y,
	    uint16_t image_width, uint16_t image_height);
void epd_turnOnDisplay(void);
void epd_sleep(void);

#endif /* SRC_EPAPER_EPAPER_H_ */
