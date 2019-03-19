/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   使用SysTick进行精确延时
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
#include "./systick/bsp_SysTick.h"


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
 
  /* 配置SysTick 为10us中断一次,时间到后触发定时中断，
	*进入stm32fxx_it.c文件的SysTick_Handler处理，通过数中断次数计时
	*/
	SysTick_Init();
  
	while(1)
	{

		LED_RED; 
	  Delay_us(100000);    	// 10000 * 10us = 1000ms
		//Delay_ms(1000);
	  
		LED_GREEN;
	  Delay_us(100000);		// 10000 * 10us = 1000ms
		//Delay_ms(1000);
	
		LED_BLUE;
	  Delay_us(100000);		// 10000 * 10us = 1000ms
		//Delay_ms(1000);


	}   
}



/*********************************************END OF FILE**********************/

