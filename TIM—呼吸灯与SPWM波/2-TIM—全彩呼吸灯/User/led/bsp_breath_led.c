/**
  ******************************************************************************
  * @file    bsp_breath_led.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   呼吸灯驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_breath_led.h"

//控制输出波形的频率
__IO uint16_t period_class = 1;

/* LED亮度等级 PWM表,指数曲线 ，此表使用工程目录下的python脚本index_wave.py生成*/
uint16_t indexWave[] = {
1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
	2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 
	6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 
	12, 13, 14, 15, 17, 18, 19, 20, 22, 
	23, 25, 27, 29, 31, 33, 36, 38, 41, 
	44, 47, 51, 54, 58, 63, 67, 72, 77, 
	83, 89, 95, 102, 110, 117, 126, 135,
	145, 156, 167, 179, 192, 206, 221, 237, 
	254, 272, 292, 313, 336, 361, 387, 415, 
	445, 477, 512, 512, 477, 445, 415, 387, 
	361, 336, 313, 292, 272, 254, 237, 221, 
	206, 192, 179, 167, 156, 145, 135, 126, 
	117, 110, 102, 95, 89, 83, 77, 72, 67, 63,
	58, 54, 51, 47, 44, 41, 38, 36, 33, 31, 29,
	27, 25, 23, 22, 20, 19, 18, 17, 15, 14, 13,
	12, 12, 11, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5,
	5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2,
	2, 2, 1, 1, 1, 1, 1, 1

};

//计算PWM表有多少个元素
uint16_t POINT_NUM = sizeof(indexWave)/sizeof(indexWave[0]); 

 /**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( BRE_TIM_GPIO_CLK, ENABLE); 

		GPIO_PinAFConfig(BRE_RED_GPIO_PORT,BRE_RED_PINSOURCE,BRE_RED_AF); 
		GPIO_PinAFConfig(BRE_GREEN_GPIO_PORT,BRE_GREEN_PINSOURCE,BRE_GREEN_AF); 
		GPIO_PinAFConfig(BRE_BLUE_GPIO_PORT,BRE_BLUE_PINSOURCE,BRE_BLUE_AF); 

		/* 配置LED灯用到的引脚 */
		//红
		GPIO_InitStructure.GPIO_Pin = BRE_RED_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_Init(BRE_RED_GPIO_PORT, &GPIO_InitStructure);	    
		
		//绿
		GPIO_InitStructure.GPIO_Pin = BRE_GREEN_PIN;	
		GPIO_Init(BRE_GREEN_GPIO_PORT, &GPIO_InitStructure);	 
		
		//蓝
		GPIO_InitStructure.GPIO_Pin = BRE_BLUE_PIN;	
		GPIO_Init(BRE_BLUE_GPIO_PORT, &GPIO_InitStructure);	 
  
}


 /**
  * @brief  通用定时器 TIMx,x[2,3,4,5]中断优先级配置
  * @param  无
  * @retval 无
  */ 
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		
		//红
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BRE_RED_TIM_IRQn; 
	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
		//绿
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BRE_GREEN_TIM_IRQn; 
	  NVIC_Init(&NVIC_InitStructure);
		
		//蓝
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BRE_BLUE_TIM_IRQn; 
	  NVIC_Init(&NVIC_InitStructure);	
	
}



/**
  * @brief  配置TIM输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */

