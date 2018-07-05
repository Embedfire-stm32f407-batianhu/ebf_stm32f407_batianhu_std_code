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
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./tim/bsp_advance_tim.h"
#include "./key/bsp_key.h" 

extern uint16_t aSRC_Buffer[3];
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
			aSRC_Buffer[0]=64;
			aSRC_Buffer[2]=511;
		}   
    /* 扫描KEY2 */
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
		{
			aSRC_Buffer[0]=383;
			aSRC_Buffer[2]=127;
		}   
  }
}



/*********************************************END OF FILE**********************/

