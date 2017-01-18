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
  * 实验平台:秉火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"  
#include "./sram/sram.h"
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./touch/gt9xx.h"
#include "./touch/palette.h"

static void LCD_Test(void);	
static void Delay ( __IO uint32_t nCount );
void Printf_Charater(void)   ;


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{
    LED_GPIO_Config();
	ILI9806G_Init ();         //LCD 初始化

	Debug_USART_Config();		
	
    /* 初始化触摸屏 */
    GTP_Init_Panel(); 
	
    
    /*调用画板函数*/
//	Palette_Init();
    
	while ( 1 )
	{
		//LCD_Test();
	}
	
	
}




/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;	
	
	LCD_SetFont(&Font24x32);
	LCD_SetColors(RED,BLACK);

  ILI9806G_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/
  ILI9806G_DispStringLine_EN(LINE(0),"BH 4.5 inch LCD para:");
  ILI9806G_DispStringLine_EN(LINE(1),"Image resolution:480x854 px");
  ILI9806G_DispStringLine_EN(LINE(2),"ILI9806G LCD driver");
  ILI9806G_DispStringLine_EN(LINE(3),"GT5688 Touch Pad driver");
  
	/********显示变量示例*******/
	LCD_SetFont(&Font24x32);
	LCD_SetTextColor(GREEN);

	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"Count : %d ",testCNT);
  ILI9806G_ClearLine(LINE(4));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9806G_DispStringLine_EN(LINE(4),dispBuff);

	/*******显示图形示例******/
	LCD_SetFont(&Font24x32);
  /* 画直线 */
  
  ILI9806G_ClearLine(LINE(4));/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  ILI9806G_DispStringLine_EN(LINE(4),"Draw line:");
  
	LCD_SetTextColor(RED);
  ILI9806G_DrawLine(50,170,850,175);  
  ILI9806G_DrawLine(50,200,850,275);
  
	LCD_SetTextColor(GREEN);
  ILI9806G_DrawLine(50,270,850,375);  
  ILI9806G_DrawLine(50,300,850,475);
	
	LCD_SetTextColor(BLUE);
  ILI9806G_DrawLine(50,320,850,225);  
  ILI9806G_DrawLine(50,400,850,395);
  
  Delay(0x2FFFFFF);
  
  ILI9806G_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */
  
  
  /*画矩形*/

  ILI9806G_ClearLine(LINE(4));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  ILI9806G_DispStringLine_EN(LINE(4),"Draw Rect:");

	LCD_SetTextColor(RED);
  ILI9806G_DrawRectangle(50,200,200,100,1);
	
	LCD_SetTextColor(GREEN);
  ILI9806G_DrawRectangle(100,200,200,150,1);
	
	LCD_SetTextColor(BLUE);
  ILI9806G_DrawRectangle(250,200,200,200,1);
  
  
  Delay(0x2FFFFFF);
	
	ILI9806G_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

  /* 画圆 */
  ILI9806G_ClearLine(LINE(4));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);
	
  ILI9806G_DispStringLine_EN(LINE(4),"Draw Cir:");

    LCD_SetTextColor(RED);
    ILI9806G_DrawCircle(150,300,100,1);

    LCD_SetTextColor(GREEN);
    ILI9806G_DrawCircle(250,300,100,1);

    LCD_SetTextColor(BLUE);
    ILI9806G_DrawCircle(350,300,100,1);

  Delay(0x2FFFFFF);
  
  ILI9806G_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

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

