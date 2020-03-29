/*
 * main.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <msp430.h>

#include "hal.h"
#include "epaper.h"
#include "paint.h"
#include "image_data.h"

#define LED1 BIT0 // LED1 P1.0 Digital Out
//uint8_t BlackImage[DISPLAY_NUM_BYTES];

void portInit()
{
	// configure ports
	gpio_setDirectionOutput(PORT1, LED1);
	gpio_setOutputLow(PORT1, LED1);
}


int main(void)
{
	wdt_stop();
	// Set frequency
	bcm_setDCOFrequency();
	portInit();
	usci_initSPI();

	epd_init(true);
	epd_clear();

#if 0   //show image for array
	//Create a new image cache
	/* you have to set a big enough heap size */
	uint8_t *BlackImage = (uint8_t *)malloc(DISPLAY_NUM_BYTES);
	if(BlackImage == NULL)
	{
		return -1;
	}
	Paint_NewImage(BlackImage, DISPLAY_WIDTH, DISPLAY_HEIGHT, 270, WHITE);


	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_1in54);

	epd_display(BlackImage);
	bcm_delay(2000);
#endif
	epd_display((uint8_t *)gImage_1in54);
	bcm_delay(2000);
#if 0   // Drawing on the image
	//1.Select Image
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);

	// 2.Drawing on the image
	Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

	Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(170, 15, 170, 55, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	Paint_DrawLine(150, 35, 190, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

	Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawRectangle(85, 10, 130, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

	Paint_DrawCircle(170, 35, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawCircle(170, 85, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	Paint_DrawString_EN(5, 85, "waveshare", &Font20, BLACK, WHITE);
	Paint_DrawNum(5, 110, 123456789, &Font20, BLACK, WHITE);

	Paint_DrawString_CN(5, 135,"ÄãºÃabc", &Font12CN, BLACK, WHITE);
	Paint_DrawString_CN(5, 155, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);

	EPD_1IN54_Display(BlackImage);
	DEV_Delay_ms(2000);
#endif

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

	epd_init(true);
	epd_clear();

	epd_sleep();
//	free(BlackImage);
//	BlackImage = NULL;

	// close 5V
	epd_exit();

	while(1)
	{
		gpio_toggleOutput(PORT1, LED1);
		bcm_delay(300);
		gpio_toggleOutput(PORT1, LED1);
		bcm_delay(500);
	}
	return 0;
}
