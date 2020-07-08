/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器级联同步PWM输出范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_general_tim.h"

/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO clocks enable */
  RCC_AHB1PeriphClockCmd(GENERAL_MASTER_GPIO_CLK |GENERAL_SLAVE1_GPIO_CLK | GENERAL_SLAVE11_GPIO_CLK, ENABLE);

  /* GPIO Configuration: PC6(TIM3 CH1) as alternate function push-pull ------*/
  GPIO_InitStructure.GPIO_Pin = GENERAL_SLAVE1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GENERAL_SLAVE1_GPIO_PORT, &GPIO_InitStructure);

  /* Connect TIM pins to GPIO_AF_TIM3 */
  GPIO_PinAFConfig(GENERAL_SLAVE1_GPIO_PORT, GENERAL_SLAVE1_PINSOURCE, GENERAL_SLAVE1_AF);

  /* GPIOB Configuration: PD12(TIM4 CH1) as alternate function push-pull ------*/
  GPIO_InitStructure.GPIO_Pin = GENERAL_SLAVE11_PIN;
  GPIO_Init(GENERAL_SLAVE11_GPIO_PORT, &GPIO_InitStructure);
  
  /* Connect TIM pins to GPIO_AF_TIM4 */
  GPIO_PinAFConfig(GENERAL_SLAVE11_GPIO_PORT, GENERAL_SLAVE11_PINSOURCE, GENERAL_SLAVE11_AF);  

  /* GPIOA Configuration: PA5(TIM2 CH1) as alternate function push-pull ------*/
  GPIO_InitStructure.GPIO_Pin = GENERAL_MASTER_PIN;
  GPIO_Init(GENERAL_MASTER_GPIO_PORT, &GPIO_InitStructure);

  /* Connect TIM pins to GPIO_AF_TIM2 */
  GPIO_PinAFConfig(GENERAL_MASTER_GPIO_PORT, GENERAL_MASTER_PINSOURCE, GENERAL_MASTER_AF);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	  /* TIM clock enable */
  RCC_APB1PeriphClockCmd(GENERAL_TIM_MASTER_CLK | GENERAL_TIM_SLAVE1_CLK |
                         GENERAL_TIM_SLAVE11_CLK, ENABLE);
  /* Timers synchronisation in cascade mode ------------------------------------
     1/TIM2 is configured as Master Timer:
         - PWM Mode is used
         - The TIM2 Update event is used as Trigger Output  

     2/TIM3 is slave for TIM2 and Master for TIM4,
         - PWM Mode is used
         - The ITR1(TIM2) is used as input trigger 
         - Gated mode is used, so start and stop of slave counter
           are controlled by the Master trigger output signal(TIM2 update event).
         - The TIM3 Update event is used as Trigger Output. 

     3/TIM4 is slave for TIM3,
         - PWM Mode is used
         - The ITR2(TIM3) is used as input trigger
         - Gated mode is used, so start and stop of slave counter
           are controlled by the Master trigger output signal(TIM3 update event).

     In this example TIM2 input clock (TIM2CLK) is set to 2 * APB1 clock (PCLK1), 
     since APB1 prescaler is different from 1.   
     TIM2CLK = 2 * PCLK1  
     PCLK1 = HCLK / 4 
     => TIM2CLK = HCLK / 2 = SystemCoreClock /2

     The Master Timer TIM2 is running at TIM2 counter clock:
     TIM2 frequency = (TIM2 counter clock)/ (TIM2 period + 1) = 500 KHz 
     and the duty cycle = TIM2_CCR1/(TIM2_ARR + 1) = 25%.

     The TIM3 is running:
     - At (TIM2 frequency)/ (TIM3 period + 1) = 100 KHz and a duty cycle
       equal to TIM3_CCR1/(TIM3_ARR + 1) = 20%

     The TIM4 is running:
     - At (TIM3 frequency)/ (TIM4 period + 1) = 20 KHz and a duty cycle
       equal to TIM4_CCR1/(TIM4_ARR + 1) = 20%
  
     Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect. 
  --------------------------------------------------------------------------- */

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 168;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(GENERAL_TIM_MASTER, &TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Period = 4;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Period = 4;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Master Configuration in PWM1 Mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 45;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(GENERAL_TIM_MASTER, &TIM_OCInitStructure);

  /* Select the Master Slave Mode */
  TIM_SelectMasterSlaveMode(GENERAL_TIM_MASTER, TIM_MasterSlaveMode_Enable);

  /* Master Mode selection */
  TIM_SelectOutputTrigger(GENERAL_TIM_MASTER, TIM_TRGOSource_Update);

  /* Slaves Configuration: PWM1 Mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;

  TIM_OC1Init(GENERAL_TIM_SLAVE1, &TIM_OCInitStructure);

  TIM_OC1Init(GENERAL_TIM_SLAVE11, &TIM_OCInitStructure);

  /* Slave Mode selection: TIM3 */
  TIM_SelectSlaveMode(GENERAL_TIM_SLAVE1, TIM_SlaveMode_Gated);
  TIM_SelectInputTrigger(GENERAL_TIM_SLAVE1, TIM_TS_ITR1);

  /* Select the Master Slave Mode */
  TIM_SelectMasterSlaveMode(GENERAL_TIM_SLAVE1, TIM_MasterSlaveMode_Enable);

  /* Master Mode selection: TIM3 */
  TIM_SelectOutputTrigger(GENERAL_TIM_SLAVE1, TIM_TRGOSource_Update);

  /* Slave Mode selection: TIM4 */
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Gated);
  TIM_SelectInputTrigger(GENERAL_TIM_SLAVE11, TIM_TS_ITR2);
  
  /* TIM enable counter */
  TIM_Cmd(GENERAL_TIM_SLAVE1,  ENABLE);
  TIM_Cmd(GENERAL_TIM_MASTER,  ENABLE);
  TIM_Cmd(GENERAL_TIM_SLAVE11, ENABLE);
}

/**
  * @brief  初始化通用定时器级联同步PWM输出
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{  
	TIMx_GPIO_Config();
	
  TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/
