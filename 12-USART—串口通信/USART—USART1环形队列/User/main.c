/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   串口接发测试，串口接收到数据后马上回传。
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
#include "./usart/rx_data_queue.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  Debug_USART_Config();
	
	rx_queue_init();
	
	/* 发送一个字符串 */
	Usart_SendString( DEBUG_USART,"这是一个串口中断接收回显实验\n");
	printf("欢迎使用野火STM32开发板\n\n\n\n");

  while(1)
	{	
		//获取数据并输出
		//实际应用中可参考pull data的方式获取数据进行处理
		pull_data_from_queue();
	}	
}



/*********************************************END OF FILE**********************/

