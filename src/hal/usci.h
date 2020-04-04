/*
 * usci.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_HAL_USCI_H_
#define SRC_HAL_USCI_H_

#ifdef __cplusplus
extern "C" {
#endif

void usci_initSPI();
void usci_sendSPI(uint8_t val);

#ifdef __cplusplus
}
#endif

#endif /* SRC_HAL_USCI_H_ */
