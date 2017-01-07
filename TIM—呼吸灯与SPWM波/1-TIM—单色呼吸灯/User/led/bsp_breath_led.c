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
  
#include "./led/bsp_breath_led.h"

//控制输出波形的频率
__IO uint16_t period_class = 10;

/* LED亮度等级 PWM表,指数曲线 ，此表使用工程目录下的python脚本index_wave.py生成*/
uint16_t indexWave[] = {
1, 1, 1, 1, 2, 2, 2, 2, 3, 3,
4, 4, 5, 5, 6, 7, 8, 9, 10, 
11, 13, 15, 17, 19, 22, 25,
28, 32, 36, 41, 47, 53, 61,
69, 79, 89, 102, 116, 131, 
149, 170, 193, 219, 250, 284,
323, 367, 417, 474, 539, 613, 
697, 792, 901, 1024, 1024, 901, 
792, 697, 613, 539, 474, 417, 
367, 323, 284, 250, 219, 193, 
170, 149, 131, 116, 102, 89, 79,
69, 61, 53, 47, 41, 36, 32, 28, 
25, 22, 19, 17, 15, 13, 11, 10, 
9, 8, 7, 6, 5, 5, 4, 4, 3, 3, 2,
2, 2, 2, 1, 1, 1, 1	
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
		RCC_AHB1PeriphClockCmd ( BRE_LED_GPIO_CLK, ENABLE); 

		GPIO_PinAFConfig(BRE_LED_GPIO_PORT,BRE_LED_PINSOURCE,BRE_LED_AF); 
	
		/*BREATH_LED1*/															   
		GPIO_InitStructure.GPIO_Pin = BRE_LED_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_Init(BRE_LED_GPIO_PORT, &GPIO_InitStructure);	    
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
		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BRE_TIM_IRQn; 
	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
  BRE_TIM_APBxClock_FUN(BRE_TIM_CLK, ENABLE); 
	
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
		POINT_NUM = 110

		#周期倍数
		PERIOD_CLASS = 10

		#幅值等级
		AMPLITUDE_CLASS = 1

		#定时器定时周期
		TIMER_TIM_Period = 2**10

		#APB1定时器分频
		TIMER_TIM_Prescaler1 = 235
		#APB2定时器分频
		TIMER_TIM_Prescaler2 = 470

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
	
	APB1总线定时器呼吸周期：3.151238
	APB2总线定时器呼吸周期：3.151238
	************************************************************/	
	
  TIM_TimeBaseStructure.TIM_Period = (1024-1);      					//当定时器从0计数到 TIM_Period+1 ，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = BRE_TIM_PRESCALER;	//设置预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  	
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BRE_TIM, &TIM_TimeBaseStructure);	
	
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;	  												//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  //当定时器计数值小于CCR1_Val时为低电平 LED灯亮
  
	BRE_TIM_OCxInit(BRE_TIM, &TIM_OCInitStructure);	 //使能通道
  
	/*使能通道重载*/
	BRE_TIM_OCxPreloadConfig(BRE_TIM, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(BRE_TIM, ENABLE);			//使能TIM重载寄存器ARR

	// 开启计数器中断
	TIM_ITConfig(BRE_TIM,TIM_IT_Update,ENABLE);
	
	// 使能计数器
	TIM_Cmd(BRE_TIM, ENABLE);	
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
