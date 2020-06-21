/*
 * main.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <cstdint>
#include <cstdlib>
#include <msp430.h>

#include "hal.h"
#include "epaper.h"
#include "paint.h"
#include "tile.h"
#include "image_data.h"

#define LED1 BIT0 // LED1 P1.0 Digital Out
//uint8_t BlackImage[DISPLAY_NUM_BYTES];

void portInit()
{
	// configure ports
	gpio_setDirectionOutput(PORT1, LED1);
	gpio_setOutputLow(PORT1, LED1);
}

namespace{
	const EPD epd{};
	const Paint paint{epd, DISPLAY_WIDTH, DISPLAY_HEIGHT};
}

int main(void)
{
	hal::wdt.hold();
	// Set frequency
	bcm_setDCOFrequency();
	portInit();
	usci_initSPI();


	epd.init(true);
	// clear active frame and frame buffer
	epd.clear();
	epd.clear();

#if TILE_SAMPLE
	tile_clear(WHITE);

	uint16_t i;
	for(i = 0;i < 8; i++)
	{
		tile_setPixel(0, i, BLACK);
		tile_setPixel(i, 0, BLACK);
		tile_setPixel(i, i, BLACK);
	}
	for(i = 0;i < 200; i += 2*16)
	{
		paint.displayTile(0,i);
		paint.displayTile(i,0);
		paint.displayTile(i,i);
	}
	epd.turnOnDisplay();
	bcm_delay(1000);
#endif

	// 2. Drawing to the
	paint.drawPoint(5, 10, BLACK, Paint::DotPixel::DP1x1, Paint::DotStyle::Fill_Around);
	paint.drawPoint(5, 25, BLACK, Paint::DotPixel::DP2x2, Paint::DotStyle::Fill_Around);
	paint.drawPoint(5, 40, BLACK, Paint::DotPixel::DP3x3, Paint::DotStyle::Fill_Around);
	paint.drawPoint(5, 55, BLACK, Paint::DotPixel::DP5x5, Paint::DotStyle::Fill_Around);

	paint.drawLine(20, 10, 70, 60, BLACK, Paint::DotPixel::DP2x2, Paint::LineStyle::Solid);
	paint.drawLine(70, 10, 20, 60, BLACK, Paint::DotPixel::DP3x3, Paint::LineStyle::Dotted);
	paint.drawLine(170, 15, 170, 55, BLACK, Paint::DotPixel::DP2x2, Paint::LineStyle::Dotted);
	paint.drawLine(150, 35, 190, 35, BLACK, Paint::DotPixel::DP1x1, Paint::LineStyle::Solid);


	paint.drawRectangle(20, 10, 70, 60, BLACK, Paint::DotPixel::DP1x1, Paint::GraphicFillStyle::Empty);
	paint.drawRectangle(85, 10, 130, 60, BLACK, Paint::DotPixel::DP1x1, Paint::GraphicFillStyle::Full);

	paint.drawCircle(170, 35, 20, BLACK, Paint::DotPixel::DP1x1, Paint::GraphicFillStyle::Empty);
	paint.drawCircle(170, 85, 20, BLACK, Paint::DotPixel::DP1x1, Paint::GraphicFillStyle::Full);

	#if 0
	Paint_DrawString_EN(5, 85, "waveshare", &Font20, BLACK, WHITE);
	Paint_DrawNum(5, 110, 123456789, &Font20, BLACK, WHITE);

	Paint_DrawString_CN(5, 135,"ÄãºÃabc", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(5, 155, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);

	EPD_1IN54_Display(BlackImage);
	DEV_Delay_ms(2000);
#endif
	epd.turnOnDisplay();
	bcm_delay(3000);

#if 0   //Partial refresh, example shows time
	epd_init(false);
	Paint_SelectImage(BlackImage);
	PAINT_TIME sPaint_time;
	sPaint_time.Hour = 12;
	sPaint_time.Min = 34;
	sPaint_time.Sec = 56;
	uint8_t num = 20;
	for (;;) {
		sPaint_time.Sec = sPaint_time.Sec + 1;
		if (sPaint_time.Sec == 60) {
			sPaint_time.Min = sPaint_time.Min + 1;
			sPaint_time.Sec = 0;
			if (sPaint_time.Min == 60) {
				sPaint_time.Hour =  sPaint_time.Hour + 1;
				sPaint_time.Min = 0;
				if (sPaint_time.Hour == 24) {
					sPaint_time.Hour = 0;
					sPaint_time.Min = 0;
					sPaint_time.Sec = 0;
				}
			}
		}
		Paint_ClearWindows(15, 65, 15 + Font20.Width * 7, 65 + Font20.Height, WHITE);
		Paint_DrawTime(15, 65, &sPaint_time, &Font20, WHITE, BLACK);
		num = num - 1;
		if(num == 0) {
			break;
		}
		EPD_1IN54_Display(BlackImage);
		DEV_Delay_ms(500);//Analog clock 1s
	}

#endif
#if 0
	epd_init(true);
	epd_clear();

	epd_sleep();
#endif

	while(1)
	{
		gpio_toggleOutput(PORT1, LED1);
		bcm_delay(300);
		gpio_toggleOutput(PORT1, LED1);
		bcm_delay(500);
	}
	return 0;
}
