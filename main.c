#include "driverlib.h"

#include "interconfig.h"
#include "interinit.h"

#include "peripherconfig.h"
#include "moduleinit.h"

#include "increase.h"
#include "PCCommunication/humancontrol.h"
#include "ScreenCmunication/read_screen.h"
#include "fpgaComunication/phaseadjust.h"
#include "dac8571/msp430f5438_DAC8571_drive.h"
#include "dac8811/dac8811Config.h"
#include "ads1118/ads1118.h"

void main(void)
{
	msp430peri_t* message;
    WDT_A_hold(WDT_A_BASE);
	/*初始化内部接口*/
	while(initInterface() == 0);

	/*初始化各个模块*/
	while(peripherModuleInit() == 0);
	/*开始各个功能模块中断*/
    __bis_SR_register(GIE);


	while(1){
		/*获取一个需要处理的handler*/
		message = getHandler(message);
		while( message == (msp430peri_t*)0){
			while(0 == produceHandler());
		}
		switch(message->id)
		{
			case IDFPGASPI:while(0 == rountineFpga(message));break;
			case IDADS1118://while(0 == rountineAds1118(message));break;
			case IDDAC8571://while(0 == rountineDac8571(message));break;
			case IDDAC8811://while(0 == rountineDac8811(message));break;
			case IDUARTSCREEN:while(0 == rountineUartScreen(message));break;
			case IDPCUARTIDLE://维持一个开销，可以删除。
			case IDPCUART:while(0 == rountinePCUart(message));break;
			case IDIDLE:while(0 == rountineIdle(message));break;
			default:;
		}
//		clearQueue();
	}
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER1_A0_ISR(void)
{
    uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A1_BASE,
                                                      TIMER_A_CAPTURECOMPARE_REGISTER_0)
                       + COMPARE_VALUE;
    clearQueue();
    Timer_A_setCompareValue(TIMER_A1_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            compVal
                            );
}
