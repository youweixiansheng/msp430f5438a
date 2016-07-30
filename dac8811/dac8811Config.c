/*
 * dac8811Config.c
 *
 *  Created on: 2016年7月20日
 *      Author: Lpw
 */
#include "../peripherconfig.h"
#include "dac8811Config.h"
#include "stdlib.h"
#include "string.h"
static void uscib1SpiSendAByte(uint8_t data);
static void writeSetting(uint16_t transset);

static void uscib1SpiSendAByte(uint8_t data){
	while(!USCI_B_SPI_getInterruptStatus(USCI_B1_BASE,
										 USCI_B_SPI_TRANSMIT_INTERRUPT))
	{
		;
	}
	//Transmit Data to slave
	USCI_B_SPI_transmitData(USCI_B1_BASE,data);
}

/*高精度版，可以加static成高速版*/
void dacOutputVol(float vol, char unit){
//	float refvol = 2.49883;//基准电压
	uint16_t data;
	if(unit == 'm'){
		data = vol*26.226273;
	}else
	{
		data = vol*26226.273896;
	}
	writeSetting(data);
}

static void writeSetting(uint16_t transset){
	GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);

	uscib1SpiSendAByte((uint8_t)(transset>>8));
	uscib1SpiSendAByte((uint8_t)transset);
	GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);
}

/*初始化dac8811*/
void configDac8811(void){
	GPIO_setAsOutputPin(
	    	        GPIO_PORT_P3,GPIO_PIN6);
}

int rountineDac8811(msp430peri_t *message){
	float trans;
	char* place;
	trans = atof((char*)(message->string));
	place = strchr((char*)(message->string),'v');
	if(*(place-1) == 'm')
		dacOutputVol(trans,'m');
	else
		dacOutputVol(trans,'v');
	message->id = IDNULL;
	message->emerge=EMERGENOUSE;
	message->state= STATEDAID;
	memset(message->string,0,STRINGLENGTH);
	return 1;
}

int insertDacTask(float vol, uint8_t unit){
	msp430peri_t dacnode;
	int insertstation;

	dacnode.id = IDDAC8811;
	dacnode.emerge=EMERGELOW;
	dacnode.state=STATEONWORK;
	if(unit == 'm'){
		sprintf((char*)(dacnode.string),"%fmv",vol);
	}else
	{
		sprintf((char*)(dacnode.string),"%fv",vol);
	}
	insertstation = insertHander(dacnode);
	return insertstation;
}
