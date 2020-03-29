/*
 * paint.h
 *
 *  Created on: 01.03.2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_PAINT_H_
#define SRC_EPAPER_PAINT_H_

#include "fonts.h"

/**
 * Image attributes
**/
typedef struct {
    uint8_t *tileBuffer;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t WidthByte;
    uint16_t HeightByte;
} PAINT;
extern PAINT Paint;

/**
 * image color
**/
#define WHITE          0xFF
#define BLACK          0x00

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

int16_t paint_init(uint16_t Width, uint16_t Height);
void paint_displayTile(uint16_t x, uint16_t y);


#endif /* SRC_EPAPER_PAINT_H_ */
