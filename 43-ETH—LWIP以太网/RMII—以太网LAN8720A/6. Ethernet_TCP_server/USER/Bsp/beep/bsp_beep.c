/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   板载蜂鸣器应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "./Bsp/beep/bsp_beep.h" 

void Beep_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启BEEP相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLK, ENABLE ); 

	/*选择要控制的GPIOG引脚*/															   
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

	/*设置引脚的输出类型为推挽输出*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
	/*设置引脚为上拉模式*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*设置引脚速率为2MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
	
}
/*********************************************END OF FILE**********************/
