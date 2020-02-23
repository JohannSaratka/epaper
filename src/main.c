/*
 * main.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>

#include "hal.h"
#include "epaper.h"
#define DIN_PIN BIT7 // DIN P1.7 SPI MOSI
#define CLK_PIN BIT5 // CLK	| P1.5 SPI SCK

#define LED1 BIT0 // LED1 P1.0 Digital Out

void portInit()
{
	// configure ports
	P1SEL |= DIN_PIN + CLK_PIN;
	P1SEL2 |= DIN_PIN + CLK_PIN;
	gpio_setDirectionOutput(PORT1,LED1);
}


int main(void)
{
	wdt_stop();
	// Set frequency
	bcm_setDCOFrequency();
	portInit();
	usci_initSPI();
	epd_init(true);

	// Clear the EPD
	epd_clear();
	epd_clear(); // TODO is twice really necessary?

	// Load image data and display the image on EPD

	// Turn off oscillator clock and DC/DC & regulator

	//Power Down or Enter into deep sleep mode
	while(1)
	{
		gpio_toggleOutput(PORT1, LED1);
		bcm_delay(250);
	}
	return 0;
}
