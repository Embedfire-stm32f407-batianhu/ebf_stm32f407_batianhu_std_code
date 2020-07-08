/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   呼吸灯例程
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
#include "./led/bsp_breath_led.h"
#include "./usart/bsp_debug_usart.h"



/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{

  /*初始化串口*/
  Debug_USART_Config();
 
	printf("\r\n 欢迎使用野火  STM32 F407 开发板。\r\n");		
	printf("\r\n 呼吸灯例程\r\n");	  
  printf("\r\n RGB LED 以呼吸灯的形式闪烁\r\n ");

  /* 初始化呼吸灯 */
	BreathLED_Config(); 
  
  while(1)
  {
  }

}


/*********************************************END OF FILE**********************/

