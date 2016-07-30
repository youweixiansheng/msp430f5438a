/*
 * ads1118.c
 *
 *  Created on: 2016年7月7日
 *      Author: Lpw
 */
#include "../peripherconfig.h"
#include "../lpwstring/lpwstring.h"
#include "ads1118.h"
#include <stdio.h>
#include <string.h>

extern uint16_t ans;
extern uint16_t real_vol;
static uint32_t ads1118Capture();
static void ads1118_write_a_byte(uint8_t data);

/*读取一次寄存器*/
static uint32_t ads1118Capture(){
	uint32_t vol = 0;

    USCI_B_SPI_clearInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_enableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    __delay_cycles(1000);


    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0);
    ads1118_write_a_byte((uint8_t)((CONTROLBIT>>8)&0xff));
    ads1118_write_a_byte((uint8_t)(CONTROLBIT&0xff));
    vol = ans;
    ads1118_write_a_byte((uint8_t)((CONTROLBIT>>8)&0xff));
    ads1118_write_a_byte((uint8_t)(CONTROLBIT&0xff));
    __delay_cycles(100000);
    vol <<= 16;
    vol |= ans;
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN0);
    return vol;
}


static void ads1118_write_a_byte(uint8_t data){
	while(!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
										 USCI_B_SPI_TRANSMIT_INTERRUPT))
	{
		;
	}
	//Transmit Data to slave
	USCI_B_SPI_transmitData(USCI_B0_BASE,data);
}


/*初始化1118*/
int ads1118Init(void){
    GPIO_setAsOutputPin(
    	        GPIO_PORT_P3,GPIO_PIN0);
    return 1;
}

/*创建一个基本任务*/
int createAds1118Task(void){
	msp430peri_t adcinputnode;
	int insertstation;

	adcinputnode.id = IDADS1118;
	adcinputnode.emerge=EMERGELOW;
	adcinputnode.state=STATEONWORK;
	memset(adcinputnode.string,0,STRINGLENGTH);
	insertstation = insertHander(adcinputnode);
	return insertstation;
}


/*采集传感器数据，采满5个打印。*/
int rountineAds1118(msp430peri_t *message){
	msp430peri_t readAdcReg;
	msp430peri_t print;
	uint8_t full = 0;//满溢出标志。
	int createstate = 0;


	memset(&readAdcReg,0,sizeof(readAdcReg));
	full = transDataStore(message);
	if(full == 1){
		uint8Strncpy(readAdcReg.string,message->string,STRINGLENGTH);
	}else
	{
		print.id=IDPCUART;
		print.state =STATEONWORK;
		print.emerge=EMERGELOW;
		uint8Strncpy(print.string,message->string,STRINGLENGTH);
		createstate = insertHander(print);
		if(!createstate)
			return 0;
	}

	readAdcReg.id=IDADS1118;
	readAdcReg.emerge=EMERGELOW;
	readAdcReg.state=STATEONWORK;
	createstate = insertHander(readAdcReg);
	message->id = IDNULL;
	message->emerge=EMERGENOUSE;
	message->state= STATEDAID;
	memset(message->string,0,STRINGLENGTH);
	return createstate;
}

/*实时采集数据*/
int transDataStore(msp430peri_t *sample){
	uint16_t adcSrcData;
	float soildNum = 0.125;
	float adcValue = 0.0;
	uint8_t i = 0;

//	adcSrcData = 1.1;
	adcSrcData = ads1118Capture();
	//4096/0x7fff =0.125;
	//6144/0x7fff = 0.1875;
	//2048/0x7fff =0.0625;
	//1024/0x7fff = 0.03125;
	//512/0x7fff=0.015625;
	//256/0x7fff=0.0078125;
	adcValue = adcSrcData*soildNum;

	while(i < 39){
		if(*(sample->string + i) != 'a'){
			sprintf((sample->string) + i,"ad%9.5f\r\n",adcValue);
			return 1;
		}
		i += 13;
	}
	return 0;
}
