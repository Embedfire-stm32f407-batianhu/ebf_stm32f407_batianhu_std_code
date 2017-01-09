/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用1.5.1版本库建的工程模板
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
#include "./Bsp/beep/bsp_beep.h" 
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/systick/bsp_SysTick.h"
#include "./Bsp/TouchPad/bsp_touchpad.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* 初始化蜂鸣器 */
	BEEP_GPIO_Config();
	
	/* 初始化调试串口，一般为串口1 */
	Debug_USART_Config();
	
	/* 初始化系统滴答定时器 */
	SysTick_Init();

  /* 初始化电容按键 */
	TPAD_Init();
	
	while(1)
	{
		if(TPAD_Scan(0))
		{
			BEEP_ON;
			Delay_ms(100);
			BEEP_OFF;
		}		
		Delay_ms(50);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
