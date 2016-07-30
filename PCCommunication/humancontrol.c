/*
 * humancontrol.c
 *
 *  Created on: 2016年7月14日
 *      Author: Lpw
 */

#include "../peripherconfig.h"
#include "humancontrol.h"
#include <string.h>
uint8_t A0receivedData = 0x00;

/*PC UART 可以不需要中断随时开*/

#if PCUART
void PC_USCI_A0_UART_string_transmitData(uint8_t *str){
	do{
        while(!USCI_A_UART_getInterruptStatus(USCI_A0_BASE,
                                              USCI_A_UART_TRANSMIT_INTERRUPT_FLAG))
        {
            ;
        }
		USCI_A_UART_transmitData(USCI_A0_BASE,*str);
	}while(*str++);
}


void PC_USCI_A0_UART_alldata_transmitData(uint8_t *str, int size){

	do{
        while(!USCI_A_UART_getInterruptStatus(USCI_A0_BASE,
                                              USCI_A_UART_TRANSMIT_INTERRUPT_FLAG))
        {
            ;
        }
		USCI_A_UART_transmitData(USCI_A0_BASE,*str);
		str++;
	}while(--size);
}
#else
void PC_USCI_A0_UART_string_transmitData(uint8_t *str){};
void PC_USCI_A0_UART_alldata_transmitData(uint8_t *str, int size){};
#endif


//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
	Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);

	switch(__even_in_range(UCA0IV,4))
    {
    //Vector 2 - RXIFG
    case 2:
        A0receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
        break;
    default: break;
    }

	Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

/*处理死亡*/
void printStringOnPC(msp430peri_t* occenmessage){
	switch(occenmessage->emerge){
		case EMERGELOW:PC_USCI_A0_UART_string_transmitData("low emerge :");break;
		case EMERGEHIGH:PC_USCI_A0_UART_string_transmitData("high emerge :");break;
		case EMERGEVERYHIGH:PC_USCI_A0_UART_string_transmitData("veryhigh emerge :");break;
		default:PC_USCI_A0_UART_string_transmitData("wrong emerge :");break;
	}
	PC_USCI_A0_UART_string_transmitData(occenmessage->string);
	PC_USCI_A0_UART_string_transmitData("\r\n");
	occenmessage->state=STATEDAID;
	occenmessage->emerge = EMERGENOUSE;
	occenmessage->id=IDNULL;
	memset(occenmessage->string,0,STRINGLENGTH);
}

int createUartPCTask(void){
	int createstate;

	msp430peri_t printuart;
	printuart.id=IDPCUARTIDLE;
	printuart.emerge=EMERGELOW;
	uartStrncpy(printuart.string,"uart idle",10);
	printuart.state=STATEONWORK;
	createstate = insertHander(printuart);

	return createstate;
}

int rountinePCUart(msp430peri_t* newmessage){
	int rountinestate;
	msp430peri_t pcuarthandler;
	if(newmessage->id == IDPCUARTIDLE){
		pcuarthandler.state=STATEONWORK;
		pcuarthandler.id=IDPCUARTIDLE;
		pcuarthandler.emerge=EMERGELOW;
		memset(pcuarthandler.string,0,STRINGLENGTH);
		rountinestate = insertHander(pcuarthandler);
		newmessage->state=STATEDAID;
		newmessage->id=IDNULL;
		newmessage->emerge=EMERGENOUSE;
		return rountinestate;
	}else
	{
		printStringOnPC(newmessage);
	}

	return 1;
}

