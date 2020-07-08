/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   给内部FLASH解锁，恢复选项字节至默认值。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
	
	
/*	
【 ！！】操作步骤：

1.跳线帽：把实验板左侧的BOOT0和BOOT1引脚都使用跳帽连接到3.3V
2.使用DAP下载器下载本程序并复位，然后耐心等待板子LED灯由蓝转绿（整个过程在2分钟左右，等待至5分钟还不转绿说明恢复失败）
3.LED灯变为绿灯后，打开其它工程下载程序到STM32的FLASH(如普通的流水灯例程)
4.若还是无法下载，请重试1-3步骤。
5.下载成功后，把BOOT0和BOOT1引脚都使用跳帽连接到GND，
6.复位开发板，观察新的程序是否正常(如流水灯例程时，LED是否正常闪烁)
7.完成。	
*/	
	
	
	
	
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./internalFlash/internalFlash_reset.h"  


void Delay(__IO u32 nCount); 
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	
	Debug_USART_Config();	
	LED_BLUE;
	
	FLASH_INFO("本程序将会被下载到STM32的内部SRAM运行。");
	FLASH_INFO("下载程序前，请确认把实验板的BOOT0和BOOT1引脚都接到3.3V电源处！！");

	FLASH_INFO("\r\n");
	FLASH_INFO("----这是一个STM32芯片内部FLASH解锁程序----"); 
	FLASH_INFO("程序会把芯片的内部FLASH选项字节恢复为默认值"); 
	
	#if 0  //工程调试、演示时使用，正常解除时不需要运行此函数
	WriteProtect_Test(); //修改写保护位，仿真芯片扇区被设置成写保护的的环境
	#endif

	OptionByte_Info();
	
	/*恢复选项字节到默认值，解除保护*/
	if(InternalFlash_Reset()==FLASH_COMPLETE)
	{		
		FLASH_INFO("选项字节恢复成功，请把BOOT0和BOOT1引脚都连接到GND，");
		FLASH_INFO("然后随便找一个普通的程序，下载程序到芯片的内部FLASH进行测试"); 
		LED_GREEN;
	}	
	else
	{			
		FLASH_INFO("选项字节恢复成功失败，请复位重试");
		LED_RED;
	}

	OptionByte_Info();	

	while (1)
	{

	}
}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

