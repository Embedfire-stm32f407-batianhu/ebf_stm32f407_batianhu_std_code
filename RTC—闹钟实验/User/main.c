/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RTC例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_debug_usart.h"
#include "./RTC/bsp_rtc.h"
#include "./beep/bsp_beep.h"

#ifdef USE_LCD_DISPLAY
	#include "./lcd/bsp_ili9806g_lcd.h"
#endif

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{  
	/* 初始化LED */
	LED_GPIO_Config();
	BEEP_GPIO_Config();

	/* 初始化调试串口，一般为串口1 */
	Debug_USART_Config();
	printf("\n\r这是一个RTC闹钟实验 \r\n");

#ifdef USE_LCD_DISPLAY	
	/*=========================液晶初始化开始===============================*/
	ILI9806G_Init ();         //LCD 初始化

	//其中0、3、5、6 模式适合从左至右显示文字，
	//不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	//其中 6 模式为大部分液晶例程的默认显示方向  
  ILI9806G_GramScan ( 6 );
  /*=========================液晶初始化结束===============================*/
#endif	
	/*
	 * 当我们配置过RTC时间之后就往备份寄存器0写入一个数据做标记
	 * 所以每次程序重新运行的时候就通过检测备份寄存器0的值来判断
	 * RTC 是否已经配置过，如果配置过那就继续运行，如果没有配置过
	 * 就初始化RTC，配置RTC的时间。
	 */
  
  /* RTC配置：选择时钟源，设置RTC_CLK的分频系数 */
  RTC_CLK_Config();
   
  if (RTC_ReadBackupRegister(RTC_BKP_DRX) != 0xf)//RTC_BKP_DATA
  {		
		/* 闹钟设置 */
		RTC_AlarmSet();
		
    /* 设置时间和日期 */
		RTC_TimeAndDate_Set();
		

  }
  else
  {
    /* 检查是否电源复位 */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n 发生电源复位....\r\n");
    }
    /* 检查是否外部复位 */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n 发生外部复位....\r\n");
    }

    printf("\r\n 不需要重新配置RTC....\r\n");
    
    /* 使能 PWR 时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
    PWR_BackupAccessCmd(ENABLE);
    /* 等待 RTC APB 寄存器同步 */
    RTC_WaitForSynchro();

    /* 清除RTC中断标志位 */
    RTC_ClearFlag(RTC_FLAG_ALRAF);
    /* 清除 EXTI Line 17 悬起位 (内部连接到RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);		
  } 
	
	/* 显示时间和日期 */
	RTC_TimeAndDate_Show();
}


/**********************************END OF FILE*************************************/
