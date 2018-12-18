/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   高级控制定时器输入捕获
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
#include "./tim/bsp_advance_tim.h"
#include "./usart/bsp_debug_usart.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{
  Debug_USART_Config();
	printf("\r\n这是一个定时器输入捕获例程\r\n");
	
  /* 初始化高级控制定时器输入捕获以及通用定时器输出PWM */
	TIMx_Configuration();
  
  while(1)
  {       
  }
}



/*********************************************END OF FILE**********************/

