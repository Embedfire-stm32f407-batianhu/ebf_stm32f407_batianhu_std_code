/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   高级控制定时器互补输出
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./tim/bsp_advance_tim.h"
#include "./key/bsp_key.h" 

extern __IO uint16_t ChannelPulse;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{
	/* 初始化按键GPIO */
  Key_GPIO_Config();
	
  /* 初始化高级控制定时器，设置PWM模式，使能通道1互补输出 */
	TIMx_Configuration();
  
  while(1)
  {       
		/* 扫描KEY1 */
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
		{
			/* 增大占空比 */
			if(ChannelPulse<960)
				ChannelPulse+=64;
			else
				ChannelPulse=1024;
			TIM_SetCompare1(ADVANCE_TIM,ChannelPulse);
		}   
    /* 扫描KEY2 */
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
		{
			/* 减小占空比 */
			if(ChannelPulse>=64)
				ChannelPulse-=64;
			else
				ChannelPulse=0;
			TIM_SetCompare1(ADVANCE_TIM,ChannelPulse);
		}   
  }
}



/*********************************************END OF FILE**********************/

