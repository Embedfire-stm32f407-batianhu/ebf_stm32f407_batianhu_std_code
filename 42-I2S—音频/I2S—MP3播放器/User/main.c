/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   WAV播放器
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                              头文件
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "Bsp/usart/bsp_debug_usart.h"
#include "Bsp/systick/bsp_SysTick.h"
#include "Bsp/wm8978/bsp_wm8978.h" 
#include "App/mp3Player.h"
#include "ff.h" 

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
FATFS fs;											/* Work area (file system object) for logical drives */	
/**
  ******************************************************************************
  *                                任务函数
  ******************************************************************************
  */

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	FRESULT result; 
  
	/*  NVIC中断优先级组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	
	/* 初始化调试串口，一般为串口1 */
	Debug_USART_Config();
	
	/* 挂载SD卡文件系统 */
	result = f_mount(&fs,"0:",1);  //挂载文件系统
	if(result!=FR_OK)
	{
		printf("\n SD卡文件系统挂载失败\n");
		while(1);
	}
	
	/* 初始化系统滴答定时器 */
	SysTick_Init();	
	printf("MP3播放器\n");
	
	/* 检测WM8978芯片，此函数会自动配置CPU的GPIO */
	if (wm8978_Init()==0)
	{
		printf("检测不到WM8978芯片!!!\n");
		while (1);	/* 停机 */
	}
	printf("初始化WM8978成功\n");	
	
	while(1)
	{		
		mp3PlayerDemo("0:/mp3/张国荣-玻璃之情.mp3");
		mp3PlayerDemo("0:/mp3/张国荣-全赖有你.mp3");
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
