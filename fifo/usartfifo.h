/*
 * usartfifo.h
 *
 *  Created on: 2016Äê7ÔÂ4ÈÕ
 *      Author: Lpw
 */

#ifndef USARTFIFO_H_
#define USARTFIFO_H_
#include "stdint.h"

#define MAX_BUFF 200

int init_fifo(uint8_t *head_buff);
int get_a_char_fifo(uint8_t *dec);
int insert_a_char_fifo(uint8_t src);

#endif /* USARTFIFO_H_ */
