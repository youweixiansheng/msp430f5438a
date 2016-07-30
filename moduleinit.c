/*
 * moduleinit.c
 *
 *  Created on: 2016Äê7ÔÂ14ÈÕ
 *      Author: Lpw
 */
unsigned long int idlenum = 0;
#include "peripherconfig.h"
#include "moduleinit.h"
#include "increase.h"
#include "PCCommunication/humancontrol.h"
#include "ScreenCmunication/read_screen.h"
#include "fpgaComunication/phaseadjust.h"
#include <string.h>
#include "button/button.h"
#include "ads1118/ads1118.h"
#include "dac8811/dac8811Config.h"
#include "dac8571/msp430f5438_DAC8571_drive.h"
#include "mcode/gene.h"

int peripherModuleInit(void){
	int initstate;

	initstate = createUartPCTask();
	if(0==initstate)
		return 0;

	initstate = createUartScreenTask();
	if(0==initstate)
		return 0;
	initstate = produceHandler();
	if(0==initstate)
		return 0;
	initstate = createFpgaSpiTask();
	if(0==initstate)
		return 0;
	inittrigger();
//	initstate = createAds1118Task();
//	if(0==initstate)
//		return 0;
//	configDac8811();
//	DAC8571_Init();
//	initButton();
	return 1;
}

int rountineIdle(msp430peri_t *message){
	int insertstate;
	msp430peri_t idletask;
	idletask.id=IDIDLE;
	idletask.emerge=EMERGELOW;
	idletask.state=STATEONWORK;
	memset(idletask.string,0,STRINGLENGTH);
	sprintf((char*)(idletask.string),"%ld",idlenum++);
	insertstate = insertHander(idletask);
	message->state=STATEDAID;
	message->emerge=EMERGENOUSE;
	message->id=IDNULL;
	memset(message->string,0,STRINGLENGTH);
	return insertstate;
}
