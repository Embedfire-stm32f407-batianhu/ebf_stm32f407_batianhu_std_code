/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ADC采集电压，DMA模式
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
#include "./adc/bsp_adc.h"


// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal; 

static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{

    /*初始化USART1*/
    Debug_USART_Config();
 
		/* 初始化滑动变阻器用到的DAC，ADC数据采集完成后直接由DMA运输数据到ADC_ConvertedValue变量
				DMA直接改变ADC_ConvertedValue的值*/
    Rheostat_Init();
  
    printf("\r\n ----这是一个光敏电阻电压采集实验(DMA传输)----\r\n");
    
    while (1)
    {
      ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*(float)3.3; // 读取转换的AD值
    
      printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
      printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 

      Delay(0xffffee);  
    }
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

