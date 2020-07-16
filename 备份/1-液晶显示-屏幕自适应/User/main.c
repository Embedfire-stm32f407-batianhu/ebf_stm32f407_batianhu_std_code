/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   液晶显示例程
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

#include "./lcd4_5/bsp_ili9806g_lcd.h"
#include "./lcd4_3/bsp_nt35510_lcd.h"

#include "./screen_dev/screen_dev.h"
#include "./font/fonts.h"
static void LCD_Test(void);	
static void Delay ( __IO uint32_t nCount );
void Printf_Charater(void);

TOUCH_PARAM_TypeDef	GT5688 = 
{
	.device_name = "GT5688",
};

TOUCH_PARAM_TypeDef	GT917S = 
{
	.device_name = "GT917S",
};

/* 实例化一个显示器设备,该设备由显示属性、触摸属性、显示驱动组成 */
DEVICE_PARAM_TypeDef NT35510 = 
{
	.my_touchpanel = &GT917S,
	.my_display = &NT35510_DP_Desc,
	.my_disp_driver = &NT35510_DP_Driver,
};

DEVICE_PARAM_TypeDef ILI9806G = 
{
	.my_touchpanel = &GT5688,
	.my_display = &ILI9806G_DP_Desc,
	.my_disp_driver = &ILI9806G_DP_Driver,
};

DEVICE_PARAM_TypeDef *LCD;

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{
  LED_GPIO_Config();

	Debug_USART_Config();		

	LCD = &ILI9806G;
	LCD->my_disp_driver->xx_init_func();
	int Chipid = LCD->my_disp_driver->xx_Read_Chipid();
	switch ( Chipid )
	{
		case 0x9806:
		{
			LCD = &ILI9806G;
		}break;
		case 0:
		{
			LCD = &NT35510;
		}break;
		default:
		{
			LCD = &NT35510;
		}break;
	};

	LCD->my_disp_driver->xx_init_func();
	printf("\r\n ********** 液晶屏英文显示程序*********** \r\n"); 
	printf("\r\n 本程序不支持中文，显示中文的程序请学习下一章 \r\n"); 

	//其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
//  ILI9806G_GramScan ( 6 );

	    
	while ( 1 )
	{
		LCD_Test();
	}
	
	
}




/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;	
	
	LCD->my_disp_driver->xx_SetFont(&Font16x32);
	LCD->my_disp_driver->xx_SetColors(RED,BLACK);

  LCD->my_disp_driver->xx_Clear(0,0,LCD->my_display->LCD_X_LENGTH,LCD->my_display->LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/
	sprintf(dispBuff,"YH %s inch LCD",LCD->my_display->size_info);
  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(0),dispBuff);
	sprintf(dispBuff,"resolution:%dx%dpx",LCD->my_display->dp_max_height,LCD->my_display->dp_max_width);
  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(2),dispBuff);
	sprintf(dispBuff,"LCD driver:%s",LCD->my_display->device_name);
  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(3),dispBuff);
	sprintf(dispBuff,"Touch driver:%s",LCD->my_touchpanel->device_name);
  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(4),dispBuff);
  
	/********显示变量示例*******/
	LCD->my_disp_driver->xx_SetTextColor(GREEN);

	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"Count : %d ",testCNT);
  LCD->my_disp_driver->xx_ClearLine(LINE(7));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	LCD->my_disp_driver->xx_DispStringLine_EN(LINE(7),dispBuff);

	/*******显示图形示例******/
  /* 画直线 */
  
  LCD->my_disp_driver->xx_ClearLine(LINE(7));/* 清除单行文字 */
	LCD->my_disp_driver->xx_SetTextColor(BLUE);

  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(7),"Draw line:");
  
	LCD->my_disp_driver->xx_SetTextColor(RED);
  LCD->my_disp_driver->xx_DrawLine(50,270,420,275);  
  LCD->my_disp_driver->xx_DrawLine(50,300,420,375);
  
	LCD->my_disp_driver->xx_SetTextColor(GREEN);
  LCD->my_disp_driver->xx_DrawLine(50,370,420,475);  
  LCD->my_disp_driver->xx_DrawLine(50,400,420,475);
	
	LCD->my_disp_driver->xx_SetTextColor(BLUE);
  LCD->my_disp_driver->xx_DrawLine(50,420,420,325);  
  LCD->my_disp_driver->xx_DrawLine(50,450,420,395);
  
  Delay(0x2FFFFFF);
  
  LCD->my_disp_driver->xx_Clear(0,32*7,LCD->my_display->LCD_X_LENGTH,LCD->my_display->LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */
  
  
  /*画矩形*/

  LCD->my_disp_driver->xx_ClearLine(LINE(7));	/* 清除单行文字 */
	LCD->my_disp_driver->xx_SetTextColor(BLUE);

  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(7),"Draw Rect:");

	LCD->my_disp_driver->xx_SetTextColor(RED);
  LCD->my_disp_driver->xx_DrawRectangle(50,300,200,100,1);
	
	LCD->my_disp_driver->xx_SetTextColor(GREEN);
  LCD->my_disp_driver->xx_DrawRectangle(100,300,200,120,0);
	
	LCD->my_disp_driver->xx_SetTextColor(BLUE);
  LCD->my_disp_driver->xx_DrawRectangle(250,300,200,150,1);
  
  
  Delay(0x2FFFFFF);
	
	LCD->my_disp_driver->xx_Clear(0,32*7,LCD->my_display->LCD_X_LENGTH,LCD->my_display->LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */

  /* 画圆 */
  LCD->my_disp_driver->xx_ClearLine(LINE(7));	/* 清除单行文字 */
	LCD->my_disp_driver->xx_SetTextColor(BLUE);
	
  LCD->my_disp_driver->xx_DispStringLine_EN(LINE(7),"Draw Cir:");

	LCD->my_disp_driver->xx_SetTextColor(RED);
	LCD->my_disp_driver->xx_DrawCircle(150,400,60,1);

	LCD->my_disp_driver->xx_SetTextColor(GREEN);
	LCD->my_disp_driver->xx_DrawCircle(250,400,60,0);

	LCD->my_disp_driver->xx_SetTextColor(BLUE);
	LCD->my_disp_driver->xx_DrawCircle(350,400,60,1);

  Delay(0x2FFFFFF);
  
  LCD->my_disp_driver->xx_Clear(0,32*7,LCD->my_display->LCD_X_LENGTH,LCD->my_display->LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */

}


/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}


/*********************************************END OF FILE**********************/

