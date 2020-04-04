/*
 * bcm.h
 *
 *  Created on: 22.02.2020
 *      Author: johann
 */

#ifndef SRC_HAL_BCM_H_
#define SRC_HAL_BCM_H_

#ifdef __cplusplus
extern "C" {
#endif

void bcm_setDCOFrequency();
void bcm_delay(uint16_t timeout_ms);
void bcm_delayintr(uint16_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* SRC_HAL_BCM_H_ */
