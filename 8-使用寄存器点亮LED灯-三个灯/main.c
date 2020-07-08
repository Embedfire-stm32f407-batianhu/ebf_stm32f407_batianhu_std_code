
/*
	使用寄存器的方法点亮LED灯
  */
#include "stm32f4xx.h" 


/**
  *   主函数
  */
int main(void)
{	
	/*开启 GPIOF 时钟，使用外设时都要先开启它的时钟*/
	RCC_AHB1ENR |= (1<<5);	
	
	/* LED 端口初始化 */
	
	/*GPIOF MODER6清空*/
	GPIOF_MODER  &= ~( 0x03<< (2*6));	
	/*PF6 MODER6 = 01b 输出模式*/
	GPIOF_MODER |= (1<<2*6);
	
	/*GPIOF OTYPER6清空*/
	GPIOF_OTYPER &= ~(1<<1*6);
	/*PF6 OTYPER6 = 0b 推挽模式*/
	GPIOF_OTYPER |= (0<<1*6);
	
	/*GPIOF OSPEEDR6清空*/
	GPIOF_OSPEEDR &= ~(0x03<<2*6);
	/*PF6 OSPEEDR6 = 0b 速率2MHz*/
	GPIOF_OSPEEDR |= (0<<2*6);
	
	/*GPIOF PUPDR6清空*/
	GPIOF_PUPDR &= ~(0x03<<2*6);
	/*PF6 PUPDR6 = 01b 上拉模式*/
	GPIOF_PUPDR |= (1<<2*6);
	
	/*PF6 BSRR寄存器的 BR6置1，使引脚输出低电平*/
	GPIOF_BSRR |= (1<<16<<6);
	
	/*PF6 BSRR寄存器的 BS6置1，使引脚输出高电平*/
	//GPIOF_BSRR |= (1<<6);

	/*GPIOF MODER7清空*/
	GPIOF_MODER  &= ~( 0x03<< (2*7));	
	/*PF7 MODER7 = 01b 输出模式*/
	GPIOF_MODER |= (1<<2*7);
	
	/*GPIOF OTYPER7清空*/
	GPIOF_OTYPER &= ~(1<<1*7);
	/*PF7 OTYPER7 = 0b 推挽模式*/
	GPIOF_OTYPER |= (0<<1*7);
	
	/*GPIOF OSPEEDR7清空*/
	GPIOF_OSPEEDR &= ~(0x03<<2*7);
	/*PF7 OSPEEDR7 = 0b 速率2MHz*/
	GPIOF_OSPEEDR |= (0<<2*7);
	
	/*GPIOF PUPDR7清空*/
	GPIOF_PUPDR &= ~(0x03<<2*7);
	/*PF7 PUPDR7 = 01b 上拉模式*/
	GPIOF_PUPDR |= (1<<2*7);
	
	/*PF7 BSRR寄存器的 BR7置1，使引脚输出低电平*/
	GPIOF_BSRR |= (1<<16<<7);
	
	/*GPIOF MODER8清空*/
	GPIOF_MODER  &= ~( 0x03<< (2*8));	
	/*PF8 MODER8 = 01b 输出模式*/
	GPIOF_MODER |= (1<<2*8);
	
	/*GPIOF OTYPER8清空*/
	GPIOF_OTYPER &= ~(1<<1*8);
	/*PF8 OTYPER8 = 0b 推挽模式*/
	GPIOF_OTYPER |= (0<<1*8);
	
	/*GPIOF OSPEEDR8清空*/
	GPIOF_OSPEEDR &= ~(0x03<<2*8);
	/*PF7 OSPEEDR8 = 0b 速率2MHz*/
	GPIOF_OSPEEDR |= (0<<2*8);
	
	/*GPIOF PUPDR8清空*/
	GPIOF_PUPDR &= ~(0x03<<2*8);
	/*PF8 PUPDR8 = 01b 上拉模式*/
	GPIOF_PUPDR |= (1<<2*8);
	
	/*PF8 BSRR寄存器的 BR7置1，使引脚输出低电平*/
	GPIOF_BSRR |= (1<<16<<8);
	while(1);

}

// 函数为空，目的是为了骗过编译器不报错
void SystemInit(void)
{	
}






/*********************************************END OF FILE**********************/

