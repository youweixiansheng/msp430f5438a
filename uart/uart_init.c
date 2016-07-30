/*
 * uart_init.c
 *
 *  Created on: 2016年7月4日
 *      Author: Lpw
 */
#include "../interconfig.h"
#include "driverlib.h"


#if UARTSCREENENABLE
/*P5.6 P5.7 串口屏*/
int UartScreenInit(void){
	GPIO_setAsPeripheralModuleFunctionInputPin(
		GPIO_PORT_P5,
		GPIO_PIN6 + GPIO_PIN7
		);

	//Initialize UART module in auto baudrate detection multiprocessor mode
	//Baudrate = 9600, clock freq = 1.048MHz
	//UCBRx = 6, UCBRFx = 13, UCBRSx = 0, UCOS16 = 1
	USCI_A_UART_initParam param = {0};
	param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
	param.clockPrescalar = 6;
	param.firstModReg = 13;
	param.secondModReg = 0;
	param.parity = USCI_A_UART_NO_PARITY;
	param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
	param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
	param.uartMode = USCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE;
	param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
	if(STATUS_FAIL == USCI_A_UART_init(USCI_A1_BASE, &param))
	{
		return 0;
	}

	//Enable UART module for operation
	USCI_A_UART_enable(USCI_A1_BASE);

	//Put UART to sleep/dormant mode toreceive break/sync first
//	USCI_A_UART_setDormant(USCI_A1_BASE);
    USCI_A_UART_transmitBreak(USCI_A1_BASE);

	//Enable Receive Interrupt
	USCI_A_UART_clearInterrupt(USCI_A1_BASE,
							   USCI_A_UART_RECEIVE_INTERRUPT);
	USCI_A_UART_enableInterrupt(USCI_A1_BASE,
								USCI_A_UART_RECEIVE_INTERRUPT);
	return 1;
}
#else
	int UartScreenInit(void){return 1;};
#endif

#if UARTPCENABLE
/*P3.4 P3.5 上位机通信*/
int UartPCInit(void){

	GPIO_setAsPeripheralModuleFunctionInputPin(
		GPIO_PORT_P3,
		GPIO_PIN4 + GPIO_PIN5
		);

	//Initialize UART module in auto baudrate detection multiprocessor mode
	//Baudrate = 9600, clock freq = 1.048MHz
	//UCBRx = 6, UCBRFx = 13, UCBRSx = 0, UCOS16 = 1
	USCI_A_UART_initParam param = {0};
	param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
	param.clockPrescalar = 6;
	param.firstModReg = 13;
	param.secondModReg = 0;
	param.parity = USCI_A_UART_NO_PARITY;
	param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
	param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
	param.uartMode = USCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE;
	param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
	if(STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param))
	{
		return 0;
	}

	//Enable UART module for operation
	USCI_A_UART_enable(USCI_A0_BASE);

	//Put UART to sleep/dormant mode toreceive break/sync first
//	USCI_A_UART_setDormant(USCI_A0_BASE);
//    Transmit Break
    USCI_A_UART_transmitBreak(USCI_A0_BASE);

//	Wait till ,odule is ready to transmit again
	//Enable Receive Interrupt
    //不使能
//	USCI_A_UART_clearInterrupt(USCI_A0_BASE,
//							   USCI_A_UART_RECEIVE_INTERRUPT);
//	USCI_A_UART_enableInterrupt(USCI_A0_BASE,
//								USCI_A_UART_RECEIVE_INTERRUPT);
	return 1;
}
#else
void UartPCInit(void){return 1;};
#endif


void uartStrncpy(uint8_t *src,uint8_t *dec,int size){
	while(size--){
		*src = *dec;
		src++;
		dec++;
	}
}