static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// 开启TIMx_CLK,x[2,3,4,5] 
	BRE_RED_TIM_APBxClock_FUN(BRE_RED_TIM_CLK,ENABLE);
	BRE_GREEN_TIM_APBxClock_FUN(BRE_GREEN_TIM_CLK,ENABLE);
	BRE_BLUE_TIM_APBxClock_FUN(BRE_BLUE_TIM_CLK,ENABLE);

	
	/* 基本定时器配置 ,配合PWM表点数、中断服务函数中的period_cnt循环次数设置*/	
	
	/* 设置使得整个呼吸过程为3秒左右即可达到很好的效果 */	
	
	//要求：
	//TIM_Period：与PWM表中数值范围一致
	//TIM_Prescaler：越小越好，可减轻闪烁现象
	//PERIOD_CLASS：中断服务函数中控制单个点循环的次数，调整它可控制拟合曲线的周期
	//POINT_NUM：PWM表的元素，它是PWM拟合曲线的采样点数

	/*************本实验中的配置***************/	
	/***********************************************
		#PWM点数
		POINT_NUM = 180

		#周期倍数
		PERIOD_CLASS = 1

		#幅值等级
		AMPLITUDE_CLASS = 256

		#定时器定时周期
		TIMER_TIM_Period = 2**9

		#APB1定时器分频
		TIMER_TIM_Prescaler1 = 11
		#APB2定时器分频
		TIMER_TIM_Prescaler2 = 22

		#STM32系统时钟频率和周期
		f_hclk = 168000000

		#定时器计时频率
		f_pclk1 = f_hclk/2
		t_pclk1 = 1/f_pclk1

		f_pclk2 = f_hclk
		t_pclk2 = 1/f_pclk2

		#APB1定时器update事件周期
		t_timer1 = t_pclk1*TIMER_TIM_Prescaler1*TIMER_TIM_Period

		#APB1每个PWM点的时间
		T_Point1 = t_timer1 * PERIOD_CLASS * AMPLITUDE_CLASS

		#APB1整个呼吸周期
		T_Up_Down_Cycle1 = T_Point1 * POINT_NUM

		print ("APB1总线定时器呼吸周期：",T_Up_Down_Cycle1)


		#APB2定时器update事件周期
		t_timer2 = t_pclk2*TIMER_TIM_Prescaler2*TIMER_TIM_Period

		#APB2每个PWM点的时间
		T_Point2 = t_timer2 * PERIOD_CLASS * AMPLITUDE_CLASS

		#APB2整个呼吸周期
		T_Up_Down_Cycle2 = T_Point2 * POINT_NUM

		print ("APB2总线定时器呼吸周期：",T_Up_Down_Cycle2)


		
	#运行结果：
	
	APB1总线定时器呼吸周期：3.08955
	APB2总线定时器呼吸周期：3.08955
	************************************************************/	
	
  TIM_TimeBaseStructure.TIM_Period = 512-1;       //当定时器从0计数到255，即为256次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = BRE_RED_TIM_PRESCALER;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;		 //设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  	
	// 初始化定时器TIMx
	TIM_TimeBaseInit(BRE_RED_TIM, &TIM_TimeBaseStructure);	
	
	
	/*基本定时器配置TIM_Prescaler根据效果来设置即可，中断周期小 灯闪烁快，大则闪烁缓慢*/
	TIM_TimeBaseStructure.TIM_Prescaler = BRE_GREEN_TIM_PRESCALER;	 
	// 初始化定时器TIMx
	TIM_TimeBaseInit(BRE_GREEN_TIM, &TIM_TimeBaseStructure);	
		
	/*基本定时器配置TIM_Prescaler根据效果来设置即可，中断周期小 灯闪烁快，大则闪烁缓慢*/
	TIM_TimeBaseStructure.TIM_Prescaler = BRE_BLUE_TIM_PRESCALER;	 
	// 初始化定时器TIMx
	TIM_TimeBaseInit(BRE_BLUE_TIM, &TIM_TimeBaseStructure);	
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;	  														//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR_Val时为低电平 LED灯亮
 
  //使能通道  
  BRE_RED_TIM_OCxInit(BRE_RED_TIM, &TIM_OCInitStructure);	 							
	/*使能通道重载*/
	BRE_RED_TIM_OCxPreloadConfig(BRE_RED_TIM, TIM_OCPreload_Enable);
	
	//使能通道  
	BRE_GREEN_TIM_OCxInit(BRE_GREEN_TIM, &TIM_OCInitStructure);	 
	/*使能通道重载*/
	BRE_GREEN_TIM_OCxPreloadConfig(BRE_GREEN_TIM, TIM_OCPreload_Enable);

  //使能通道  
  BRE_BLUE_TIM_OCxInit(BRE_BLUE_TIM, &TIM_OCInitStructure);	 
	/*使能通道重载*/
	BRE_BLUE_TIM_OCxPreloadConfig(BRE_BLUE_TIM, TIM_OCPreload_Enable);
	
	//使能TIM重载寄存器ARR
	TIM_ARRPreloadConfig(BRE_RED_TIM, ENABLE);			
	TIM_ARRPreloadConfig(BRE_GREEN_TIM, ENABLE);			
	TIM_ARRPreloadConfig(BRE_BLUE_TIM, ENABLE);			

	// 开启计数器中断
	TIM_ITConfig(BRE_RED_TIM,TIM_IT_Update,ENABLE);
	TIM_ITConfig(BRE_GREEN_TIM,TIM_IT_Update,ENABLE);
	TIM_ITConfig(BRE_BLUE_TIM,TIM_IT_Update,ENABLE);

	// 使能计数器
	TIM_Cmd(BRE_RED_TIM, ENABLE);		
	TIM_Cmd(BRE_GREEN_TIM, ENABLE);
	TIM_Cmd(BRE_BLUE_TIM, ENABLE);
}

/**
  * @brief  初始化呼吸灯
  * @param  无
  * @retval 无
  */
void BreathLED_Config(void)
{

	TIMx_GPIO_Config();
	
	TIM_Mode_Config();

	TIMx_NVIC_Configuration();	
}

/*********************************************END OF FILE**********************/
