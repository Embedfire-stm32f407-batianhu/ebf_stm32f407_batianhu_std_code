/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   基本定时器定时
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
#include "./tim/bsp_basic_tim.h"
#include "./led/bsp_led.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{

	LED_GPIO_Config();

  /* 初始化基本定时器定时，0.5s产生一次中断 */
	TIMx_Configuration();
  
  while(1)
  {       
  }
}



/*********************************************END OF FILE**********************/

