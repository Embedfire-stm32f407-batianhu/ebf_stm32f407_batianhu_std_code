#ifndef __ADVANCE_TIM_H
#define	__ADVANCE_TIM_H

#include "stm32f4xx.h"

#define ADVANCE_TIM           		TIM1
#define ADVANCE_TIM_CLK       		RCC_APB2Periph_TIM1

#define ADVANCE_TIM_IRQn					TIM1_UP_TIM10_IRQn
#define ADVANCE_TIM_IRQHandler    TIM1_UP_TIM10_IRQHandler


void TIMx_Configuration(void);

#endif /* __ADVANCE_TIM_H */

