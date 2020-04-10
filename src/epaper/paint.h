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
	Paint(const EPD &epd_ref, const uint16_t Width, const uint16_t Height);
    void displayTile(uint16_t x, uint16_t y) const;
private:
    const EPD &epd;
    const uint16_t WidthMemory;
    const uint16_t HeightMemory;
    const uint16_t WidthByte;
    const uint16_t HeightByte;
    const uint8_t *tileBuffer;
};

#endif

#endif /* SRC_EPAPER_PAINT_H_ */
