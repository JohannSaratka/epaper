/*
 * usci.c
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */
#include <stdint.h>
#include <msp430.h>
#include "gpio.h"

port_regs *const PORT1 = (port_regs *) &P1IN;
port_regs *const PORT2 = (port_regs *) &P2IN;

void gpio_setDirectionOutput(port_regs *port, uint8_t pin)
{
	port->PxDIR |= pin;
}

void gpio_setOutputHigh(port_regs *port, uint8_t pin)
{
	port->PxOUT |= pin;
}

void gpio_setOutputLow(port_regs *port, uint8_t pin)
{
	port->PxOUT &= ~pin;
}

void gpio_toggleOutput(port_regs *port, uint8_t pin)
{
	port->PxOUT ^= pin;
}

uint8_t gpio_getInputPin(port_regs *port, uint8_t pin)
{
	return (port->PxIN & pin);
}
