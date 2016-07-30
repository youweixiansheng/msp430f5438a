/*
 * peripherconfig.h
 *
 *  Created on: 2016年7月14日
 *      Author: Lpw
 */

#ifndef PERIPHERCONFIG_H_
#define PERIPHERCONFIG_H_
#include "interconfig.h"
#include "driverlib.h"
#include <stdio.h>

#if UARTPCENABLE
#define PCUART	1//电脑通信打印串口。
#else
#define PCUART	0//电脑通信打印串口。
#endif

#if UARTSCREENENABLE
#define SCREENUART 1//串口屏通信。
#else
#define SCREENUART 0//串口屏通信。
#endif


#if USCI_B0_ADS1118_SPI
#define ADS1118CONFIG 1
#else
#define ADS1118CONFIG 0
#endif /* PERIPHERCONFIG_H_ */

#define ENABLEUSCIB0ADS1118 USCI_B_SPI_enableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
#define DISABLEUSCIB0ADS1118 USCI_B_SPI_disableInterrupt(USCI_B0_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
#define ENABLEUSCIB2FPGA USCI_B_SPI_enableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);
#define DISABLEUSCIB2FPGA USCI_B_SPI_disableInterrupt(USCI_B2_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

#define IDADS1118 31
#define IDDAC8571 32
#define IDUARTSCREEN 33
#define IDPCUART 35
#define IDPCUARTIDLE 36
#define IDFPGASPI 37
#define IDDAC8811 38
#endif
