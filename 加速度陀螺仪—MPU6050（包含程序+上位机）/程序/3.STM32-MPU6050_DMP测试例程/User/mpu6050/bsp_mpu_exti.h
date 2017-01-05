#ifndef __MPU_EXTI_H
#define	__MPU_EXTI_H

#include "stm32f4xx.h"


#define MPU_INT_GPIO_PORT                GPIOF
#define MPU_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOF
#define MPU_INT_GPIO_PIN                 GPIO_Pin_10
#define MPU_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOF
#define MPU_INT_EXTI_PINSOURCE           EXTI_PinSource10
#define MPU_INT_EXTI_LINE                EXTI_Line10
#define MPU_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define MPU_IRQHandler                   EXTI15_10_IRQHandler

void EXTI_MPU_Config(void);

#endif /* __EXTI_H */
