/**
  ******************************************************************************
  * @file    bsp_touchpad.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   电容按键应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "./Bsp/TouchPad/bsp_touchpad.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/systick/bsp_SysTick.h"

//定时器最大计数值
#define TPAD_ARR_MAX_VAL 	0XFFFF	

//保存没按下时定时器计数值
volatile uint16_t tpad_default_val=0;

/***********************************
 *
 * 定时器输入捕获配置
 *
 ***********************************/
static void TIMx_CHx_Cap_Init(uint32_t arr,uint16_t psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	//使能TIM时钟
	RCC_APB1PeriphClockCmd(TPAD_TIM_CLK,ENABLE);  	
	//使能通道引脚时钟	
	RCC_AHB1PeriphClockCmd(TPAD_TIM_GPIO_CLK, ENABLE); 	
	//指定引脚复用
	GPIO_PinAFConfig(TPAD_TIM_CH_PORT,TPAD_TIM_SOURCE,TPAD_TIM_AF); 
	
	//端口配置
	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;	
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//不带上下拉 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
	//初始化TIM
	//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Period = arr;
	//预分频器
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TPAD_TIMx, &TIM_TimeBaseStructure); 
	
	//初始化通道
	//选择定时器输入通道
	TIM_ICInitStructure.TIM_Channel = TPAD_TIM_Channel_X; 
	//上升沿触发
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	// 输入捕获选择
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
	//配置输入滤波器 不滤波
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TPAD_TIMx, &TIM_ICInitStructure);
	
	//使能TIM
	TIM_Cmd ( TPAD_TIMx, ENABLE ); 
}

/****************************************
 *
 * 为电容按键放电
 * 清除定时器标志及计数
 *
 *****************************************/
static void TPAD_Reset(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	//配置引脚为普通推挽输出
 	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
 	GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
	//输出低电平,放电
 	GPIO_ResetBits ( TPAD_TIM_CH_PORT, TPAD_TIM_CH_PIN );						 
  //保持一小段时间低电平，保证放电完全
	Delay_ms(5);
	
	//清除中断标志
	TIM_ClearITPendingBit(TPAD_TIMx, TPAD_TIM_IT_CCx|TIM_IT_Update); 
	//计数器归0
	TIM_SetCounter(TPAD_TIMx,0);	
	
	//引脚配置为复用功能，不上、下拉
	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPAD_TIM_CH_PORT,&GPIO_InitStructure);
}

/****************************************************
 *
 * 得到定时器捕获值
 * 如果超时,则直接返回定时器的计数值.
 *
 *****************************************************/
static uint16_t TPAD_Get_Val(void)
{		
  /* 先放电完全，并复位计数器 */	
	TPAD_Reset();
	//等待捕获上升沿
	while(TIM_GetFlagStatus ( TPAD_TIMx, TPAD_TIM_IT_CCx ) == RESET)
	{
		//超时了,直接返回CNT的值
		if(TIM_GetCounter(TPAD_TIMx)>TPAD_ARR_MAX_VAL-500)
			return TIM_GetCounter(TPAD_TIMx);
	};
	/* 捕获到上升沿后输出TIMx_CCRx寄存器值 */
	return TPAD_TIM_GetCaptureX(TPAD_TIMx );	  
} 	

/****************************************************
 *
 * 读取n次,取最大值
 * n：连续获取的次数
 * 返回值：n次读数里面读到的最大读数值
 * 
 *****************************************************/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp=0;
	uint16_t res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//得到一次值
		if(temp>res)res=temp;
	};
	return res;
}  

/********************************************************
*
* 初始化触摸按键
* 获得空载的时候触摸按键的取值.
* 返回值:0,初始化成功;1,初始化失败
*
*********************************************************/
uint8_t TPAD_Init(void)
{
	uint16_t buf[10];
	uint32_t temp=0;
	uint8_t j,i;
	
	//设定定时器预分频器目标时钟为：4MHz(84Mhz/21)
	TIMx_CHx_Cap_Init(TPAD_ARR_MAX_VAL,21-1);
	for(i=0;i<10;i++)//连续读取10次
	{				 
		buf[i]=TPAD_Get_Val();
		Delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)//排序
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	//取中间的6个数据进行平均
	for(i=2;i<8;i++)
	{
	  temp+=buf[i];
	}
	
	tpad_default_val=temp/6;	
	/* printf打印函数调试使用，用来确定阈值TPAD_GATE_VAL，在应用工程中应注释掉 */
//	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	
	//初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
	{
		return 1;
	}
	
	return 0;		     	    					   
}
 

/*******************************************************************************
*
* 扫描触摸按键
* mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
* 返回值:0,没有按下;1,有按下;			
*
*******************************************************************************/
//阈值：捕获时间必须大于(tpad_default_val + TPAD_GATE_VAL),才认为是有效触摸.
#define TPAD_GATE_VAL 	100	

uint8_t TPAD_Scan(uint8_t mode)
{
	//0,可以开始检测;>0,还不能开始检测	
	static uint8_t keyen=0;
	//扫描结果
	uint8_t res=0;
	//默认采样次数为3次
	uint8_t sample=3;	
  //捕获值	
	uint16_t rval;
	
	if(mode)
	{
		//支持连按的时候，设置采样次数为6次
		sample=6;	
		//支持连按	
		keyen=0;	  
	}	
	/* 获取当前捕获值(返回 sample 次扫描的最大值) */
	rval=TPAD_Get_MaxVal(sample); 	
	/* printf打印函数调试使用，用来确定阈值TPAD_GATE_VAL，在应用工程中应注释掉 */
//	printf("scan_rval=%d\n",rval);
	
	//大于tpad_default_val+TPAD_GATE_VAL,且小于10倍tpad_default_val,则有效
	if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))
	{			
    //keyen==0,有效 		
		if(keyen==0)
		{
			res=1;		 
		}			
		keyen=3;				//至少要再过3次之后才能按键有效   
	}
	
	if(keyen)
	{
		keyen--;		
	}		
	return res;
}	 


/*********************************************END OF FILE**********************/
