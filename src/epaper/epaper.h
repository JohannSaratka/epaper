/*
 * epaper.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_EPAPER_H_
#define SRC_EPAPER_EPAPER_H_

void epd_reset(void);
void epd_init(bool useFullUpdate);
void epd_setWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void epd_setCursor(uint16_t Xstart, uint16_t Ystart);
void epd_turnOnDisplay(void);
void epd_clear(void);

#endif /* SRC_EPAPER_EPAPER_H_ */
