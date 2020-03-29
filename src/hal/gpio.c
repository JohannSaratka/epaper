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
	gpio_setPeripheralFunction(port, pin, GPIO_MODULE_FUNCTION_IO);
	port->PxDIR |= pin;
}

void gpio_setDirectionInput(port_regs *port, uint8_t pin)
{
	gpio_setPeripheralFunction(port, pin, GPIO_MODULE_FUNCTION_IO);
	port->PxDIR &= ~pin;
}

void gpio_setPeripheralFunction(port_regs *port, uint8_t pin, enum gpio_module_function mode)
{
	switch(mode)
	{
		case GPIO_MODULE_FUNCTION_IO:
			port->PxSEL &= ~pin;
			port->PxSEL2 &= ~pin;
			break;
		case GPIO_MODULE_FUNCTION_PRIMARY:
			port->PxSEL |= pin;
			port->PxSEL2 &= ~pin;
			break;
		case GPIO_MODULE_FUNCTION_SPECIAL:
			port->PxSEL &= ~pin;
			port->PxSEL2 |= pin;
			break;
		case GPIO_MODULE_FUNCTION_SECONDARY:
			port->PxSEL |= pin;
			port->PxSEL2 |= pin;
			break;
		default:
			// this should not be possible
			break;
	}
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
