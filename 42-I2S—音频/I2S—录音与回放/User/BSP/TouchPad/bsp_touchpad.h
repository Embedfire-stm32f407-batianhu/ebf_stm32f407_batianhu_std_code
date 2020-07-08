#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "stm32f4xx.h"
 
/******************** TPAD 引脚配置参数定义 **************************/
#define TPAD_TIMx                   TIM2
#define TPAD_TIM_CLK                RCC_APB1Periph_TIM2

#define TPAD_TIM_Channel_X          TIM_Channel_1
#define TPAD_TIM_IT_CCx             TIM_IT_CC1
#define TPAD_TIM_GetCaptureX        TIM_GetCapture1

#define TPAD_TIM_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define TPAD_TIM_CH_PORT            GPIOA
#define TPAD_TIM_CH_PIN             GPIO_Pin_5
#define TPAD_TIM_AF                 GPIO_AF_TIM2
#define TPAD_TIM_SOURCE             GPIO_PinSource5

/************************** TPAD 函数声明********************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(uint8_t mode);
 

#endif /* __TOUCHPAD_H_ */

