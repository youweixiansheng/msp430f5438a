#include <stdint.h>
#include "../peripherconfig.h"
#include <stdlib.h>
#include <string.h>
#include "msp430f5438_DAC8571_drive.h"

void Delay_us(int i){
	__delay_cycles(100);
}

void Delay_ms(int i){
	while(i--)
		Delay_us(1000);
}

void DAC8571_Init()
{
  SCL_OUT;
}

void I2C_START()
{ 
  SDA_OUT;
  
  SDA_H;
  SCL_H;
  Delay_us(1);
  SDA_L;
  Delay_us(1);
}

void I2C_STOP()
{ 
  SDA_OUT;
 
  SDA_L;
  Delay_us(1);
  SCL_H;
  Delay_us(1);
  SDA_H;
  Delay_us(1);
}

void writebyte(uint8_t date)
{
  uint8_t i,temp;
  temp=date;
  
  SDA_OUT;
  Delay_us(1);
  
  for(i=0;i<8;i++)
  {
   SCL_L;
   Delay_us(1);
   if(0x80&temp)
    SDA_H;
   else
    SDA_L;
   Delay_us(1);
   SCL_H;
   Delay_us(1);
   temp = temp<<1;
  }
  SCL_L;
  SDA_H;
  SCL_H;
  Delay_ms(1);
  SCL_L;
}

uint8_t readbyte()
{
  uint8_t i,temp;
  
  SDA_IN; 
  Delay_us(1);

  for(i=0;i<8;i++)
  {
    temp <<= 1;
    SCL_L;
    SCL_H;
    Delay_us(1);
    
    if(SDA_Value)
      temp |= 0x01;
  }
  SCL_L;
  
  SDA_OUT;
  
  SDA_L;
  SCL_H;
  SCL_L;
  SDA_H;
    
  return temp;
}

void DAC8571_Write_DATA(uint16_t DATA)
{
  uint8_t DATA_H,DATA_L;
  DATA_H=(DATA>>8);
  DATA_L=(DATA&0xff);
  
  I2C_START();
  Delay_us(1);
  writebyte(0x98);
  Delay_us(1);
  writebyte(0x10);
  Delay_us(1);
  writebyte(DATA_H);
  Delay_us(1);
  writebyte(DATA_L);
  Delay_us(1);
  I2C_STOP();
}

void DAC8571_DA_Conversion(float Vout)
{
  float Vref=2.048;
//   float Vref=4.096;
//     float Vref=2.5;
  uint16_t DATA;
  DATA=(uint16_t)(Vout*65536/Vref);
  DAC8571_Write_DATA(DATA);
}

int insertDac8571Task(float vol){
	msp430peri_t dacnode;
	int insertstation;

	dacnode.id = IDDAC8571;
	dacnode.emerge=EMERGELOW;
	dacnode.state=STATEONWORK;
	sprintf((char*)(dacnode.string),"%fv",vol);

	insertstation = insertHander(dacnode);
	return insertstation;
}

int rountineDac8571(msp430peri_t *message){
	float trans;

	trans = atof((char*)(message->string));
	DAC8571_DA_Conversion(trans);
	message->id = IDNULL;
	message->emerge=EMERGENOUSE;
	message->state= STATEDAID;
	memset(message->string,0,STRINGLENGTH);
	return 1;
}
