#ifndef __COLOR_LED_H
#define	__COLOR_LED_H

#include "stm32f4xx.h"

  /*
	！！！！！！！！！！！！！！！！！！
  本工程的LED灯各通道使用的不是同一个定时器
  设计硬件时建议使用同一个定时器来控制三盏灯，简化代码
	若使用的硬件是同一个定时器，需要调整源代码和头文件，不能直接修改宏来实现
	特别是定时器初始化部分。
	！！！！！！！！！！！！！！！！！！
*/
/********************定时器通道**************************/
#define COLOR_TIM_GPIO_CLK             (RCC_AHB1Periph_GPIOF)

/************红灯***************/
#define COLOR_RED_TIM           						TIM10
#define COLOR_RED_TIM_CLK       						RCC_APB2Periph_TIM10
#define COLOR_RED_TIM_APBxClock_FUN        RCC_APB2PeriphClockCmd
/*计算说明见c文件*/
/*部分通用定时器的时钟为HCLK/4，部分为HCLK/2，注意要把三个通道的定时器频率配置为一致*/
#define COLOR_RED_TIM_PRESCALER						(((SystemCoreClock)/1000000)*30-1)

/************绿灯***************/
#define COLOR_GREEN_TIM           						TIM11
#define COLOR_GREEN_TIM_CLK       						RCC_APB2Periph_TIM11
#define COLOR_GREEN_TIM_APBxClock_FUN        RCC_APB2PeriphClockCmd
/*部分通用定时器的时钟为HCLK/4，部分为HCLK/2，注意要把三个通道的定时器频率配置为一致*/
#define COLOR_GREEN_TIM_PRESCALER						(((SystemCoreClock)/1000000)*30-1)

/************蓝灯***************/
#define COLOR_BLUE_TIM           							TIM13
#define COLOR_BLUE_TIM_CLK       						RCC_APB1Periph_TIM13
#define COLOR_BLUE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
/*部分通用定时器的时钟为HCLK/4，部分为HCLK/2，注意要把三个通道的定时器频率配置为一致*/
#define COLOR_BLUE_TIM_PRESCALER						(((SystemCoreClock/2)/1000000)*30-1)



/************红灯***************/

#define COLOR_RED_PIN                  GPIO_Pin_6                 
#define COLOR_RED_GPIO_PORT            GPIOF                      
#define COLOR_RED_PINSOURCE						GPIO_PinSource6
#define COLOR_RED_AF										GPIO_AF_TIM10

#define  COLOR_RED_TIM_OCxInit                TIM_OC1Init            //通道初始化函数
#define  COLOR_RED_TIM_OCxPreloadConfig       TIM_OC1PreloadConfig //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：COLOR_TIMx->COLOR_RED_CCRx ，可访问该通道的比较寄存器
#define  COLOR_RED_CCRx                       	CCR1		

/************绿灯***************/
#define COLOR_GREEN_PIN                  GPIO_Pin_7                 
#define COLOR_GREEN_GPIO_PORT            GPIOF                      
#define COLOR_GREEN_PINSOURCE						GPIO_PinSource7
#define COLOR_GREEN_AF										GPIO_AF_TIM11

#define  COLOR_GREEN_TIM_OCxInit                TIM_OC1Init            //通道初始化函数
#define  COLOR_GREEN_TIM_OCxPreloadConfig       TIM_OC1PreloadConfig //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：COLOR_TIMx->COLOR_GREEN_CCRx ，可访问该通道的比较寄存器
#define  COLOR_GREEN_CCRx                       CCR1

/************蓝灯***************/
#define COLOR_BLUE_PIN                  GPIO_Pin_8                 
#define COLOR_BLUE_GPIO_PORT            GPIOF                       
#define COLOR_BLUE_PINSOURCE						GPIO_PinSource8
#define COLOR_BLUE_AF										GPIO_AF_TIM13

#define   COLOR_BLUE_TIM_OCxInit              TIM_OC1Init            //通道初始化函数
#define   COLOR_BLUE_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig  //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：COLOR_TIMx->COLOR_BLUE_CCRx ，可访问该通道的比较寄存器
#define   COLOR_BLUE_CCRx                      CCR1


/************************************************************/

/* RGB颜色值 24位*/
#define COLOR_WHITE          0xFFFFFF
#define COLOR_BLACK          0x000000
#define COLOR_GREY           0xC0C0C0
#define COLOR_BLUE           0x0000FF
#define COLOR_RED            0xFF0000
#define COLOR_MAGENTA        0xFF00FF
#define COLOR_GREEN          0x00FF00
#define COLOR_CYAN           0x00FFFF
#define COLOR_YELLOW         0xFFFF00


void ColorLED_Config(void);
void SetRGBColor(uint32_t rgb);
void SetColorValue(uint8_t r,uint8_t g,uint8_t b);

#endif /* __COLOR_LED_H */
