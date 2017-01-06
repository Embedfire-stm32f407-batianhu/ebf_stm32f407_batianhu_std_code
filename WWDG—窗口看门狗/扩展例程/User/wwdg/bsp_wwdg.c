/**
  ******************************************************************************
  * @file    bsp_wwdg.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   窗口看门狗驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./wwdg/bsp_wwdg.h"   

//用于记录看门狗 递减计数器的值，方便喂狗函数直接使用
static uint8_t wwdg_cnt ;

// WWDG 中断优先级初始化
static void WWDG_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}




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
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	
	wwdg_cnt = tr;//保存CNT配置，用在喂狗函数
	
	// 开启 WWDG 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// 设置预分频器的值
	WWDG_SetPrescaler( prv );
	
	// 设置上窗口值
	WWDG_SetWindowValue( wr );
	
	// 设置计数器的值，使能WWDG
	WWDG_Enable(tr);	
	
	// 清除提前唤醒中断标志位
	WWDG_ClearFlag();	
	// 配置WWDG中断优先级
	WWDG_NVIC_Config();	
	// 开WWDG 中断
	WWDG_EnableIT();
}

// 喂狗
void WWDG_Feed(void)
{
	// 喂狗，刷新递减计数器的值，设置成最大WDG_CNT=0X7F
	WWDG_SetCounter( wwdg_cnt );
}


/*********************************************END OF FILE**********************/
