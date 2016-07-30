/*
 * dac8811Config.h
 *
 *  Created on: 2016Äê7ÔÂ20ÈÕ
 *      Author: Lpw
 */

#ifndef DAC8811_DAC8811CONFIG_H_
#define DAC8811_DAC8811CONFIG_H_

void dacOutputVol(float vol, char unit);
void configDac8811(void);
int insertDacTask(float vol, uint8_t unit);
int rountineDac8811(msp430peri_t *message);
#endif /* DAC8811_DAC8811CONFIG_H_ */
