/*
 * button.c
 *
 *  Created on: 2016Äê7ÔÂ19ÈÕ
 *      Author: Lpw
 */

#include "../peripherconfig.h"
#include "button.h"

void initButton(void){
	GPIO_setAsOutputPin(GPIO_PORT_P6,
			GPIO_PIN0 + GPIO_PIN1);
}

void changeState(uint8_t* push){
	switch(*push){
		case 1:GPIO_setOutputLowOnPin(GPIO_PORT_P6,
				GPIO_PIN0 + GPIO_PIN1);break;
		case 2:GPIO_setOutputHighOnPin(GPIO_PORT_P6,
				GPIO_PIN0);
			   GPIO_setOutputLowOnPin(GPIO_PORT_P6,
						GPIO_PIN1);break;
		case 3:GPIO_setOutputHighOnPin(GPIO_PORT_P6,
				GPIO_PIN1);
			   GPIO_setOutputLowOnPin(GPIO_PORT_P6,
						GPIO_PIN0);break;
		case 4:GPIO_setOutputHighOnPin(GPIO_PORT_P6,
				GPIO_PIN0 + GPIO_PIN1);break;
	}
}
