/*
 * gpio.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_HAL_GPIO_H_
#define SRC_HAL_GPIO_H_

typedef struct
{
	uint8_t volatile PxIN;
	uint8_t volatile PxOUT;
	uint8_t volatile PxDIR;
	uint8_t volatile PxIFG;
	uint8_t volatile PxIES;
	uint8_t volatile PxIE;
	uint8_t volatile PxSEL;
	uint8_t volatile PxREN;
	uint8_t volatile reserved[25];
	uint8_t volatile PxSEL2;
}port_regs;

extern port_regs *const PORT1;
extern port_regs *const PORT2;

void gpio_setDirectionOutput(port_regs *port, uint8_t pin);
void gpio_setOutputHigh(port_regs *port, uint8_t pin);
void gpio_setOutputLow(port_regs *port, uint8_t pin);
void gpio_toggleOutput(port_regs *port, uint8_t pin);

uint8_t gpio_getInputPin(port_regs *port, uint8_t pin);

#endif /* SRC_HAL_GPIO_H_ */