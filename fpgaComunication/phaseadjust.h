/*
 * phaseadjust.h
 *
 *  Created on: 2016Äê7ÔÂ19ÈÕ
 *      Author: Lpw
 */

#ifndef FPGACOMUNICATION_PHASEADJUST_H_
#define FPGACOMUNICATION_PHASEADJUST_H_

int rountineFpga(msp430peri_t *message);
int createFpgaSpiTask(void);
int spiFpgaMessageFill(text_t *context);
#endif /* FPGACOMUNICATION_PHASEADJUST_H_ */
