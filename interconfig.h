/*
 * interconfig.h
 *
 *  Created on: 2016Äê7ÔÂ14ÈÕ
 *      Author: Lpw
 */

#ifndef INTERCONFIG_H_
#define INTERCONFIG_H_

#include "driverlib.h"
#define COMPARE_VALUE 5000

#define UARTPCENABLE 1
#define UARTSCREENENABLE 1
#define FIFOENABLE 1
#define USCI_B0_ADS1118_SPI 1
#define USCI_B1_DAC8811_SPI 1
#define USCI_B2_FPGA_SPI 1
#define USCI_B3_RESERVE_SPI 1


#define STRINGLENGTH 40
typedef struct msp430peri{
	int id;
	uint8_t string[STRINGLENGTH];
	uint8_t state;
	uint8_t emerge;
	struct msp430peri* next;
}msp430peri_t;

typedef struct screen_message{
	uint8_t id;
	int strlengh;
	uint8_t	str[20];
}text_t;

#define IDNULL 0
#define IDIDLE 30

#define STATEDAID 0
#define STATEONWORK 1
#define STATEBUSY 2

#define EMERGENOUSE 0
#define EMERGELOW 1
#define EMERGEHIGH 2
#define EMERGEVERYHIGH 3

#define NOTIMERINTERUPT 	Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0)
#define TIMERINTERUPT		Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0)
#include "uart/uart_init.h"
#include "queue/event.h"
#endif /* INTERCONFIG_H_ */
