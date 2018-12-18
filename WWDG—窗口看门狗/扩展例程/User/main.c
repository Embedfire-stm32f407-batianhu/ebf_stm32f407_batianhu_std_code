/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   窗口看门狗例程
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
#include "./led/bsp_led.h"
#include "./key/bsp_key.h" 
#include "./wwdg/bsp_wwdg.h"
#include "./systick/bsp_SysTick.h"

/*简单任务管理*/
uint32_t Task_Delay[NumOfTask]={40};

/*停止喂狗标志，按下KEY1会使这个标志置1，systick停止喂狗*/
uint8_t stop_feed = 0;

static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	/*初始化systick*/
	SysTick_Init();

	/* 检查是否为窗口看门狗复位 */
  if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
  {
		/*停止喂狗后，触发复位，会进入这个环节，亮红灯*/	

    /* 窗口看门狗复位 */
    /*  亮红灯 */
    LED_RED;

    /* 清除标志 */
    RCC_ClearFlag();		
		
		/*延时，方便复位后看到红灯亮*/
		Delay(0XFFFFFF);	
  }
  else
  {
    /*不是窗口看门狗复位(可能为上电复位之类的) */
    /* 亮绿灯 */
    LED_GREEN;
  }	
	
  
  /*初始化按键*/
  Key_GPIO_Config();	
	
	// WWDG配置
	
	/* WWDG 配置函数
	 * tr ：递减计时器的值， 取值范围为：0x7f~0x40，超出范围会直接复位
	 * wr ：窗口值，取值范围为：0x7f~0x40
	 * prv：预分频器值，取值可以是
	 *      @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(42MHz)/4096)/1  约10253Hz 97us
	 *      @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(42MHz)/4096)/2	 约5126Hz	195us
	 *      @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(42MHz)/4096)/4	 约2563Hz	390us
	 *      @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(42MHz)/4096)/8  约1281Hz	780us
	 *      
	 *			例：tr = 127(0x7f，tr的最大值)  wr = 80（0x50, 0x40为最小wr最小值）  prv =  WWDG_Prescaler_8
	 *			~780 * (127-80) = 36.6ms < 刷新窗口 < ~780 * 64 = 49.9ms
	 *			也就是说调用WWDG_Config进行这样的配置，若在之后的36.6ms前喂狗，系统会复位，在49.9ms后没有喂狗，系统也会复位。
	 *			需要在刷新窗口的时间内喂狗，系统才不会复位。	
	*/	
	WWDG_Config(127,80,WWDG_Prescaler_8);	
	/*设置下面的喂狗任务在40ms后开始*/
 	Task_Delay[0]=40;

	while(1)                            
	{		
		//-----------------------------------------------------
		// 这部分应该写需要被WWDG监控的程序，这段程序运行的时间
		// 决定了前面WWDG_Config函数窗口值及递减计数器应该设置成多大。
		//-----------------------------------------------------
		
		/*按下按键后，将停止喂狗*/
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
		{
			/*设置标志位，用于指示不要喂狗*/
			stop_feed = 1;		
			
			/*若不喂狗，会产生复位，在窗口看门狗复位前，会进入窗口看门狗中断，处理复位前的紧急事务*/

		}  
		
		/*定时喂狗*/
		if(Task_Delay[0]==0)
		{						
			if(stop_feed != 1)
			{
				// 喂狗，如果不喂狗，系统则会复位，复位后亮红灯，
				//如果在窗口时间内准时喂狗的话，则会亮绿灯
				WWDG_Feed();		
				//喂狗后亮绿灯
				LED_GREEN;
			}		
			Task_Delay[0]=40; //此值每1ms会减1，减到0才可以重新进来这里
		}   

	}
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

