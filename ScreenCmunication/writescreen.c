/*
 * ctrlusart.c
 *
 *  Created on: 2016年7月4日
 *      Author: Lpw
 */
#include "../peripherconfig.h"
#include "writescreen.h"
#include "string.h"
#include "../fifo/usartfifo.h"
#include "../PCCommunication/humancontrol.h"

uint8_t A1receivedData = 0x00;
/*全局中断*/
#if SCREENUART
/*普通船体基础上上加有结束符*/
void USCI_A1_UART_screen_string_transmitData(uint8_t *str){
	int8_t i = 3;

	do{
		while(!USCI_A_UART_getInterruptStatus(USCI_A1_BASE,
				                                              USCI_A_UART_TRANSMIT_INTERRUPT_FLAG))
		{
			;
		}
		USCI_A_UART_transmitData(USCI_A1_BASE,*str);
	}while(*(++str));

	while(i--){
		while(!USCI_A_UART_getInterruptStatus(USCI_A1_BASE,
		                                              USCI_A_UART_TRANSMIT_INTERRUPT_FLAG))
		{
			;
		}
		USCI_A_UART_transmitData(USCI_A1_BASE,0xff);
	};
}
#else
void USCI_A1_UART_screen_string_transmitData(uint8_t *str){};
#endif


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A1_VECTOR)))
#endif
void USCI_A1_ISR(void)
{
	Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
	DISABLEUSCIB0ADS1118;
	DISABLEUSCIB2FPGA;
	switch(__even_in_range(UCA1IV,4))
    {
    case 2:
        //Receive the data
        A1receivedData = USCI_A_UART_receiveData(USCI_A1_BASE);
        if(0 == insert_a_char_fifo(A1receivedData)){
        	PC_USCI_A0_UART_string_transmitData("full error\r\n");
        }
        break;
    default: break;
    }
	ENABLEUSCIB0ADS1118;
	ENABLEUSCIB2FPGA;
	Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
