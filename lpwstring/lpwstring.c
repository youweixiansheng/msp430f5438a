/*
 * lpwstring.c
 *
 *  Created on: 2016Äê7ÔÂ21ÈÕ
 *      Author: Lpw
 */

#include "../peripherconfig.h"
#include "lpwstring.h"

void uint8Strncpy(uint8_t *str, uint8_t *string,int num){
	while(num--){
		*str++ = *string++;
	}
}

