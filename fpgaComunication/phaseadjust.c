/*
 * phaseadjust.c
 *
 *  Created on: 2016年7月19日
 *      Author: Lpw
 */
#include "../peripherconfig.h"
#include "phaseadjust.h"
#include "../spi/usci_b2_spi.h"
#include "../ScreenCmunication/writescreen.h"
#include <stdlib.h>
#include <string.h>
uint32_t transPhase;

static void master_write_a_byte(uint8_t data);
static uint32_t phaseSet(uint32_t *countnum);
static uint32_t masterWriteMessage(uint32_t phasenum);
static void lpwstrncpy(uint8_t *str, uint8_t *string,int num);
static uint32_t masterWriteMessage(uint32_t phasenum){

	transPhase = 0;
//	GPIO_setOutputLowOnPin(GPIO_PORT_P9,GPIO_PIN0);
//  GPIO_setOutputHighOnPin(GPIO_PORT_P9,GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P9,GPIO_PIN0);
    USCI_B_SPI_clearInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_enableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

    master_write_a_byte((uint8_t)(phasenum>>24));
    master_write_a_byte((uint8_t)(phasenum>>16));
    master_write_a_byte((uint8_t)(phasenum>>8));
    master_write_a_byte((uint8_t)phasenum);
//    __delay_cycles(10000);
//    USCI_B_SPI_disableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
    GPIO_setOutputHighOnPin(GPIO_PORT_P9,GPIO_PIN0);

    return transPhase;
}

/*调整简单相位关系*/
static uint32_t phaseSet(uint32_t *countnum){
//	uint32_t ornum;//鉴相数据
//	uint32_t get;
	int32_t maxnum;
	msp430peri_t usart;

	char dispalyfre[40];
	char us[20] = {0};
	maxnum = masterWriteMessage(*countnum);
	memset(dispalyfre,0,40);
	memset(us,0,20);
	memset(&usart,0,sizeof(usart));
	sprintf(dispalyfre,"t6.txt=\"%ld\"",maxnum);
	usart.id=IDPCUART;
	usart.emerge=EMERGELOW;
	usart.state=STATEONWORK;
	sprintf(us,"%ld\r\n",maxnum - 1);
	uartStrncpy(usart.string,us,STRINGLENGTH);
	insertHander(usart);
	USCI_A1_UART_screen_string_transmitData(dispalyfre);
//	ornum = masterWriteMessage(*countnum);
	/*测试屏蔽*/
//	if(maxnum & 0x80000000){
//		get = maxnum - 0x80000000;
//		maxnum =  ornum;
//		ornum = get;
//	}else
//	{
//		ornum = ornum - 0x80000000;
//	}

//	if(ornum < *countnum){
//		(*countnum)--;
//		masterWriteMessage(*countnum);
//	}else if(ornum > *countnum)
//	{
//		(*countnum)++;
//		masterWriteMessage(*countnum);
//	}
	return maxnum;
}


static void master_write_a_byte(uint8_t data){
	while(!USCI_B_SPI_getInterruptStatus(USCI_B2_BASE,
										 USCI_B_SPI_TRANSMIT_INTERRUPT))
	{
		;
	}
	//Transmit Data to slave
	USCI_B_SPI_transmitData(USCI_B2_BASE,data);
}

int rountineFpga(msp430peri_t *message){
	static uint32_t countnum = 200;
	static uint32_t maxnum = 50000;
	double accur;
	msp430peri_t fpgaAdjust;
	static int degree;
	int rountinestate = 1;

	if(message->emerge == EMERGEHIGH){
		degree = atoi(message->string);
		message->id = IDNULL;
		message->emerge = EMERGENOUSE;
		message->state = STATEDAID;
		memset(message->string,0,STRINGLENGTH);
	}else{
		fpgaAdjust.id = IDFPGASPI;
		fpgaAdjust.emerge = EMERGELOW;
		fpgaAdjust.state = STATEONWORK;
		memset(fpgaAdjust.string,0,STRINGLENGTH);
		rountinestate = insertHander(fpgaAdjust);
	}
	maxnum = phaseSet(&countnum);
	return rountinestate;
}

int spiFpgaMessageFill(text_t *context){
	int rountinestate = 1;
	msp430peri_t srcspi;
	srcspi.id = IDFPGASPI;
	srcspi.emerge = EMERGEHIGH;
	srcspi.state = STATEONWORK;
	memset(srcspi.string,0,STRINGLENGTH);
	lpwstrncpy(srcspi.string,context->str,context->strlengh);
	rountinestate = insertHander(srcspi);

	return rountinestate;
}

/*强迫症专用*/
static void lpwstrncpy(uint8_t *str, uint8_t *string,int num){
	while(num--){
		*str++ = *string++;
	}
}

int createFpgaSpiTask(void){
	int createstate;

	msp430peri_t fpgaspi;
	fpgaspi.id=IDFPGASPI;
	fpgaspi.emerge=EMERGELOW;
	memset(fpgaspi.string,0,STRINGLENGTH);
	fpgaspi.state=STATEONWORK;
	createstate = insertHander(fpgaspi);

	return createstate;
}
