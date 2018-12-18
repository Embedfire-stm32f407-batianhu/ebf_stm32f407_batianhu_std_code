
/*
	自己写库的方法点亮LED灯
  */
#include "stm32f4xx_gpio.h"  

void Delay( uint32_t nCount); 

/**
  *   主函数，使用封装好的函数来控制LED灯
  */
int main(void)
{	
	GPIO_InitTypeDef InitStruct;
	
	/*开启 GPIOF 时钟，使用外设时都要先开启它的时钟*/
	RCC->AHB1ENR |= (1<<5);

	/* LED 端口初始化 */
	
	/*初始化PF6引脚*/
	/*选择要控制的GPIO引脚*/															   
	InitStruct.GPIO_Pin = GPIO_Pin_6;
	/*设置引脚模式为输出模式*/
	InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	/*设置引脚的输出类型为推挽输出*/
	InitStruct.GPIO_OType = GPIO_OType_PP;
	/*设置引脚为上拉模式*/
	InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	/*设置引脚速率为2MHz */   
	InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	GPIO_Init(GPIOF, &InitStruct);	

	/*使引脚输出低电平,点亮LED1*/
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);

	/*延时一段时间*/
	Delay(0xFFFFFF);	
	
	/*使引脚输出高电平，关闭LED1*/
	GPIO_SetBits(GPIOF,GPIO_Pin_6);
	
	/*初始化PF7引脚*/
	InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOF,&InitStruct);
	
	/*使引脚输出低电平，点亮LED2*/
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);

	while(1);

}

//简单的延时函数，让cpu执行无意义指令，消耗时间
//具体延时时间难以计算，以后我们可使用定时器精确延时
void Delay( uint32_t nCount)	 
{
	for(; nCount != 0; nCount--);
}


// 函数为空，目的是为了骗过编译器不报错
void SystemInit(void)
{	
}



///**
//  *   主函数,使用结构体指针，直接控制寄存器的方式来点灯
//			感兴趣可以解除注释来尝试一下
//  */
//int main(void)
//{	
//	/*开启 GPIOF 时钟，使用外设时都要先开启它的时钟*/
//	RCC_AHB1ENR |= (1<<5);	
//	
//	/* LED 端口初始化 */
//	
//	/*GPIOF MODER6清空*/
//	GPIOF_MODER  &= ~( 0x03<< (2*6));	
//	/*PF6 MODER6 = 01b 输出模式*/
//	GPIOF_MODER |= (1<<2*6);
//	
//	/*GPIOF OTYPER6清空*/
//	GPIOF_OTYPER &= ~(1<<1*6);
//	/*PF6 OTYPER6 = 0b 推挽模式*/
//	GPIOF_OTYPER |= (0<<1*6);
//	
//	/*GPIOF OSPEEDR6清空*/
//	GPIOF_OSPEEDR &= ~(0x03<<2*6);
//	/*PF6 OSPEEDR6 = 0b 速率2MHz*/
//	GPIOF_OSPEEDR |= (0<<2*6);
//	
//	/*GPIOF PUPDR6清空*/
//	GPIOF_PUPDR &= ~(0x03<<2*6);
//	/*PF6 PUPDR6 = 01b 上拉模式*/
//	GPIOF_PUPDR |= (1<<2*6);
//	
//	/*PF6 BSRR寄存器的 BR6置1，使引脚输出低电平*/
//	GPIOF_BSRR |= (1<<16<<6);
//	
//	/*PF6 BSRR寄存器的 BS6置1，使引脚输出高电平*/
//	//GPIOF_BSRR |= (1<<6);

//	while(1);

//}


/*********************************************END OF FILE**********************/

