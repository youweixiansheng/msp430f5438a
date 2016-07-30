/*
 * event.c
 *
 *  Created on: 2016��7��14��
 *      Author: Lpw
 */
#include "../interconfig.h"
#include <stdio.h>
#include <string.h>
#include "event.h"
static void mystringncpy(uint8_t* dec, uint8_t *src, int size);

/*�� queuehead = queuetail;
 * �� queuehead = queuetail -> next*/
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

/*queueheadΪʵ��,ȡͷ*/
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

/*queuetailָ�գ���β*/
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

/*���ǵ�ǰ��node�����Ǵ���ǰһ���ڵ�*/
void deleteHander(msp430peri_t *deleteAhead){
	msp430peri_t *deletenode = deleteAhead->next;
	deleteAhead->next = deletenode->next;
	deletenode->next = queuetail->next;
	queuetail->next = deletenode;
	deletenode->emerge=EMERGENOUSE;
	deletenode->id=IDNULL;
	memset(deletenode->string,0,STRINGLENGTH);
}

/*�жϵ�������*/
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
			deleteHander(rememberhead);//ɾ������Ԫ�ء�
		}
		rememberhead = rememberhead->next;
	}

	/*���������㷨*/
	rememberhead = queuehead;//�ڲ�ѭ��������
	currentnode = queuehead;//���ѭ��������
	aheadnode = queuehead;//��֤���в��ϵ�
	while(currentnode != queuetail){
		while((rememberhead->next) != queuetail){//û�е����β
			if((rememberhead->next->emerge) > (currentnode->emerge)){
				changenode = rememberhead->next;//��Ҫ��ǰԪ��
				rememberhead->next = changenode->next;//�����пճ�ѡ�нڵ㡣
				changenode->next = currentnode;//����ǰһ���ڵ�
				if(currentnode == queuehead){//ǰһ���ڵ������ԡ�
					queuehead = changenode;//ָ��head��ָ������
					prehead->next=queuehead;
				}else
				{
					aheadnode->next = changenode;//
				}
				currentnode = changenode;//�ı�����
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
