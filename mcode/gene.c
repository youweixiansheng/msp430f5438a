/*
 * gene.c
 *
 *  Created on: 2016��7��26��
 *      Author: Lpw
 */

#include "gene.h"
#include "../peripherconfig.h"

void inittrigger(void){
	GPIO_setAsOutputPin(GPIO_PORT_P6,
			GPIO_PIN0);
}

void trigger(void){
	GPIO_setOutputHighOnPin(GPIO_PORT_P6,
			GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P6,
			GPIO_PIN0);
}
