/*
 * humancontrol.h
 *
 *  Created on: 2016Äê7ÔÂ14ÈÕ
 *      Author: Lpw
 */

#ifndef PCCOMMUNICATION_HUMANCONTROL_H_
#define PCCOMMUNICATION_HUMANCONTROL_H_

void PC_USCI_A0_UART_string_transmitData(uint8_t *str);
void PC_USCI_A0_UART_alldata_transmitData(uint8_t *str, int size);
int createUartPCTask(void);
int rountinePCUart(msp430peri_t* newmessage);

#endif /* PCCOMMUNICATION_HUMANCONTROL_H_ */
