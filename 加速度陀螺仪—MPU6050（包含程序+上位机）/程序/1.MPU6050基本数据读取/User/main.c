/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   陀螺仪基本数据读取
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./i2c/i2c.h"
#include "./mpu6050/mpu6050.h"


//设置是否使用LCD进行显示，不需要的话把这个宏注释掉即可
//#define USE_LCD_DISPLAY

#ifdef USE_LCD_DISPLAY
 #include "./lcd/bsp_lcd.h"
#endif

/*简单任务管理*/
uint32_t Task_Delay[NumOfTask]={0};

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
	short Acel[3];
	short Gyro[3];
	float Temp;
	
	SysTick_Init();
	
  LED_GPIO_Config();
	
	
#ifdef USE_LCD_DISPLAY	
		 /*初始化液晶屏*/
		LCD_Init();
		LCD_LayerInit();
		LTDC_Cmd(ENABLE);
		
		/*把背景层刷黑色*/
		LCD_SetLayer(LCD_BACKGROUND_LAYER);  
		LCD_Clear(LCD_COLOR_BLACK);
		
		/*初始化后默认使用前景层*/
		LCD_SetLayer(LCD_FOREGROUND_LAYER); 
		/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
		LCD_SetTransparency(0xFF);
		LCD_Clear(LCD_COLOR_BLACK);
		
			/*设置字体颜色及字体的背景颜色*/
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);	

		/*英文字体*/
		LCD_SetFont(&Font16x24); 	
		
		LCD_DisplayStringLine(LINE(1),(uint8_t* )"           This is a MPU6050 demo       ");			
		LCD_DisplayStringLine(LINE(2),(uint8_t* )"                 (Base Version)       ");		
#endif
	
	
  /*初始化USART1*/
  Debug_USART_Config(); 
		
	//初始化 I2C
	I2cMaster_Init(); 

	printf("\r\n 欢迎使用秉火  STM32 F429 开发板。\r\n");		 

	printf("\r\n 这是一个I2C外设(AT24C02)读写测试例程 \r\n");

 	 	//MPU6050初始化
	MPU6050_Init();
	
	//检测MPU6050
	if (MPU6050ReadID() == 1)
	{	
		while(1)
		{
			if(Task_Delay[0]==TASK_ENABLE)
			{
				LED2_TOGGLE;
				Task_Delay[0]=1000;
			}
			
			if(Task_Delay[1]==0)
			{
				MPU6050ReadAcc(Acel);
				printf("加速度：%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
				MPU6050ReadGyro(Gyro);
				printf("    陀螺仪%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
				MPU6050_ReturnTemp(&Temp);
				printf("    温度%8.2f\r\n",Temp);				
				
				
				#ifdef USE_LCD_DISPLAY	
					{
						char cStr [ 70 ];
						sprintf ( cStr, "Acceleration:%8d%8d%8d",Acel[0],Acel[1],Acel[2] );	//加速度原始数据


						LCD_DisplayStringLine(LINE(7),(uint8_t* )cStr);			

						sprintf ( cStr, "Gyro        :%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2] );	//角原始数据

						LCD_DisplayStringLine(LINE(8),(uint8_t* )cStr);			

						sprintf ( cStr, "Temperture  :%8.2f",Temp );	//温度值
						LCD_DisplayStringLine(LINE(9),(uint8_t* )cStr);			

					}
				#endif
				
				Task_Delay[1]=500; //更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
				
			}

			//*************************************	下面是增加任务的格式************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}

	}
	else
	{
			printf("\r\n没有检测到MPU6050传感器！\r\n");
			LED_RED; 
			#ifdef USE_LCD_DISPLAY			
				/*设置字体颜色及字体的背景颜色*/
				LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);	

				LCD_DisplayStringLine(LINE(4),(uint8_t* )"No MPU6050 detected! ");			//野火自带的16*24显示
				LCD_DisplayStringLine(LINE(5),(uint8_t* )"Please check the hardware connection! ");			//野火自带的16*24显示

			#endif
		while(1);	
	}


}




/*********************************************END OF FILE**********************/

