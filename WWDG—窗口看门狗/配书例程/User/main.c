/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   窗口看门狗例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h" 
#include "./wwdg/bsp_wwdg.h"



static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */

int main(void)
{
	uint8_t wwdg_tr, wwdg_wr;
	
	/* LED 端口初始化 */
	LED_GPIO_Config();
  
	// BLUE 蓝色灯亮
	LED3(ON);
	Delay(0XFFFFFF);	
	
	// WWDG配置
	
/* WWDG 配置函数
 * tr ：递减计时器的值， 取值范围为：0x7f~0x40，超出范围会直接复位
 * wr ：窗口值，取值范围为：0x7f~0x40
 * prv：预分频器值，取值可以是
	 *      @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(42MHz)/4096)/1  约10253Hz 97us
	 *      @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(42MHz)/4096)/2	 约5126Hz	195us
	 *      @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(42MHz)/4096)/4	 约2563Hz	390us
	 *      @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(42MHz)/4096)/8  约1281Hz	780us
	 *      
	 *			例：tr = 127(0x7f，tr的最大值)  wr = 80（0x50, 0x40为最小wr最小值）  prv =  WWDG_Prescaler_8
	 *			~780 * (127-80) = 36.6ms < 刷新窗口 < ~780 * 64 = 49.9ms
	 *			也就是说调用WWDG_Config进行这样的配置，若在之后的36.6ms前喂狗，系统会复位，在49.9ms后没有喂狗，系统也会复位。
	 *			需要在刷新窗口的时间内喂狗，系统才不会复位。	
*/	
	// 初始化WWDG：配置计数器初始值，配置上窗口值，启动WWDG，使能提前唤醒中断
	WWDG_Config(127,80,WWDG_Prescaler_8);	
	
	// 窗口值我们在初始化的时候设置成0X5F，这个值不会改变
	wwdg_wr = WWDG->CFR & 0X7F;
	
	while(1)
	{	
		// BLUE 蓝色灯
		LED3(OFF);
		//-----------------------------------------------------
		// 这部分应该写需要被WWDG监控的程序，这段程序运行的时间
		// 决定了窗口值应该设置成多大。
		//-----------------------------------------------------
		
		// 计时器值，初始化成最大0X7F，当开启WWDG时候，这个值会不断减小
		// 当计数器的值大于窗口值时喂狗的话，会复位，当计数器减少到0X40
		// 还没有喂狗的话就非常非常危险了，计数器再减一次到了0X3F时就复位
		// 所以要当计数器的值在窗口值和0X40之间的时候喂狗，其中0X40是固定的。
		wwdg_tr = WWDG->CR & 0X7F;
		
		if( wwdg_tr < wwdg_wr )
		{
			// 喂狗，重新设置计数器的值为最大0X7F
			WWDG_Feed();
		}
	}
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
