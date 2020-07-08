/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SCT文件应用——自动分配变量到外部SRAM实验
	* @note 	 -本工程修改了启动文件和sct文件!!
	*					 -本工程使用手动编写的sct文件控制存储器分配
	*          本实验中的sct配置，堆区工作不正常，使用malloc无法得到正常的地址，不推荐在实际工程应用
  *          另一种我们推荐的配置请参考教程中的说明
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
#include <stdlib.h>


//定义变量到外部SRAM
uint32_t testValue  =7 ;
//定义变量到外部SRAM
uint32_t testValue2  =0;



//定义数组到外部SRAM
uint8_t testGrup[100]  ={0};
//定义数组到外部SRAM
uint8_t testGrup2[100] ={1,2,3};


/*本实验中的sct配置，堆区可能工作不正常，使用malloc无法得到正常的地址，不推荐在实际工程应用*/
/*另一种我们推荐的配置请参考教程中的说明*/

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{ 	
	uint32_t inerTestValue =10;

	
	LED_GPIO_Config();
	LED_BLUE;
	
	/* 配置串口1为：115200 8-N-1 */
	Debug_USART_Config();
  
    //FSMC_SRAM_Init已经在启动文件的Reset_handler中调用，进入main之前已经完成初始化*/
//    FSMC_SRAM_Init();
	
 printf("\r\nSCT文件应用——自动分配变量到外部SRAM实验\r\n");
  
	printf("\r\n使用“	uint32_t inerTestValue =10; ”语句定义的局部变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&inerTestValue,inerTestValue);
	
  printf("\r\n使用“uint32_t testValue  =7 ;”语句定义的全局变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&testValue,testValue);
	
  printf("\r\n使用“uint32_t testValue2  =0 ; ”语句定义的全局变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&testValue2,testValue2);
	
	
	printf("\r\n使用“uint8_t testGrup[100]  ={0};”语句定义的全局数组：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d,%d,%d\r\n",(uint32_t)&testGrup,testGrup[0],testGrup[1],testGrup[2]);
	
  printf("\r\n使用“uint8_t testGrup2[100] ={1,2,3};”语句定义的全局数组：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d，%d,%d\r\n",(uint32_t)&testGrup2,testGrup2[0],testGrup2[1],testGrup2[2]);
	
	
	/*本实验中的sct配置，堆区可能工作不正常，使用malloc无法得到正常的地址，不推荐在实际工程应用*/
  /*另一种我们推荐的配置请参考教程中的说明*/
	
	uint32_t * pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); 
	if(pointer != NULL)
	{
		*(pointer)=1;
		*(++pointer)=2;
		*(++pointer)=3;	
		
		printf("\r\n使用“	uint32_t *pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); ”动态分配的变量\r\n");
		printf("\r\n定义后的操作为：\r\n*(pointer++)=1;\r\n*(pointer++)=2;\r\n*pointer=3;\r\n\r\n");
		printf("结果：操作后它的地址为：0x%x,查看变量值操作：\r\n",(uint32_t)pointer); 
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer)=%d, \r\n",*(pointer));
		
		free(pointer);
	}
	else
	{
		printf("\r\n使用malloc动态分配变量出错！！！\r\n");	
	}

  LED_BLUE; 
	

  while(1); 
}



/*********************************************END OF FILE**********************/

