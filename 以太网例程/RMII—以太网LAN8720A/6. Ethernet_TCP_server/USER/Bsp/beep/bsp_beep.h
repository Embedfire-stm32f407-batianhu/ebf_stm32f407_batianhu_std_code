#ifndef __BEEP_H_
#define	__BEEP_H_

#include "stm32f4xx.h"


/******************** BEEP 引脚配置参数定义 **************************/
#define BEEP_GPIO_CLK      RCC_AHB1Periph_GPIOI
#define BEEP_PORT          GPIOI
#define BEEP_PIN           GPIO_Pin_11



/******************** BEEP 函数宏定义 **************************/
#define BEEP_ON            GPIO_SetBits(BEEP_PORT,BEEP_PIN)
#define BEEP_OFF           GPIO_ResetBits(BEEP_PORT,BEEP_PIN)


/************************** BEEP 函数声明********************************/
void Beep_GPIO_Config(void);



#endif /* __BEEP_H_ */
