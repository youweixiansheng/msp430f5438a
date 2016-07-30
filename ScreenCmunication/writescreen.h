/*
 * ctrlusart.h
 *
 *  Created on: 2016Äê7ÔÂ4ÈÕ
 *      Author: Lpw
 */

#ifndef CTRLUSART_H_
#define CTRLUSART_H_
#include "stdint.h"
int USCI_A1_URAT_read_screen_string(uint8_t *str , int* size);
void USCI_A1_UART_screen_string_transmitData(uint8_t *str);
#endif /* CTRLUSART_H_ */
