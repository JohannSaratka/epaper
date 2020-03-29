/*
 * tile.c
 *
 *  Created on: Mar 29, 2020
 *      Author: johann
 */

#include <stdint.h>
#include <stdbool.h>

#include "tile.h"
#include "paint.h"

uint8_t tileBuffer[TILE_HEIGHT]; // 1 tile row = 8 Bit, 8 rows = 8 bytes
inline uint8_t *getTileBufferAddr(){ return &tileBuffer[0]; }

/** Clear the tile buffer
 *
 * \param Color : Painted colors
 */
void tile_clear(uint16_t color)
{
	for (uint16_t Y = 0; Y < TILE_HEIGHT; Y++)
	{
		tileBuffer[Y] = color;
	}
}

/** Draw Pixels
 *
 * \param x_point At point X
 * \param y_point At point Y
 * \param color Painted colors
 */
void tile_setPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color)
{
	if (xPoint > TILE_WIDTH || yPoint > TILE_HEIGHT)
	{
		return;
	}
	if (color == BLACK)
	{
		tileBuffer[yPoint] &= ~(0x80 >> xPoint);
	}
	else
	{
		tileBuffer[yPoint] |= (0x80 >> xPoint);
	}
}

/* TODO TILE:
 *
 */
