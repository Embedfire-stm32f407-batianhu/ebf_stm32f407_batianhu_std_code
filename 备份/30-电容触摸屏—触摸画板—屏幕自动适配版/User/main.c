/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   电容触摸屏-触摸画板
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

#include "screen_dev.h"

#include "./touch/gt9xx.h"
#include "./lcd/bsp_nt35510_lcd.h"
#include "./LCD_4_5/lcd/bsp_ili9806g_lcd.h"

#include "./touch/palette.h"


/* 实例化一个显示器设备,该设备由显示属性、触摸属性、显示驱动实例化 */
DEVICE_PARAM_TypeDef NT35510 = 
{
	.my_touchpanel = &GT917S_TP_Desc,
	.my_display = &NT35510_DP_Desc,
	.my_disp_driver = &NT35510_DP_Driver,
};

DEVICE_PARAM_TypeDef ILI9806G = 
{
	.my_touchpanel = &GT5688_TP_Desc,
	.my_display = &ILI9806G_DP_Desc,
	.my_disp_driver = &ILI9806G_DP_Driver,
};
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{
	LED_GPIO_Config();

	Debug_USART_Config();	

	
	//其中0、3、5、6 模式适合从左至右显示文字，
	//不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	//其中 6 模式为大部分液晶例程的默认显示方向  

	/* 先初始化触摸屏，触摸屏会根据液晶的扫描方向输出匹配的触摸坐标 */
	/* 每次修改液晶扫描方向后，应重新调用一次GTP_Init_Panel函数更新触摸配置 */
	/* 初始化完成后，会返回当前触摸芯片类型，根据类型使用对应的显示屏驱动 */
	switch ( GTP_Init_Panel())
	{
		case GT5688:
		{
			LCD = &ILI9806G;
		}break;
		case GT917S:
		{
			LCD = &NT35510;
		}break;
		default:
		{
			LCD = &ILI9806G;
		}break;
	};
	
	/* 初始化显示屏 */
	LCD->my_disp_driver->xx_init_func();
	printf("\r\n ********** 触摸画板程序 *********** \r\n"); 
	printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 
 
	//绘制触摸画板界面
	Palette_Init();
    
	while ( 1 )
	{
	}
	
	
}





/*********************************************END OF FILE**********************/

