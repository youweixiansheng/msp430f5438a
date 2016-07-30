/*
 * increase.c
 *
 *  Created on: 2016Äê7ÔÂ14ÈÕ
 *      Author: Lpw
 */
#include "interconfig.h"
#include "queue/event.h"
#include "increase.h"
#include <stdio.h>
#include <string.h>

int produceHandler(void){
	msp430peri_t ideltask;
	int insertstate;
	ideltask.id=IDIDLE;
	ideltask.state=STATEONWORK;
	ideltask.emerge=EMERGELOW;
	memset(ideltask.string,0,STRINGLENGTH);
	insertstate = insertHander(ideltask);

	return insertstate;
}

