/*
 * screenid.c
 *
 *  Created on: 2016Äê7ÔÂ22ÈÕ
 *      Author: Lpw
 */
#include "../interconfig.h"
#include "screenid.h"
#include "../peripherconfig.h"
#include "../lpwstring/lpwstring.h"
#include <string.h>

text_t screenMessageTrans(uint8_t* srcmessage, int size){
	text_t screenData;

	memset(&screenData,0,sizeof(screenData));

	switch(*srcmessage){
		case 0x01:screenData.id=0x01; srcmessage++; break;//txt
		case 0x02:screenData.id=0x02; srcmessage++; break;//txt
		case 0x03:screenData.id=0x03; srcmessage++; break;//button
		case 0x04:screenData.id=0x04; srcmessage++; break;
		case 0x05:screenData.id=0x05; srcmessage++; break;
		case 0x06:screenData.id=0x06; srcmessage++; break;//button
		case 0x07:screenData.id=0x07; srcmessage++;	break;//»¬¿é;
		default:screenData.id=0xff;
	}

	switch(*srcmessage){
		case 0x70:
		case 0x71:	srcmessage++;
					screenData.strlengh = size - 2;
					uint8Strncpy(screenData.str,srcmessage,size -2);
					break;
		default:	uint8Strncpy(screenData.str,srcmessage,size);
	}
	return screenData;
}
