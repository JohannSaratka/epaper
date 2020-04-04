/*
 * paint.h
 *
 *  Created on: 01.03.2020
 *      Author: johann
 */

#ifndef SRC_EPAPER_PAINT_H_
#define SRC_EPAPER_PAINT_H_

/**
 * image color
**/
#define WHITE          0xFF
#define BLACK          0x00

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

#ifdef __cplusplus

/**
 * Image attributes
**/
class Paint
{
public:
    EPD *epd;
    uint8_t *tileBuffer;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t WidthByte;
    uint16_t HeightByte;
    void init(EPD *epd, uint16_t Width, uint16_t Height);
    void displayTile(uint16_t x, uint16_t y);
};

#endif

#endif /* SRC_EPAPER_PAINT_H_ */
