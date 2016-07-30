/*
 * event.h
 *
 *  Created on: 2016Äê7ÔÂ14ÈÕ
 *      Author: Lpw
 */

#ifndef QUEUE_EVENT_H_
#define QUEUE_EVENT_H_
#define MAXEVENT 20

void initQueue();
msp430peri_t* getHandler(msp430peri_t* hook);
int insertHander(msp430peri_t node);
void deleteHander(msp430peri_t *deleteAhead);
void clearQueue(void);

#endif /* QUEUE_EVENT_H_ */
