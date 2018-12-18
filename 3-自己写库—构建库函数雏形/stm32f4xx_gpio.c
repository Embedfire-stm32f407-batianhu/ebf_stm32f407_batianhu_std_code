
#include "stm32f4xx_gpio.h"



/**
  *函数功能：设置引脚为高电平
  *参数说明：GPIOx，该参数为GPIO_TypeDef类型的指针，指向GPIO端口的地址
  * 			  GPIO_Pin:选择要设置的GPIO端口引脚，可输入宏GPIO_Pin_0-15，
	*										表示GPIOx端口的0-15号引脚。
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	/*设置GPIOx端口BSRRL寄存器的第GPIO_Pin位，使其输出高电平*/
	/*因为BSRR寄存器写0不影响，
	  GPIO_Pin只是对应位为1，其它位均为0，所以可以直接赋值*/
	
  GPIOx->BSRRL = GPIO_Pin;
}

/**
  *函数功能：设置引脚为低电平
  *参数说明：GPIOx，该参数为GPIO_TypeDef类型的指针，指向GPIO端口的地址
  * 			  GPIO_Pin:选择要设置的GPIO端口引脚，可输入宏GPIO_Pin_0-15，
	*										表示GPIOx端口的0-15号引脚。
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	/*设置GPIOx端口BSRRH寄存器的第GPIO_Pin位,使其输出低电平*/
	/*因为BSRR寄存器写0不影响，
	GPIO_Pin只是对应位为1，其它位均为0，所以可以直接赋值*/
	
  GPIOx->BSRRH = GPIO_Pin;
}


/**
  *函数功能：初始化引脚模式
  *参数说明：GPIOx，该参数为GPIO_TypeDef类型的指针，指向GPIO端口的地址
  * 			  GPIO_InitTypeDef:GPIO_InitTypeDef结构体指针，指向初始化变量
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
	  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;
	
	/*-- GPIO Mode Configuration --*/
  for (pinpos = 0x00; pinpos < 16; pinpos++)
  {
		/*以下运算是为了通过 GPIO_InitStruct->GPIO_Pin 算出引脚号0-15*/
		
		/*经过运算后pos的pinpos位为1，其余为0，与GPIO_Pin_x宏对应。pinpos变量每次循环加1，*/
		pos = ((uint32_t)0x01) << pinpos;
   
		/* pos与GPIO_InitStruct->GPIO_Pin做 & 运算，若运算结果currentpin == pos，
		则表示GPIO_InitStruct->GPIO_Pin的pinpos位也为1，
		从而可知pinpos就是GPIO_InitStruct->GPIO_Pin对应的引脚号：0-15*/
    currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

		/*currentpin == pos时执行初始化*/
    if (currentpin == pos)
		{		
			/*GPIOx端口，MODER寄存器的GPIO_InitStruct->GPIO_Pin对应的引脚，MODER位清空*/
			GPIOx->MODER  &= ~(3 << (2 *pinpos));
		
			/*GPIOx端口，MODER寄存器的GPIO_Pin引脚，MODER位设置"输入/输出/复用输出/模拟"模式*/
			GPIOx->MODER |= (((uint32_t)GPIO_InitStruct->GPIO_Mode) << (2 *pinpos));

			/*GPIOx端口，PUPDR寄存器的GPIO_Pin引脚，PUPDR位清空*/
			GPIOx->PUPDR &= ~(3 << ((2 *pinpos)));
		
			/*GPIOx端口，PUPDR寄存器的GPIO_Pin引脚，PUPDR位设置"上/下拉"模式*/
			GPIOx->PUPDR |= (((uint32_t)GPIO_InitStruct->GPIO_PuPd) << (2 *pinpos));		
		
			/*若模式为"输出/复用输出"模式，则设置速度与输出类型*/
			if ((GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF))
			{
				/*GPIOx端口，OSPEEDR寄存器的GPIO_Pin引脚，OSPEEDR位清空*/
				GPIOx->OSPEEDR &= ~(3 << (2 *pinpos));
				/*GPIOx端口，OSPEEDR寄存器的GPIO_Pin引脚，OSPEEDR位设置输出速度*/
				GPIOx->OSPEEDR |= ((uint32_t)(GPIO_InitStruct->GPIO_Speed) << (2 *pinpos));

				/*GPIOx端口，OTYPER寄存器的GPIO_Pin引脚，OTYPER位清空*/
				GPIOx->OTYPER  &= ~(1 << (pinpos)) ;
				/*GPIOx端口，OTYPER位寄存器的GPIO_Pin引脚，OTYPER位设置"推挽/开漏"输出类型*/
				GPIOx->OTYPER |= (uint16_t)(((uint16_t)GPIO_InitStruct->GPIO_OType) << (pinpos));
			}
		}
	}
}



/*********************************************END OF FILE**********************/

