/*
 * usartfifo.c
 *
 *  Created on: 2016Äê7ÔÂ4ÈÕ
 *      Author: Lpw
 */
#include "../peripherconfig.h"
#include "usartfifo.h"
#include "driverlib.h"
#include <string.h>

uint8_t queue[MAX_BUFF] = {0};
static uint8_t* head;
static uint8_t* tail;

#if SCREENUART
int init_fifo(uint8_t *head_buff){
	int size = MAX_BUFF;

	head = queue;
	head_buff = head;
	tail= head + size - 1;

	while(size--){
		head_buff[size] = 0;
	}

	return 0;
}


/*tail + 1 == head ¿Õ×´Ì¬*/
int get_a_char_fifo(uint8_t *dec){
	if(tail + 1 == head){
		return 0;
	}else if(tail == (queue + MAX_BUFF - 1))
	{
		if(head == queue){
			return 0;
		}else{
			tail = queue;
			*dec = *tail;
		}
	}else
	{
		tail++;
		*dec = *tail;
	}
	return 1;
}



/*tail == head Âú×´Ì¬*/
int insert_a_char_fifo(uint8_t src){
	if(tail == head){
		return 0;
	}else if(head == (queue + MAX_BUFF - 1))
	{
		*head = src;
		head = queue;
	}else
	{
		*head = src;
		head++;
	}
	return 1;
}
#else
int init_fifo(uint8_t *head_buff){};
int get_a_char_fifo(uint8_t *dec){};
int insert_a_char_fifo(uint8_t src){};
#endif

