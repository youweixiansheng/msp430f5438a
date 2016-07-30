/*
 * spi.c
 *
 *  Created on: 2016Äê7ÔÂ6ÈÕ
 *      Author: Lpw
 */
#include "../interconfig.h"
#include "driverlib.h"
#include "usci_b0_spi.h"

static uint8_t returnValue = 0x00;
static uint8_t receiveData = 0x00;
uint16_t ans;
uint16_t real_vol;
#if USCI_B0_ADS1118_SPI
int usci_b0_ads1118_spi_init(void){
//Set P1.1 for slave reset
	GPIO_setAsOutputPin(
	        GPIO_PORT_P3,
			GPIO_PIN0
			);
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P3,
        GPIO_PIN0
        );
    //P3.5,4,0 option select
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3
        );
    //P3.1 SIMO

    //Initialize Master
    USCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = UCS_getSMCLK();
    param.desiredSpiClock = SPI0CLK;
    param.msbFirst = USCI_B_SPI_MSB_FIRST;
    param.clockPhase = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    returnValue = USCI_B_SPI_initMaster(USCI_B0_BASE, &param);

    if(STATUS_FAIL == returnValue)
    {
        return 0;
    }
    //Enable SPI module
    USCI_B_SPI_enable(USCI_B0_BASE);
    //Enable Receive interrupt

    __delay_cycles(100);
    return 1;
}
#else
int ads1118_spi_init(void){return 1};
#endif


//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCI_B0_ISR(void)
{
	NOTIMERINTERUPT;
    switch(__even_in_range(UCB0IV,4))
    {
    case 2:
        receiveData = USCI_B_SPI_receiveData(USCI_B0_BASE);
        switch(receiveData){
        	case 0x52:real_vol = ans;break;
        	case 0xEA:break;
        	default: ans <<= 8; ans |= receiveData;
        }
        break;
    default: break;
    }
    TIMERINTERUPT;
}

