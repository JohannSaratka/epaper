/*
 * epaper.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_EPAPER_H_
#define SRC_EPAPER_EPAPER_H_

void epd_init(bool useFullUpdate);
void epd_exit(void);
void epd_clear(void);
void epd_display(uint8_t *image);
void epd_sleep(void);

#endif /* SRC_EPAPER_EPAPER_H_ */
