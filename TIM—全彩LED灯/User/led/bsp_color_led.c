/**
  ******************************************************************************
  * @file    bsp_breath_led.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   全彩LED灯驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_color_led.h"

 /**
  * @brief  配置TIM3复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( COLOR_TIM_GPIO_CLK, ENABLE); 

		GPIO_PinAFConfig(COLOR_RED_GPIO_PORT,COLOR_RED_PINSOURCE,COLOR_RED_AF); 
		GPIO_PinAFConfig(COLOR_GREEN_GPIO_PORT,COLOR_GREEN_PINSOURCE,COLOR_GREEN_AF); 
		GPIO_PinAFConfig(COLOR_BLUE_GPIO_PORT,COLOR_BLUE_PINSOURCE,COLOR_BLUE_AF);  
	
		/*COLOR_LED1*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_RED_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	
		GPIO_Init(COLOR_RED_GPIO_PORT, &GPIO_InitStructure);	
    
    /*COLOR_LED2*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_GREEN_PIN;	
    GPIO_Init(COLOR_GREEN_GPIO_PORT, &GPIO_InitStructure);	
    
    /*COLOR_LED3*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_BLUE_PIN;	
    GPIO_Init(COLOR_BLUE_GPIO_PORT, &GPIO_InitStructure);	
		
}



/**
  * @brief  配置COLOR_TIMx输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	COLOR_RED_TIM_APBxClock_FUN(COLOR_RED_TIM_CLK,ENABLE);
	COLOR_GREEN_TIM_APBxClock_FUN(COLOR_GREEN_TIM_CLK,ENABLE);
	COLOR_BLUE_TIM_APBxClock_FUN(COLOR_BLUE_TIM_CLK,ENABLE);

	
	/*注意要把三个通道的定时器频率配置为一致*/
  
	/* 累计 TIM_Period个后产生一个更新或者中断*/		 
  TIM_TimeBaseStructure.TIM_Period = 256-1;       //当定时器从0计数到255，即为256次，为一个定时周期
	
	// APB1定时器时钟源TIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
	// 定时器频率为 = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000)*30 = 1000000/30 = 1/30MHz


	//APB2定时器时钟源TIMxCLK = 2 * PCLK2  
	//				PCLK2 = HCLK / 2 
	//				=> TIMxCLK = HCLK  = SystemCoreClock 
	// 定时器频率为 = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock )/((SystemCoreClock)/1000000)*30 = 1000000/30 = 1/30MHz


	/*基本定时器配置TIM_Prescaler根据效果来设置，中断周期小 灯闪烁快，大则闪烁缓慢*/

	TIM_TimeBaseStructure.TIM_Prescaler = COLOR_RED_TIM_PRESCALER;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;		 //设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  	
	// 初始化定时器TIMx
	TIM_TimeBaseInit(COLOR_RED_TIM, &TIM_TimeBaseStructure);	
	
	
	/*基本定时器配置TIM_Prescaler根据效果来设置即可，中断周期小 灯闪烁快，大则闪烁缓慢*/
	TIM_TimeBaseStructure.TIM_Prescaler = COLOR_GREEN_TIM_PRESCALER;	 
	// 初始化定时器TIMx
	TIM_TimeBaseInit(COLOR_GREEN_TIM, &TIM_TimeBaseStructure);	
		
	/*基本定时器配置TIM_Prescaler根据效果来设置即可，中断周期小 灯闪烁快，大则闪烁缓慢*/
	TIM_TimeBaseStructure.TIM_Prescaler = COLOR_BLUE_TIM_PRESCALER;	 
	// 初始化定时器TIMx
	TIM_TimeBaseInit(COLOR_BLUE_TIM, &TIM_TimeBaseStructure);	
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;	  														//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR_Val时为低电平 LED灯亮
 
  //使能通道  
  COLOR_RED_TIM_OCxInit(COLOR_RED_TIM, &TIM_OCInitStructure);	 							
	/*使能通道重载*/
	COLOR_RED_TIM_OCxPreloadConfig(COLOR_RED_TIM, TIM_OCPreload_Enable);
	
	//使能通道  
	COLOR_GREEN_TIM_OCxInit(COLOR_GREEN_TIM, &TIM_OCInitStructure);	 
	/*使能通道重载*/
	COLOR_GREEN_TIM_OCxPreloadConfig(COLOR_GREEN_TIM, TIM_OCPreload_Enable);

  //使能通道  
  COLOR_BLUE_TIM_OCxInit(COLOR_BLUE_TIM, &TIM_OCInitStructure);	 
	/*使能通道重载*/
	COLOR_BLUE_TIM_OCxPreloadConfig(COLOR_BLUE_TIM, TIM_OCPreload_Enable);
	
	//使能TIM重载寄存器ARR
	TIM_ARRPreloadConfig(COLOR_RED_TIM, ENABLE);			
	TIM_ARRPreloadConfig(COLOR_GREEN_TIM, ENABLE);			
	TIM_ARRPreloadConfig(COLOR_BLUE_TIM, ENABLE);			


	// 使能计数器
	TIM_Cmd(COLOR_RED_TIM, ENABLE);		
	TIM_Cmd(COLOR_GREEN_TIM, ENABLE);
	TIM_Cmd(COLOR_BLUE_TIM, ENABLE);
	
}


 /**
  * @brief  设置RGB LED的颜色
	* @param  rgb:要设置LED显示的颜色值格式RGB888
  * @retval 无
  */
void SetRGBColor(uint32_t rgb)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_RED_TIM->COLOR_RED_CCRx = (uint8_t)(rgb>>16);				//R
	COLOR_GREEN_TIM->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);	  //G     
	COLOR_BLUE_TIM->COLOR_BLUE_CCRx = (uint8_t)rgb;						//B
}

 /**
  * @brief  设置RGB LED的颜色
	* @param  r\g\b:要设置LED显示的颜色值
  * @retval 无
  */
void SetColorValue(uint8_t r,uint8_t g,uint8_t b)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_RED_TIM->COLOR_RED_CCRx = r;	
	COLOR_GREEN_TIM->COLOR_GREEN_CCRx = g;	  
	COLOR_BLUE_TIM->COLOR_BLUE_CCRx = b;	
}


/**
  * @brief  初始化呼吸灯
  * @param  无
  * @retval 无
  */
void ColorLED_Config(void)
{

	TIMx_GPIO_Config();
	
	TIM_Mode_Config();
	
	//上电后默认显示
	SetColorValue(0xff,0xff,0xff);

}

/*********************************************END OF FILE**********************/
