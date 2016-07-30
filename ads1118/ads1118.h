/*
 * ads1118.h
 *
 *  Created on: 2016年7月7日
 *      Author: Lpw
 */

#ifndef ADS1118_H_
#define ADS1118_H_


#define CONTROLBIT 0x52EB//0b1100 C(0) 000 大小 0 连续转换模式 111 高速 0 ad转换 1 上拉使能  01 有效数据 1 保留

int transDataStore(msp430peri_t *sample);
int ads1118Init(void);
int createAds1118Task(void);
int rountineAds1118(msp430peri_t *message);

//uint16_t modeChange[8] = {
//		0x40EB,//6.144
//		0x42EB,//4.906
//		0x44EB,//2.048
//		0x46EB,//1.024
//		0x48EB,//0.512
//		0x4aEB,//0.256
//}
#endif /* ADS1118_H_ */
