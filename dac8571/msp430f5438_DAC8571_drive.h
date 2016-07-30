#ifndef __msp430f5438_DAC8571_drive_H_
#define __msp430f5438_DAC8571_drive_H_

#define SDA_IN         GPIO_setAsInputPin(GPIO_PORT_P1,GPIO_PIN5)
#define SDA_OUT        GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN5)

#define SDA_L          GPIO_setOutputLowOnPin( GPIO_PORT_P1,GPIO_PIN5)
#define SDA_H          GPIO_setOutputHighOnPin( GPIO_PORT_P1,GPIO_PIN5)
#define SDA_Value      GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN5)

#define SCL_OUT        GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN6)

#define SCL_L          GPIO_setOutputLowOnPin( GPIO_PORT_P1,GPIO_PIN6)
#define SCL_H          GPIO_setOutputHighOnPin( GPIO_PORT_P1,GPIO_PIN6)

void DAC8571_Init();
void I2C_START ();
void I2C_STOP ();
void writebyte(uint8_t date);
uint8_t readbyte();
void DAC8571_Write_DATA(uint16_t DATA);
void DAC8571_DA_Conversion(float Vout);
int insertDac8571Task(float vol);
int rountineDac8571(msp430peri_t *message);
#endif
