#ifndef __BREATH_LED_H
#define	__BREATH_LED_H

#include "stm32f4xx.h"

/*电压幅值等级数*/
#define AMPLITUDE_CLASS 256

//控制输出波形的频率
extern __IO uint16_t period_class ;

/*PWM表中的点数*/
extern uint16_t POINT_NUM;



/********************定时器通道**************************/



//R 红色灯
#define BRE_RED_TIM           							TIM10
#define BRE_RED_TIM_CLK       						RCC_APB2Periph_TIM10
#define BRE_TIM_GPIO_CLK              		(RCC_AHB1Periph_GPIOF)
#define BRE_RED_TIM_APBxClock_FUN        RCC_APB2PeriphClockCmd

#define BRE_RED_TIM_IRQn								TIM1_UP_TIM10_IRQn
#define BRE_RED_TIM_IRQHandler 				TIM1_UP_TIM10_IRQHandler

/*计算说明见c文件*/
/*部分通用定时器的时钟为HCLK/4，部分为HCLK/2*/
#define BRE_RED_TIM_PRESCALER						(22-1)


#define BRE_RED_PIN                  GPIO_Pin_6                 
#define BRE_RED_GPIO_PORT            GPIOF                      
#define BRE_RED_PINSOURCE							GPIO_PinSource6
#define BRE_RED_AF										GPIO_AF_TIM10

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：BRE_TIMx->BRE_LED_CCRx ，可访问该通道的比较寄存器
#define BRE_RED_CCRx									CCR1
#define BRE_RED_TIM_CHANNEL					TIM_Channel_1

#define  BRE_RED_TIM_OCxInit              TIM_OC1Init            //通道选择，1~4
#define  BRE_RED_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig 


//G 绿色灯
#define BRE_GREEN_TIM           							TIM11
#define BRE_GREEN_TIM_CLK       						RCC_APB2Periph_TIM11
#define BRE_GREEN_TIM_APBxClock_FUN        RCC_APB2PeriphClockCmd

#define BRE_GREEN_TIM_IRQn								TIM1_TRG_COM_TIM11_IRQn
#define BRE_GREEN_TIM_IRQHandler 				TIM1_TRG_COM_TIM11_IRQHandler

/*计算说明见c文件*/
/*部分通用定时器的时钟为HCLK/4，部分为HCLK/2*/
#define BRE_GREEN_TIM_PRESCALER						(22-1)


#define BRE_GREEN_PIN                  GPIO_Pin_7                 
#define BRE_GREEN_GPIO_PORT            GPIOF                      
#define BRE_GREEN_PINSOURCE							GPIO_PinSource7
#define BRE_GREEN_AF										GPIO_AF_TIM11

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：BRE_TIM->BRE_LED_CCRx ，可访问该通道的比较寄存器
#define BRE_GREEN_CCRx									CCR1
#define BRE_GREEN_TIM_CHANNEL					TIM_Channel_1

#define  BRE_GREEN_TIM_OCxInit              TIM_OC1Init            //通道选择，1~4
#define  BRE_GREEN_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig 

//B 蓝色灯
#define BRE_BLUE_TIM           							TIM13
#define BRE_BLUE_TIM_CLK       						RCC_APB1Periph_TIM13
#define BRE_BLUE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd

#define BRE_BLUE_TIM_IRQn								TIM8_UP_TIM13_IRQn
#define BRE_BLUE_TIM_IRQHandler 				TIM8_UP_TIM13_IRQHandler

/*计算说明见c文件*/
/*部分通用定时器的时钟为HCLK/4，部分为HCLK/2*/
#define BRE_BLUE_TIM_PRESCALER						(11-1)


#define BRE_BLUE_PIN                  GPIO_Pin_8                 
#define BRE_BLUE_GPIO_PORT            GPIOF                      
#define BRE_BLUE_PINSOURCE							GPIO_PinSource8
#define BRE_BLUE_AF										GPIO_AF_TIM13

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：BRE_TIM->BRE_LED_CCRx ，可访问该通道的比较寄存器
#define BRE_BLUE_CCRx									CCR1
#define BRE_BLUE_TIM_CHANNEL					TIM_Channel_1

#define  BRE_BLUE_TIM_OCxInit              TIM_OC1Init            //通道选择，1~4
#define  BRE_BLUE_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig 


void BreathLED_Config(void);

#endif /* __BREATH_LED_H */
