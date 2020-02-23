/*
 * usci.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <stdint.h>
#include <msp430.h>
#include "gpio.h"

#define CS_PIN BIT0 // CS P2.0 SPI CS

void usci_initSPI()
{
	UCB0CTL1 |= UCSWRST;
	// init usci registers
	// 8Bit, MSB first, Master, mode 0 (3wire), Phase 0 Polarity 0
	UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCMODE_0 + UCSYNC;

	UCB0CTL1 |= UCSSEL_2; // SMCLK
	UCB0BR0 |= 0x04;// clock source division factor
	UCB0BR1 = 0x00;
	UCA0MCTL = 0; // No modulation
	UCB0CTL1 &= ~UCSWRST;

	gpio_setDirectionOutput(PORT2,CS_PIN);
	gpio_setOutputHigh(PORT2, CS_PIN);
}

void usci_sendSPI(uint8_t val)
{
	gpio_setOutputLow(PORT2, CS_PIN);
	UCB0TXBUF = val;
	while(UCB0STAT & UCBUSY);
	gpio_setOutputHigh(PORT2, CS_PIN);
}
