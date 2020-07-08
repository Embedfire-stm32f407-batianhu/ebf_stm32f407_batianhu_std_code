#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"

/* 定时器 */
#define GENERAL_TIM_MASTER     		    TIM2
#define GENERAL_TIM_MASTER_CLK        RCC_APB1Periph_TIM2

#define GENERAL_TIM_SLAVE1     		    TIM3
#define GENERAL_TIM_SLAVE1_CLK        RCC_APB1Periph_TIM3

#define GENERAL_TIM_SLAVE11     		  TIM4
#define GENERAL_TIM_SLAVE11_CLK       RCC_APB1Periph_TIM4

/* TIM主模式PWM输出引脚 */
#define GENERAL_MASTER_PIN            GPIO_Pin_5             
#define GENERAL_MASTER_GPIO_PORT      GPIOA                      
#define GENERAL_MASTER_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define GENERAL_MASTER_PINSOURCE			GPIO_PinSource5
#define GENERAL_MASTER_AF							GPIO_AF_TIM2

/* TIM从模式PWM输出引脚 */
#define GENERAL_SLAVE1_PIN            GPIO_Pin_6             
#define GENERAL_SLAVE1_GPIO_PORT      GPIOC                      
#define GENERAL_SLAVE1_GPIO_CLK       RCC_AHB1Periph_GPIOC
#define GENERAL_SLAVE1_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE1_AF							GPIO_AF_TIM3

/* TIM从模式的从模式PWM输出引脚 */
#define GENERAL_SLAVE11_PIN           GPIO_Pin_6             
#define GENERAL_SLAVE11_GPIO_PORT     GPIOB                      
#define GENERAL_SLAVE11_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define GENERAL_SLAVE11_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE11_AF						GPIO_AF_TIM4


void TIMx_Configuration(void);

#endif /* __GENERAL_TIM_H */

