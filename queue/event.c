/*
 * event.c
 *
 *  Created on: 2016年7月14日
 *      Author: Lpw
 */
#include "../interconfig.h"
#include <stdio.h>
#include <string.h>
#include "event.h"
static void mystringncpy(uint8_t* dec, uint8_t *src, int size);

/*空 queuehead = queuetail;
 * 满 queuehead = queuetail -> next*/
msp430peri_t onqueue[MAXEVENT];
msp430peri_t* queuehead;
msp430peri_t* queuetail;
msp430peri_t* prehead;
void initQueue(){
	int i = 0;
	queuehead = onqueue;
	queuetail = onqueue;

	while(i < MAXEVENT){
		onqueue[i].id = IDNULL;
		onqueue[i].emerge = EMERGENOUSE;
		onqueue[i].state = STATEDAID;
		memset(onqueue[i].string ,0, STRINGLENGTH);
		onqueue[i].next = &onqueue[((i+1)%MAXEVENT)];
		i++;
	}
	prehead = &onqueue[MAXEVENT - 1];
}

/*queuehead为实体,取头*/
msp430peri_t* getHandler(msp430peri_t* hook){
	NOTIMERINTERUPT;
	hook = queuehead;
	if(queuehead == queuetail)
		return (msp430peri_t*)0;
	queuehead->state = STATEBUSY;
	prehead = queuehead;
	queuehead = queuehead->next;
	TIMERINTERUPT;
	return hook;
}

/*queuetail指空，插尾*/
int insertHander(msp430peri_t node){
	NOTIMERINTERUPT;
	if((queuetail->next) == queuehead)
		return 0;
	queuetail->id = node.id;
	queuetail->emerge = node.emerge;
	queuetail->state =node.state;
	mystringncpy(queuetail->string,node.string,STRINGLENGTH);
	queuetail =queuetail->next;
	TIMERINTERUPT;
	return 1;
}

/*不是当前的node，而是传入前一个节点*/
void deleteHander(msp430peri_t *deleteAhead){
	msp430peri_t *deletenode = deleteAhead->next;
	deleteAhead->next = deletenode->next;
	deletenode->next = queuetail->next;
	queuetail->next = deletenode;
	deletenode->emerge=EMERGENOUSE;
	deletenode->id=IDNULL;
	memset(deletenode->string,0,STRINGLENGTH);
}

/*中断调整队列*/
void clearQueue(void){

	msp430peri_t* rememberhead;
	msp430peri_t* currentnode;
	msp430peri_t* changenode;
	msp430peri_t* aheadnode;

	if(queuehead == queuetail)
		return;
	if(queuehead->state==STATEDAID){
		queuehead = queuehead->next;
	}
	rememberhead = queuehead;
	while((rememberhead->next) != queuetail){
		if((rememberhead->next->state) == STATEDAID){
			deleteHander(rememberhead);//删除部分元素。
		}
		rememberhead = rememberhead->next;
	}

	/*插入排序算法*/
	rememberhead = queuehead;//内层循环记忆体
	currentnode = queuehead;//外层循环记忆体
	aheadnode = queuehead;//保证队列不断掉
	while(currentnode != queuetail){
		while((rememberhead->next) != queuetail){//没有到达队尾
			if((rememberhead->next->emerge) > (currentnode->emerge)){
				changenode = rememberhead->next;//需要提前元素
				rememberhead->next = changenode->next;//连队列空出选中节点。
				changenode->next = currentnode;//插入前一个节点
				if(currentnode == queuehead){//前一个节点特殊性。
					queuehead = changenode;//指向head的指针死了
					prehead->next=queuehead;
				}else
				{
					aheadnode->next = changenode;//
				}
				currentnode = changenode;//改变最大点
				changenode = NULL;
			}
			rememberhead = rememberhead->next;
		}
		aheadnode = currentnode;
		currentnode = currentnode->next;
		rememberhead = currentnode;
	}
}


static void mystringncpy(uint8_t* dec, uint8_t *src, int size){
	while(size--){
		*dec = *src;
		dec++;
		src++;
	}
}
