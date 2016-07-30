/*
 * spi.c
 *
 *  Created on: 2016Äê7ÔÂ6ÈÕ
 *      Author: Lpw
 */
#include "../interconfig.h"
#include "driverlib.h"
#include "usci_b3_spi.h"

static uint8_t returnValue = 0x00;
static uint8_t receiveData = 0x00;

#if USCI_B3_RESERVE_SPI
int usci_b3_reserve_spi_init(void){

	GPIO_setAsOutputPin(
	        GPIO_PORT_PA,
			GPIO_PIN0
			);
    GPIO_setOutputHighOnPin(
        GPIO_PORT_PA,
        GPIO_PIN0
        );
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PA,
		GPIO_PIN1+GPIO_PIN2+GPIO_PIN3
        );
    //Initialize Master
    USCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = UCS_getSMCLK();
    param.desiredSpiClock = SPI3CLK;
    param.msbFirst = USCI_B_SPI_MSB_FIRST;
    param.clockPhase = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    returnValue = USCI_B_SPI_initMaster(USCI_B3_BASE, &param);

    if(STATUS_FAIL == returnValue)
    {
        return 0;
    }

    //Enable SPI module
    USCI_B_SPI_enable(USCI_B3_BASE);

    __delay_cycles(100);
    return 1;
}
#else
int usci_b3_reserve_spi_init(void){return 1};
#endif


//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B3_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B3_VECTOR)))
#endif
void USCI_B3_ISR(void)
{
    switch(__even_in_range(UCB3IV,4))
    {
    case 2:
        receiveData = USCI_B_SPI_receiveData(USCI_B3_BASE);
        break;
    default: break;
    }
}
