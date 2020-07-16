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
#include <stdio.h>
#include <string.h>

static void LCD_Test(void);	
static void Delay ( __IO uint32_t nCount );

void Printf_Charater(void)   ;

/* 实例化触摸设备，本工程未用到，只初始化名称 */
TOUCH_PARAM_TypeDef	GT5688 = 
{
	.device_name = "GT5688",
};

TOUCH_PARAM_TypeDef	GT917S = 
{
	.device_name = "GT917S",
};
/* 实例化一个显示器设备,该设备由显示属性、触摸属性、显示驱动实例化 */
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


int main(void)
{	
	//LCD 初始化
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
	
	/* USART config */
	Debug_USART_Config();		

	
	printf("\r\n ********** 液晶屏中文显示程序（字库在外部FLASH）*********** \r\n"); 
	printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 

 //其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
//	ILI9806G_GramScan ( 6 );
//	ILI9806G_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */

	Printf_Charater();
	
	while ( 1 )
	{
		LCD_Test();
	}
	
	
}



/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testIntCNT = 0;	
	static double testFloatCNT = 0;	
	char dispBuff[100];
	char *pStr = 0;
	
	testIntCNT++;	
	testFloatCNT += 0.1;
	
	LCD->my_disp_driver->xx_SetFont(&Font16x32);	
	LCD->my_disp_driver->xx_SetColors(RED,BLACK);
	

	/********显示变量示例*******/
	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"显示整型变量： %d ",testIntCNT);
  LCD->my_disp_driver->xx_ClearLine(LINE(5));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9806G_DispStringLine_EN_CH(LINE(5),dispBuff);
	
	
	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"显示浮点型变量： %f ",testFloatCNT);
  LCD->my_disp_driver->xx_ClearLine(LINE(6));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(6),dispBuff);


	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"浮点型(保留2位小数)： %.2f ",testFloatCNT);
  LCD->my_disp_driver->xx_ClearLine(LINE(7));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(7),dispBuff);
	
	/********居中显示示例*******/
	LCD->my_disp_driver->xx_SetTextColor(GREEN);

	pStr = "插入2个英文空格示例";
	//使用 %*c 在字符串前插入指定个数的英文空格
	sprintf(dispBuff,"%*c%s",2,' ',pStr);
	
	LCD->my_disp_driver->xx_ClearLine(LINE(9));												// 清除单行文字 
	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(9),dispBuff);//显示格式化后的字符串

	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(11),"纯英文24x32居中示例：");
	LCD->my_disp_driver->xx_SetFont(&Font24x48);	
	pStr = "ABCDEF";
	//居中时，要插入的空格个数 = (液晶宽度/单个字体宽度 - 字符串长度)/2
	sprintf(dispBuff,"%*c%s",((LCD->my_display->LCD_X_LENGTH/(((sFONT *)LCD->my_disp_driver->xx_GetFont())->Width))-strlen(pStr))/2,' ',pStr);
 
	LCD->my_disp_driver->xx_ClearLine(LINE(8));													// 清除单行文字 
	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(8),dispBuff);//显示格式化后的字符串
	
	LCD->my_disp_driver->xx_SetFont(&Font16x32);	
	pStr = "中文居中示例";
	//居中时，要插入的空格个数 = (液晶宽度/字体宽度 - 字符串长度)/2
	//strlen计算长度时，一个中文等于2个字节，即2个英文字符，而且插入的是英文空格
	//所以用(WIDTH_CH_CHAR/2)来计算字体宽度
	sprintf(dispBuff,"%*c%s",(LCD->my_display->LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(pStr))/2,' ',pStr);
	
	LCD->my_disp_driver->xx_ClearLine(LINE(15));													// 清除单行文字 
	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(15),dispBuff);	//显示格式化后的字符串
	
	
	pStr = "含英文居中示例ABCDEFG";
	//居中时，要插入的空格个数 = (液晶宽度/字体宽度 - 字符串长度)/2
	//strlen计算长度时，一个中文等于2个字节，即2个英文字符，而且插入的是英文空格
	//所以用(WIDTH_CH_CHAR/2)来计算字体宽度
	sprintf(dispBuff,"%*c%s",(LCD->my_display->LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(pStr))/2,' ',pStr);
 
	LCD->my_disp_driver->xx_ClearLine(LINE(16));												// 清除单行文字 
	LCD->my_disp_driver->xx_DispStringLine_EN_CH(LINE(16),dispBuff);//显示格式化后的字符串
	
	Delay(0x4ffffff);
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



/*"当"字符的字模16x16 */ 	  
unsigned char charater_matrix[] =    
{ /*"当",0*/
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

};   
  
/**
  * @brief  使用串口在上位机打印字模
	*					演示字模显示原理
  * @retval 无
  */
void Printf_Charater(void)   
{   
	int i,j;   
	unsigned char kk; 
  
	/*i用作行计数*/
  for ( i=0;i<16;i++)   
	{   
		/*j用作一字节内数据的移位计数*/
		/*一行像素的第一个字节*/
    for(j=0; j<8; j++)   
    {      
			/*一个数据位一个数据位地处理*/
      kk = charater_matrix[2*i] << j ;  //左移J位          
			if( kk & 0x80)     
			{   
				printf("*"); //如果最高位为1，输出*号，表示笔迹
			}   
			else  
			{   
				printf(" "); //如果最高位为0，输出空格，表示空白  
			}   
    }   
		/*一行像素的第二个字节*/
    for(j=0; j<8; j++)   
		{              
      kk = charater_matrix[2*i+1] << j ;  //左移J位   
       
			if( kk & 0x80)         
			{   
				printf("*"); //如果最高位为1，输出*号，表示笔迹   
			}   
			else  
			{   
				printf(" "); //如果最高位为0，输出空格，表示空白 
			}   
		}         
    printf("\n");    //输出完一行像素，换行 
	}   
	printf("\n\n"); 		//一个字输出完毕
}   


/*********************************************END OF FILE**********************/

