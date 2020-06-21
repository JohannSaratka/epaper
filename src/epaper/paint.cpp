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

bool Paint::inDisplayRange(const uint16_t x, const uint16_t y) const
{
	return ((x <= WidthMemory) && (y <= HeightMemory));
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
	static uint8_t lastYPos = 0;
	static uint8_t lastXVal = IMAGE_BACKGROUND;

	if (!inDisplayRange(x, y))
	{
		return;
	}

	if(((x & 0xF8) != lastXPos)||(y != lastYPos))
	{
		lastXVal = IMAGE_BACKGROUND;
		lastXPos = x & 0xF8;
		lastYPos = y;
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

void Paint::drawPoint(const uint16_t x, const uint16_t y, const uint16_t color,
	const DotPixel dotPixel, const DotStyle dotStyle) const
{
	if (!inDisplayRange(x, y)) {
		//        Debug("Input exceeds the normal display range\r\n");
		//        printf("x = %d , WidthMemory = %d  \r\n ", x, WidthMemory);
		//        printf("y = %d , HeightMemory = %d  \r\n ", y, HeightMemory);
		return;
	}

	int16_t sizePixel = static_cast<uint16_t>(dotPixel);
	if (dotStyle == DotStyle::Fill_Around)
	{
		for (int16_t yDirNum = 0; yDirNum < 2 * sizePixel - 1; yDirNum++)
		{
			for (int16_t xDirNum = 0; xDirNum < 2 * sizePixel - 1; xDirNum++)
			{
				if( ((int16_t)(x + xDirNum - sizePixel) < 0)
					|| ((int16_t)(y + yDirNum - sizePixel) < 0))
				{
					break;
				}
				// printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
				setPixel(
					x + xDirNum - sizePixel,
					y + yDirNum - sizePixel,
					color);
			}
		}
	}
	else if (dotStyle == DotStyle::Fill_Bar)
	{
		for (int16_t xDirNum = 0; xDirNum <  sizePixel; xDirNum++)
		{
			setPixel(
				x + xDirNum - 1,
				y - 1,
				color);
		}
	}
	else
	{
		for (int16_t yDirNum = 0; yDirNum <  sizePixel; yDirNum++)
		{
			for (int16_t xDirNum = 0; xDirNum <  sizePixel; xDirNum++)
			{
				setPixel(
					x + xDirNum - 1,
					y + yDirNum - 1,
					color);
			}
		}
	}

}

void Paint::drawLine(const uint16_t xStart, const uint16_t yStart,
	const uint16_t xEnd, const uint16_t yEnd, const uint16_t color,
	const DotPixel lineWidth, const LineStyle lineStyle) const
{
	if ((!inDisplayRange(xStart, yStart)) ||
		(!inDisplayRange(xEnd,yEnd)))
	{
		//Debug("Input exceeds the normal display range\r\n");
		return;
	}

	uint16_t x = xStart;
	uint16_t y = yStart;
	const int16_t dx = (int16_t)xEnd - (int16_t)xStart >= 0 ? xEnd - xStart : xStart - xEnd;
	const int16_t dy = (int16_t)yEnd - (int16_t)yStart <= 0 ? yEnd - yStart : yStart - yEnd;

	// Increment direction, 1 is positive, -1 is counter;
	const int16_t xInc = xStart < xEnd ? 1 : -1;
	const int16_t yInc = yStart < yEnd ? 1 : -1;

	//Cumulative error
	int16_t err = dx + dy;
	int8_t lenghtDotted = 0;

	for (;;)
	{
		lenghtDotted++;
		//Painted dotted line, 2 point is really virtual
		if ((lineStyle == LineStyle::Dotted) && (lenghtDotted % 3 == 0))
		{
			//Debug("LINE_DOTTED\r\n");
			drawPoint(x, y, IMAGE_BACKGROUND, lineWidth, DotStyle::Fill_Bar);
			lenghtDotted = 0;
		}
		else
		{
			drawPoint(x, y, color, lineWidth, DotStyle::Fill_Bar);
		}

		if (2 * err >= dy)
		{
			if (x == xEnd)
			{
				break;
			}
			err += dy;
			x += xInc;
		}
		if (2 * err <= dx)
		{
			if (y == yEnd)
			{
				break;
			}
			err += dx;
			y += yInc;
		}
	}
}

void Paint::drawRectangle(const uint16_t xStart, const uint16_t yStart,
	const uint16_t xEnd, const uint16_t yEnd, const uint16_t color,
	const DotPixel lineWidth, const GraphicFillStyle drawFill) const
{
    if ((!inDisplayRange(xStart, yStart)) ||
		(!inDisplayRange(xEnd,yEnd)))
    {
        //Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (drawFill== GraphicFillStyle::Full)
    {
        for(uint16_t y = yStart; y < yEnd; y++)
        {
            drawLine(xStart, y, xEnd, y, color , lineWidth, LineStyle::Solid);
        }
    } else {
        drawLine(xStart, yStart, xStart, yEnd, color, lineWidth, LineStyle::Solid);
        drawLine(xEnd, yEnd, xEnd, yStart, color, lineWidth, LineStyle::Solid);
        drawLine(xStart, yStart, xEnd, yStart, color, lineWidth, LineStyle::Solid);
        drawLine(xEnd, yEnd, xStart, yEnd, color, lineWidth, LineStyle::Solid);
    }
}

void Paint::drawCircle(const uint16_t xCenter, const uint16_t yCenter,
	const uint16_t radius, const uint16_t color,
	const DotPixel lineWidth, const GraphicFillStyle drawFill) const
{
    if (!inDisplayRange(xCenter, yCenter)) {
       // Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (drawFill == GraphicFillStyle::Full)
	{
    	drawSolidCircle(xCenter, yCenter, radius, color);
	}
	else
	{
		drawEmptyCircle(xCenter, yCenter, radius, color, lineWidth);
	}
}

void Paint::drawSolidCircle(const uint16_t xCenter, const uint16_t yCenter,
	const uint16_t radius, const uint16_t color) const
{
    // Draw a circle from(0, R) as a starting point
    int16_t xCurrent = 0;
	int16_t yCurrent = radius;

    // Cumulative error
    int16_t err = 3 - (2 * radius);

	while (xCurrent <= yCurrent )
	{
		drawLine(xCenter + xCurrent, yCenter + yCurrent,
			xCenter - xCurrent, yCenter + yCurrent,
			color, DotPixel::DP1x1, LineStyle::Solid);
		drawLine(xCenter + yCurrent, yCenter + xCurrent,
			xCenter - yCurrent, yCenter + xCurrent,
			color, DotPixel::DP1x1, LineStyle::Solid);
		drawLine(xCenter + yCurrent, yCenter - xCurrent,
			xCenter - yCurrent, yCenter - xCurrent,
			color, DotPixel::DP1x1, LineStyle::Solid);
		drawLine(xCenter + xCurrent, yCenter - yCurrent,
			xCenter - xCurrent, yCenter - yCurrent,
			color, DotPixel::DP1x1, LineStyle::Solid);
		if (err < 0 )
		{
			err += 4 * xCurrent + 6;
		}
		else
		{
			err += 10 + 4 * (xCurrent - yCurrent );
			yCurrent --;
		}
		xCurrent ++;
	}
}

void Paint::drawEmptyCircle(const uint16_t xCenter, const uint16_t yCenter,
	const uint16_t radius, const uint16_t color, const DotPixel lineWidth) const
{
	/* function for drawing single segment with bresenham algorithm at once.
	 * We can't draw all segments at the same time because jumping between the different
	 * coordinates will reset the memory of the last byte written to the frame buffer.
	 */
	auto drawSegment =
		[=](bool swapXY, int16_t signX, int16_t signY)
	{
		// starting point
		int16_t x = 0;
		int16_t y = radius;

		// Cumulative error
		int16_t err = 3 - (2 * radius);

		while (x <= y)
		{
			// Draw a circle segment (1/8)
			if (swapXY)
			{
				drawPoint(xCenter + signX * y, yCenter + signY * x,
					color, lineWidth, DotStyle::Fill_Around);
			}
			else
			{
				drawPoint(xCenter + signX * x, yCenter + signY * y,
					color, lineWidth, DotStyle::Fill_Around);
			}
			if (err < 0)
			{
				err += 4 * x + 6;
			}
			else
			{
				err += 10 + 4 * (x - y);
				y--;
			}
			x++;
		}
	};
	// Draw a circle from segments
	drawSegment(false, +1, +1);
	drawSegment(false, -1, +1);
	drawSegment(false, +1, -1);
	drawSegment(false, -1, -1);
	drawSegment(true, +1, +1);
	drawSegment(true, -1, +1);
	drawSegment(true, +1, -1);
	drawSegment(true, -1, -1);
}
/*
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
