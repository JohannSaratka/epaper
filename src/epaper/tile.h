/*
 * tile.h
 *
 *  Created on: Mar 29, 2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_TILE_H_
#define SRC_EPAPER_TILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TILE_WIDTH (8U)
#define TILE_HEIGHT (8U)
#define TILE_SIZE (TILE_WIDTH * TILE_HEIGHT)


void tile_clear(uint16_t color);
void tile_setPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color);
extern uint8_t *getTileBufferAddr();

#ifdef __cplusplus
}
#endif

#endif /* SRC_EPAPER_TILE_H_ */
