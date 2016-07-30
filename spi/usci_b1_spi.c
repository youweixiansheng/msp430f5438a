/*
 * spi.c
 *
 *  Created on: 2016Äê7ÔÂ6ÈÕ
 *      Author: Lpw
 */
#include "../interconfig.h"
#include "driverlib.h"
#include "usci_b1_spi.h"
static uint8_t receiveData = 0x00;
static uint8_t returnValue = 0x00;
#if USCI_B1_DAC8811_SPI
int usci_b1_dac8811_spi_init(void){
    //P3.5,4,0 option select
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN7
        );
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
		GPIO_PIN4+GPIO_PIN5
        );
    //Initialize Master
    USCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = UCS_getSMCLK();
    param.desiredSpiClock = SPI1CLK;
    param.msbFirst = USCI_B_SPI_MSB_FIRST;
    param.clockPhase = USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    param.clockPolarity = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    returnValue = USCI_B_SPI_initMaster(USCI_B1_BASE, &param);

    if(STATUS_FAIL == returnValue)
    {
        return 0;
    }

    //Enable SPI module
    USCI_B_SPI_enable(USCI_B1_BASE);

    __delay_cycles(100);
    return 1;
}
#else
int usci_b1_dac8811_spi_init(void){return 1};
#endif

//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B1_VECTOR)))
#endif
void USCI_B1_ISR(void)
{
	NOTIMERINTERUPT;
    switch(__even_in_range(UCB1IV,4))
    {
    case 2:
        receiveData = USCI_B_SPI_receiveData(USCI_B1_BASE);
        break;
    default: break;
    }
    TIMERINTERUPT;
}
