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
void Paint::init(EPD *pEpd, uint16_t Width, uint16_t Height)
{
	tileBuffer = getTileBufferAddr();
	
	WidthMemory = Width;
	HeightMemory = Height;

	WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
	HeightByte = Height;

	epd = pEpd;
}

void Paint::displayTile(uint16_t x, uint16_t y)
{
	epd->displayImage(tileBuffer, x, y, TILE_WIDTH, TILE_HEIGHT);
}
/* TODO PAINT:
 * properties: rotate, mirror, scale, color

void Paint_Clear(uint16_t Color);

void Paint_ClearWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color);

void Paint_DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color,
		DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)

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
