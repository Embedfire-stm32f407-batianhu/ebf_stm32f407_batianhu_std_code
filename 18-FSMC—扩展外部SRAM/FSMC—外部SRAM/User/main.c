/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SRAM应用例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"  
#include "./sram/bsp_sram.h"	  
 

  
/*绝对定位方式访问SRAM,这种方式必须定义成全局变量*/
uint8_t testValue __attribute__((at(Bank1_SRAM4_ADDR)));
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{ 	
	LED_GPIO_Config();
	LED_BLUE;
	
	/* 配置串口1为：115200 8-N-1 */
	Debug_USART_Config();
  
    //初始化外部SRAM  
    FSMC_SRAM_Init();
	
	printf ( "\r\n野火外部 SRAM 测试\r\n" );
	
  /*蓝灯亮，表示正在读写SRAM测试*/
  LED_BLUE;
  
  /*对SRAM进行读写测试，检测SRAM是否正常*/
  if(SRAM_Test()==1)
  {
		//测试正常 绿灯亮
    LED_GREEN;			  
  }
	else
	{
		//测试失败 红灯亮
		LED_RED;
	}
	
	/*指针方式访问SRAM*/
	{	
	 uint32_t temp;
	
	 printf("\r\n指针方式访问SRAM\r\n");
	/*向SRAM写入8位数据*/
	 *( uint8_t*) (Bank1_SRAM4_ADDR ) = (uint8_t)0xAA;
	 printf("\r\n指针访问SRAM，写入数据0xAA \r\n");

	 /*从SRAM读取数据*/
	 temp =  *( uint8_t*) (Bank1_SRAM4_ADDR );
	 printf("读取数据：0x%X \r\n",temp);

	 /*写/读 16位数据*/
	 *( uint16_t*) (Bank1_SRAM4_ADDR+10 ) = (uint16_t)0xBBBB;
	 printf("指针访问SRAM，写入数据0xBBBB \r\n");
	 
	 temp =  *( uint16_t*) (Bank1_SRAM4_ADDR+10 );
	 printf("读取数据：0x%X \r\n",temp);


	 /*写/读 32位数据*/
	 *( uint32_t*) (Bank1_SRAM4_ADDR+20 ) = (uint32_t)0xCCCCCCCC;
	 printf("指针访问SRAM，写入数据0xCCCCCCCC \r\n");	 
	 temp =  *( uint32_t*) (Bank1_SRAM4_ADDR+20 );
	 printf("读取数据：0x%X \r\n",temp);

	}
	
	/*绝对定位方式访问SRAM,这种方式必须定义成全局变量*/
	{
		testValue = 0xDD;
		printf("\r\n绝对定位访问SRAM，写入数据0xDD,读出数据0x%X,变量地址为%X\r\n",testValue,(uint32_t )&testValue);	 
	}
    while(1)
    {

    }
}



void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/*********************************************END OF FILE**********************/

