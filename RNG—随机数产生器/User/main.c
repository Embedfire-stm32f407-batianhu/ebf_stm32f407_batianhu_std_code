/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   使用RNG产生随机数
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
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"

static void Delay(__IO u32 nCount); 


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint8_t i;
	uint32_t random = 0;

	
	LED_GPIO_Config();
	
  /*初始化USART，配置模式为 115200 8-N-1*/
  Debug_USART_Config(); 

	printf("\r\n 欢迎使用秉火  STM32 F407 开发板。\r\n");	
	
	/* 使能RNG时钟 */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	
	/* 使能RNG外设 */
  RNG_Cmd(ENABLE);
	
  while(1)
	{		
		for(i = 0; i < 8; i++)
    {
      /* 等待随机数产生完毕 */
      while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET)
      {
      }

      /*获取随机数*/       
      random = RNG_GetRandomNumber();
			
			printf("\r\nRNG 产生的随机数：0x%08x",random);

    }
		Delay(0xAFFFFFF);


	} 

}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

