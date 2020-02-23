/*
 * wdt.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <stdint.h>
#include <msp430.h>

void wdt_stop()
{
	WDTCTL = WDTPW + WDTHOLD;
}
