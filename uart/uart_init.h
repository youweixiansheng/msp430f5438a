/*
 * uart_init.h
 *
 *  Created on: 2016��7��4��
 *      Author: Lpw
 */

#ifndef UART_INIT_H_
#define UART_INIT_H_


int UartScreenInit(void);
int UartPCInit(void);
void uartStrncpy(uint8_t *src,uint8_t *dec,int size);
#endif /* UART_INIT_H_ */
