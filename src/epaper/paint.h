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
    /* Size of pixel */
    enum class DotPixel
    {
        DP1x1 = 1,
        DP2x2,
        DP3x3,
        DP4x4,
        DP5x5,
        DP6x6,
        DP7x7,
        DP8x8
    };

    /* Pixel fill style */
    enum class DotStyle
    {
        Fill_Around = 1,
        Fill_RightUp,
    };

	Paint(const EPD &epd_ref, const uint16_t Width, const uint16_t Height);
    void displayTile(uint16_t x, uint16_t y) const;

    void drawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color,
        DotPixel Dot_Pixel, DotStyle Dot_Style) const;
private:
    const EPD &epd;
    const uint16_t WidthMemory;
    const uint16_t HeightMemory;
    const uint16_t WidthByte;
    const uint16_t HeightByte;
    const uint8_t *tileBuffer;//TODO tile buffer should be input to specific function not member


    void setPixel(uint16_t Xpoint, uint16_t Ypoint,uint16_t Color) const;
};

#endif

#endif /* SRC_EPAPER_PAINT_H_ */
