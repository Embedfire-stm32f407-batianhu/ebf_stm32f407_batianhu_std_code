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
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./spwm/bsp_spwm.h"

//控制输出波形的频率
__IO uint16_t period_class = 1;

/* SPWM表,正弦曲线，此表使用工程目录下的python脚本sin_wave.py生成*/
const uint16_t indexWave[] = {
0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 89, 98,
	107, 116, 125, 133, 142, 151, 159, 168, 176,
	184, 193, 201, 209, 218, 226, 234, 242, 249,
	257, 265, 273, 280, 288, 295, 302, 310, 317, 
	324, 331, 337, 344, 351, 357, 364, 370, 376, 
	382, 388, 394, 399, 405, 410, 416, 421, 426, 
	431, 436, 440, 445, 449, 454, 458, 462, 465, 
	469, 473, 476, 479, 482, 485, 488, 491, 493, 
	496, 498, 500, 502, 503, 505, 506, 508, 509, 
	510, 510, 511, 512, 512, 512, 512, 512, 512,
	511, 510, 510, 509, 508, 506, 505, 503, 502,
	500, 498, 496, 493, 491, 488, 485, 482, 479,
	476, 473, 469, 465, 462, 458, 454, 449, 445, 
	440, 436, 431, 426, 421, 416, 410, 405, 399, 
	394, 388, 382, 376, 370, 364, 357, 351, 344, 
	337, 331, 324, 	317, 310, 302, 295, 288, 280, 
	273, 265, 257, 249, 242, 234, 226, 218, 209, 
	201, 193, 184, 176, 168, 159, 151, 142, 133, 
125, 116, 107, 98, 89, 81, 72, 63, 54, 45, 36,
27, 18, 9, 0
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
