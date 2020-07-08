#ifndef __ADVANCE_TIM_H
#define	__ADVANCE_TIM_H

#include "stm32f4xx.h"

/* 定时器 */
#define ADVANCE_TIM           		    TIM8
#define ADVANCE_TIM_CLK       		    RCC_APB2Periph_TIM8

/* TIM8通道1输出引脚 */
#define ADVANCE_OCPWM_PIN             GPIO_Pin_6              
#define ADVANCE_OCPWM_GPIO_PORT       GPIOC                      
#define ADVANCE_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define ADVANCE_OCPWM_PINSOURCE				GPIO_PinSource6
#define ADVANCE_OCPWM_AF							GPIO_AF_TIM8

/* DMA  */
#define TIM_DMA_ADDRESS               TIM8_BASE+0x34
#define TIM_DMA_BUFSIZE               3
#define TIM_DMA_CLK                   RCC_AHB1Periph_DMA2
#define TIM_DMA_CHANNEL               DMA_Channel_7
#define TIM_DMA_STREAM                DMA2_Stream2

void TIMx_Configuration(void);

#endif /* __ADVANCE_TIM_H */

