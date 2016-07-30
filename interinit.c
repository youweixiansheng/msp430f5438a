/*
 * init.c
 *
 *  Created on: 2016Äê7ÔÂ14ÈÕ
 *      Author: Lpw
 */
#include "interinit.h"
#include "interconfig.h"
#include "queue/event.h"
#include "fifo/usartfifo.h"
#include "timer/timer.h"
#include "spi/usci_b2_spi.h"
#include "spi/usci_b0_spi.h"
#include "spi/usci_b1_spi.h"

int initInterface(void){
	uint8_t *screenqueue = 0;

	init_fifo(screenqueue);
	initQueue();
	if(0 == UartScreenInit())
		return 0;
	if(0 == UartPCInit())
		return 0;
	if(0 == usci_b2_fpga_spi_init())
		return 0;
//	if(0 == usci_b0_ads1118_spi_init())
//		return 0;
//	if(0 == usci_b1_dac8811_spi_init())
//		return 0;
	initTimerRountine();
	return 1;
}

