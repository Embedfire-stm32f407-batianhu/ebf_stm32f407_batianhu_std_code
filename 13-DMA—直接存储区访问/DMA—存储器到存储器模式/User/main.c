/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DMA 存储器到存储器模式
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

/* 相关宏定义，使用存储器到存储器传输必须使用DMA2 */
#define DMA_STREAM               DMA2_Stream0
#define DMA_CHANNEL              DMA_Channel_0
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define DMA_FLAG_TCIF            DMA_FLAG_TCIF0

#define BUFFER_SIZE              32
#define TIMEOUT_MAX              10000 /* Maximum timeout value */

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* 定义DMA传输目标存储器 */
uint32_t aDST_Buffer[BUFFER_SIZE];
                                    
                              
static void Delay(__IO uint32_t nCount);
static void DMA_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  /* 定义存放比较结果变量 */
  uint8_t TransferStatus;
  
	/* LED 端口初始化 */
	LED_GPIO_Config();
    
  /* 设置RGB彩色灯为紫色 */
  LED_PURPLE;  
  
  /* 简单延时函数 */
  Delay(0xFFFFFF);  
  
  /* DMA传输配置 */
  DMA_Config(); 
  
  /* 等待DMA传输完成 */
  while(DMA_GetFlagStatus(DMA_STREAM,DMA_FLAG_TCIF)==DISABLE)
  {
    
  }   
  
  /* 比较源数据与传输后数据 */
  TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
  
  /* 判断源数据与传输后数据比较结果*/
  if(TransferStatus==0)  
  {
    /* 源数据与传输后数据不相等时RGB彩色灯显示红色 */
    LED_RED;
  }
  else
  { 
    /* 源数据与传输后数据相等时RGB彩色灯显示蓝色 */
    LED_BLUE;
  }

	while (1)
	{		
	}
}

/* 简单的延时函数 */
static void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

/**
  * DMA传输配置
  */
static void DMA_Config(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
  __IO uint32_t    Timeout = TIMEOUT_MAX;
    
  /* 使能DMA时钟 */
  RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
  
  /* 复位初始化DMA数据流 */
  DMA_DeInit(DMA_STREAM);

  /* 确保DMA数据流复位完成 */
  while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE)
  {
  }
  
  /* DMA数据流通道选择 */
  DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  
  /* 源数据地址 */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
  /* 目标地址 */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aDST_Buffer;
  /* 存储器到存储器模式 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
  /* 数据数目 */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
  /* 使能自动递增功能 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  /* 使能自动递增功能 */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  /* 源数据是字大小(32位) */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  /* 目标数据也是字大小(32位) */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  /* 一次传输模式，存储器到存储器模式不能使用循环传输 */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  /* DMA数据流优先级为高 */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  /* 禁用FIFO模式 */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;     
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  /* 单次模式 */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  /* 单次模式 */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  /* 完成DMA数据流参数配置 */
  DMA_Init(DMA_STREAM, &DMA_InitStructure);
  
  /* 清除DMA数据流传输完成标志位 */
  DMA_ClearFlag(DMA_STREAM,DMA_FLAG_TCIF);
  
  /* 使能DMA数据流，开始DMA数据传输 */
  DMA_Cmd(DMA_STREAM, ENABLE);

  /* 检测DMA数据流是否有效并带有超时检测功能 */
  Timeout = TIMEOUT_MAX;
  while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0))
  {
  }
   
  /* 判断是否超时 */
  if (Timeout == 0)
  {
    /* 超时就让程序运行下面循环：RGB彩色灯闪烁 */
    while (1)
    {      
      LED_RED;
      Delay(0xFFFFFF);
      LED_RGBOFF;
      Delay(0xFFFFFF);
    }
  } 
}

/**
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}

/*********************************************END OF FILE**********************/

