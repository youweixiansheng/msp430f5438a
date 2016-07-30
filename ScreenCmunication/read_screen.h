/*
 * read_screen.h
 *
 *  Created on: 2016Äê7ÔÂ5ÈÕ
 *      Author: Lpw
 */

#ifndef READ_SCREEN_H_
#define READ_SCREEN_H_



text_t get_message(uint8_t* srcmessage, int size);
int USCI_A1_URAT_read_screen_string(uint8_t *str , int* size);
void screen_strncpy(uint8_t *str, uint8_t *string,int num);
int createUartScreenTask(void);
int rountineUartScreen(msp430peri_t* newmessage);

#endif /* READ_SCREEN_H_ */
