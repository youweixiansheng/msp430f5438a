/*
 * read_screen.c
 *
 *  Created on: 2016年7月5日
 *      Author: Lpw
 */
#include "../peripherconfig.h"
#include "../fifo/usartfifo.h"
#include <stdint.h>
#include <stdlib.h>
#include "read_screen.h"
#include <string.h>
#include "writescreen.h"
#include "../fpgaComunication/phaseadjust.h"
#include "../button/button.h"
#include "screenid.h"
#include "../mcode/gene.h"
static int screenRuning(uint8_t *str, int size,msp430peri_t screenhandler);



/*getmessage 不完整， 结构体有问题*/
text_t get_message(uint8_t* srcmessage, int size){
	text_t result;

	if(*srcmessage == 0x70){
		srcmessage++;
		result.id = 0x01;
		memset(result.str,0,20);
		result.strlengh = size - 1;
		screen_strncpy(result.str,srcmessage,size - 1);
	}
	else if(*srcmessage == 0x71){
		srcmessage++;
		result.id = 0x02;
		memset(result.str,0,20);
		result.strlengh = size - 1;
		screen_strncpy(result.str,srcmessage,size - 1);
	}
	else{
		result.id = 0xff;
	}

	return result;
}


/*读取一个字符串，非阻塞式. 返回src，和string的大小size*/
int USCI_A1_URAT_read_screen_string(uint8_t *str , int* size){
	static uint8_t string[30] = {0};
	static uint8_t i = 0;
	uint8_t message = 0;
	static uint8_t over = 0;//奇怪。

	while(over < 3){
		if(get_a_char_fifo(&message) == 0){
			return 0;
		}else
		{
			if(message == 0xff){
				over++;
				continue;
			}
			string[i] = message;
			i++;
		}
	}

	*size = i;
	screen_strncpy(str,string,*size);
	i = 0;
	over = 0;
	memset(string,0,30);
	return 1;
}

/*强迫症专用*/
void screen_strncpy(uint8_t *str, uint8_t *string,int num){
	while(num--){
		*str++ = *string++;
	}
}


int createUartScreenTask(void){
	int createstate;

	msp430peri_t printuartscreen;
	printuartscreen.id=IDUARTSCREEN;
	printuartscreen.emerge=EMERGELOW;
	uartStrncpy(printuartscreen.string,"uartscreen idle",10);
	printuartscreen.state=STATEONWORK;
	createstate = insertHander(printuartscreen);
	return createstate;
}


int rountineUartScreen(msp430peri_t* newmessage){
	int dealrountinestate = 1;
	int idlerountinestate = 1;
	uint8_t str[30];
	uint8_t scan;
	int size;
	msp430peri_t screenhandler;
	msp430peri_t screenrountinehandler;

	scan = USCI_A1_URAT_read_screen_string(str,&size);
	if(0 != scan){
		screenhandler.id=IDPCUART;
		screenhandler.state=STATEONWORK;
		screenhandler.emerge=EMERGEHIGH;
		dealrountinestate = screenRuning(str,size,screenhandler);
		screenrountinehandler.emerge=EMERGELOW;
	}else{
		screenrountinehandler.emerge=EMERGEHIGH;
	}
	screenrountinehandler.id=IDUARTSCREEN;
	screenrountinehandler.state=STATEONWORK;
	memset(screenrountinehandler.string,0,STRINGLENGTH);
	idlerountinestate = insertHander(screenrountinehandler);
	newmessage->state = STATEDAID;
	newmessage->id=IDNULL;
	newmessage->emerge= EMERGENOUSE;
	return (idlerountinestate==dealrountinestate)&&1;
}


static int screenRuning(uint8_t *str, int size,msp430peri_t screenhandler){
	text_t screenbuff;
	uint8_t screendatafill[40];
	uint8_t *strhex_p;
	int insertstate;
	screenbuff = screenMessageTrans(str,size);
	memset(screendatafill,0,40);

	switch(screenbuff.id){
		case 0x01:	uartStrncpy(screenhandler.string,"usartScreen:trigger",19);
					strhex_p = screenhandler.string + 19;
					trigger();
					break;//t6.txt第一个窗口。
		case 0x02:	uartStrncpy(screenhandler.string,"usartScreen:t7.txt=",19);
					strhex_p = screenhandler.string + 19;
					break;//t7.txt第二个窗口。
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:	changeState(screenbuff.str);
					uartStrncpy(screenhandler.string,"usartScreen:button ",19);
					strhex_p = screenhandler.string + 19;break;
		case 0x07:	uartStrncpy(screenhandler.string,"usartScreen:slider ",19);
					strhex_p = screenhandler.string + 19;
					while(0 == spiFpgaMessageFill(&screenbuff));//fpga调试
					break;
		case 0xff:	uartStrncpy(screenhandler.string,"usartScreen: ",13);
					strhex_p = screenhandler.string + 13;break;
		default:	uartStrncpy(screenhandler.string,"usartScreen: ",13);
					strhex_p = screenhandler.string + 13;
	}
//	if(screenbuff.id == 0x01){//处理屏幕反馈
//		uartStrncpy(screenhandler.string,"usartScreen: n0.val=",20);
//		uartStrncpy(screendatafill,"n0.val=",7);
//		uartStrncpy(screendatafill+7,screenbuff.str,screenbuff.strlengh);
//		strhex_p = screenhandler.string + 20;
//		__delay_cycles(100000);//无语的响应等待。
//		while(0 == spiFpgaMessageFill(&screenbuff));//fpga调试
//		USCI_A1_UART_screen_string_transmitData(screendatafill);
//	}
//	else if(screenbuff.id == 0x02){
//		changeState(screenbuff.str);
//		uartStrncpy(screenhandler.string,"usartScreen:button ",19);
//		strhex_p = screenhandler.string + 19;
//	}
//	else
//	{
//		uartStrncpy(screenhandler.string,"usartScreen: ",13);
//		strhex_p = screenhandler.string + 13;
//	}
	/*将串口屏数据丢到串口中去*/
	while(size--){
		sprintf((char*)strhex_p,"%02x",(uint16_t)(*str));
		str++;
		strhex_p = strhex_p + 2;
	}
	insertstate = insertHander(screenhandler);
	return insertstate;
}
