/*
 * bcm.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <stdint.h>
#include <msp430.h>

#define CYCLES_PER_MS 2047 // TODO: this value is not accurate and depends on system clock

void bcm_setDCOFrequency()
{
	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;
}
// TODO move to tools folder
void bcm_delay(uint16_t timeout_ms)
{
	uint16_t i;
	uint16_t j;
	for(i = 0 ;i < timeout_ms; i++)
	{
		j = CYCLES_PER_MS;
		while(j--)
		{
			__nop();
		}
	}
}

void bcm_delay_intr(uint16_t timeout_ms)
{
	for(uint16_t i = 0 ;i < timeout_ms; i++)
	{
		__delay_cycles(1000);
	}
}
