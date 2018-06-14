/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   读写EEPROM
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
#include "./usart/bsp_debug_usart.h"
#include "./i2c/bsP_i2c_ee.h"
#include "./led/bsp_led.h"




uint8_t cal_flag = 0;
uint8_t k;

/*存储小数和整数的数组，各7个*/
long double double_buffer[7] = {0};
int int_bufffer[7] = {0};

#define DOUBLE_ADDR       10
#define LONGINT_ADDR      70



/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  LED_GPIO_Config();
	
	LED_BLUE;
  /*初始化USART1*/
  Debug_USART_Config(); 

	printf("\r\n 这是一个EEPROM 读写小数和长整数实验 \r\n");

	/* I2C 外设初(AT24C02)始化 */
	I2C_EE_Init();
 	 
	/*读取数据标志位*/
  I2C_EE_BufferRead(&cal_flag, 0, 1);
  
	 if( cal_flag != 0xAB )	/*若标志等于0xab，表示之前已有写入数据*/
    {      
        printf("\r\n没有检测到数据标志，EEPROM没有存储数据，即将进行小数写入实验\r\n");
        cal_flag =0xAB;
        
        /*写入标志到0地址*/
        I2C_EE_BufferWrite(&cal_flag, 0, 1); 
        
        /*生成要写入的数据*/
        for( k=0; k<7; k++ )
        {
           double_buffer[k] = k +0.1;
           int_bufffer[k]=k*500+1 ;
        }

        /*写入小数数据到地址10*/
        I2C_EE_BufferWrite((void*)double_buffer,DOUBLE_ADDR, sizeof(double_buffer));
        /*写入整数数据到地址60*/
        I2C_EE_BufferWrite((void*)int_bufffer, LONGINT_ADDR, sizeof(int_bufffer));
              
        printf("向芯片写入数据：");
        /*打印到串口*/
        for( k=0; k<7; k++ )
        {
          printf("小数tx = %LF\r\n",double_buffer[k]);
          printf("整数tx = %d\r\n",int_bufffer[k]);
        }
        
        printf("\r\n请复位开发板，以读取数据进行检验\r\n");      
		
    }    
    else
    {      
      	 printf("\r\n检测到数据标志\r\n");

				/*读取小数数据*/
        I2C_EE_BufferRead((void*)double_buffer, DOUBLE_ADDR, sizeof(double_buffer));
				
				/*读取整数数据*/
        I2C_EE_BufferRead((void*)int_bufffer, LONGINT_ADDR, sizeof(int_bufffer));
	
			
				printf("\r\n从芯片读到数据：\r\n");			
        for( k=0; k<7; k++ )
				{
					printf("小数 rx = %LF \r\n",double_buffer[k]);
					printf("整数 rx = %d \r\n",int_bufffer[k]);				
				}
      
    }   
		
  while (1)
  {      
  }  

}




/*********************************************END OF FILE**********************/

