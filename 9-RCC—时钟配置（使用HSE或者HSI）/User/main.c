/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RCC 时钟配置
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

// 使用HSE或者HSI 配置系统时钟
#include "stm32f4xx.h"
#include "./rcc/bsp_clkconfig.h"
#include "./led/bsp_led.h"



void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	  // MCO GPIO  初始化
  MCO1_GPIO_Config();
  MCO2_GPIO_Config();	
  
  // MCO1 输出PLLCLK	
  RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);
	
	// MCO2 输出SYSCLK
	RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO1Div_1);
	// 程序来到main函数之前，启动文件：statup_stm32f4xx.s已经调用
	// SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f4xx.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改	
	// 重新设置系统时钟，这时候可以选择使用HSE还是HSI
	
	/*  注意 ：由于在 PLL 使能后主 PLL 配置参数便不可更改，而系统上电后会
	 *  自动进行初始化，因此在对HES重新初始化之前，需要将system_stm32f4xx.c	  
	 *  中的line514：SetSysClock();注释掉,否则HSE重新初始化之后不生效。
	 */
	
	
	// 使用HSE，配置系统时钟为168M
	//HSE_SetSysClock(25, 336, 2, 7);
	
	//系统时钟超频到216M爽一下，最高是216M，别往死里整
  HSE_SetSysClock(25, 432, 2, 9);
	
	// 使用HSI，配置系统时钟为168M
	//HSI_SetSysClock(16, 336, 2, 7);
	
	// LED 端口初始化 
	LED_GPIO_Config();
	


	while (1)
	{
		LED1( ON );			 // 亮 
		Delay(0x0FFFFF);
		LED1( OFF );		  // 灭
	  Delay(0x0FFFFF);
	}
}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

