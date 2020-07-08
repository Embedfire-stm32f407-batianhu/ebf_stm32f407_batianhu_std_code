/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   使用RS232串口，中断接收测试，演示接收数据到buff，并进行简单命令处理。
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
#include "./usart/bsp_rs232_usart.h"

uint8_t Rxflag=0;
uint8_t ucTemp;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint8_t ucaRxBuf[256];
	uint16_t usRxCount=0; 
	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  Debug_USART_Config();
 
	/*调用printf函数，因为重定向了fputc，printf的内容会输出到串口*/
	printf("\r\nPrintf方式输出：这是一个串口中断接收回显实验 \r\n");	

	
	/*自定义函数方式*/
	Usart_SendString( RS232_USART, (uint8_t *)"自定义函数输出：这是一个串口中断接收回显实验\n" );
	Usart_SendString( RS232_USART, (uint8_t *)"输入数据并以回车键结束\n" );
	
	/*STM32串口接收到字符后会进入stm32f4xx_it.c文件的中断服务函数，
	*接收该数据，并标记Rxflag标志位。*/

  while(1)
	{	
		/* 
			接收DEBUG_USART口的数据，分析并处理 
			可以将此段代码封装为一个函数，在主程序其它流程调用
		*/
		if(Rxflag)
		{
			if (usRxCount < sizeof(ucaRxBuf))
			{
				ucaRxBuf[usRxCount++] = ucTemp;
			}
			else
			{
				usRxCount = 0;
			}
			
			/* 简单的通信协议，遇到回车换行符认为1个命令帧，可自行加其它判断实现自定义命令 */
			/* 遇到换行字符，认为接收到一个命令 */
			if (ucTemp == 0x0A)	/* 换行字符 */
			{		
				/*检测到有回车字符就把数据返回给上位机*/
				Usart_SendStr_length( RS232_USART, ucaRxBuf, usRxCount );
				//Usart_SendString();
				usRxCount = 0;
			}
			Rxflag=0;
		}
	}	


}



/*********************************************END OF FILE**********************/

