/*
 * paint.h
 *
 * Based on GUI_Paint by waveshare \see https://github.com/waveshare/e-Paper
 *  Created on: 01.03.2020
 *      Author: johann
 */
/******************************************************************************

 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "epaper.h"
#include "paint.h"

#include "tile.h"


/** Init paint object
 *
 * \param width The width of the picture
 * \param height The height of the picture
 */
Paint::Paint(const EPD &epd_ref, const uint16_t Width, const uint16_t Height):
	epd(epd_ref),
	WidthMemory(Width),
	HeightMemory( Height),
	WidthByte((Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1)),
	HeightByte(Height),
	tileBuffer(getTileBufferAddr())
{
}

void Paint::displayTile(uint16_t x, uint16_t y) const
{
	epd.displayImage(tileBuffer, x, y, TILE_WIDTH, TILE_HEIGHT);
}
/* TODO PAINT: properties: rotate, mirror, scale, background color

void Paint_Clear(uint16_t Color);

void Paint_ClearWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color);
*/
void Paint::setPixel(const uint16_t x, const uint16_t y, const uint16_t color) const
{
    static uint8_t lastXPos = 0;
    static uint8_t lastXVal = WHITE;
    if (x > WidthMemory || y > HeightMemory)
    {
        return;
    }

    if((x & 0xF8) != lastXPos)
    {
        lastXVal = WHITE;
        lastXPos = x & 0xF8;
    }

    if (color == BLACK)
    {
        lastXVal &= ~(0x80 >> (x & 0x07));
    }
    else
    {
        lastXVal |= (0x80 >> (x & 0x07));
    }

    epd.setByte(x, y, lastXVal);
}

void Paint::drawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color,
		DotPixel Dot_Pixel, DotStyle Dot_Style) const
{
    if (Xpoint > WidthMemory || Ypoint > HeightMemory) {
//        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
//        printf("Xpoint = %d , WidthMemory = %d  \r\n ", Xpoint, WidthMemory);
//        printf("Ypoint = %d , HeightMemory = %d  \r\n ", Ypoint, HeightMemory);
        return;
    }

    int16_t XDir_Num , YDir_Num;
    int16_t sizePixel = static_cast<uint16_t>(Dot_Pixel);
    if (Dot_Style == DotStyle::Fill_Around)
    {
        for (XDir_Num = 0; XDir_Num < 2 * sizePixel - 1; XDir_Num++)
        {
            for (YDir_Num = 0; YDir_Num < 2 * sizePixel - 1; YDir_Num++)
            {
                if( ((int16_t)(Xpoint + XDir_Num - sizePixel) < 0)
                    || ((int16_t)(Ypoint + YDir_Num - sizePixel) < 0))
                {
                    break;
                }
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                setPixel(
                    Xpoint + XDir_Num - sizePixel,
                    Ypoint + YDir_Num - sizePixel,
                    Color);
            }
        }
    }
    else
    {
        for (XDir_Num = 0; XDir_Num <  sizePixel; XDir_Num++)
        {
            for (YDir_Num = 0; YDir_Num <  sizePixel; YDir_Num++)
            {
                setPixel(
                    Xpoint + XDir_Num - 1,
                    Ypoint + YDir_Num - 1,
                    Color);
            }
        }
    }

}
/*
Paint_DrawLine(..., DOT_PIXEL Line_width, LINE_STYLE Line_Style)

Paint_DrawRectangle(..., DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)

Paint_DrawCircle(..., DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)

void Paint_DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char Acsii_Char,
		sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)

void Paint_DrawString(uint16_t Xstart, uint16_t Ystart, const char * pString,
		sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)

void Paint_DrawNum(uint16_t Xpoint, uint16_t Ypoint, int32_t Nummber,
		sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)

void Paint_DrawTime(uint16_t Xstart, uint16_t Ystart, PAINT_TIME *pTime,
		sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)

void Paint_DrawBitMap(const unsigned char* image_buffer)

void Paint_DrawBitMap_Block(const unsigned char* image_buffer, uint8_t Region)
 */
