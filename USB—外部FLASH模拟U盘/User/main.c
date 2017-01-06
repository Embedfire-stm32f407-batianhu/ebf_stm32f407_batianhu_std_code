/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   使用USB接口及板载FLASH模拟U盘
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
#include "./Bsp/led/bsp_led.h" 
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./bsp/flash/bsp_spi_flash.h"

#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"


__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	__IO uint32_t i = 0;
	
	
	/* 初始化LED */
	LED_GPIO_Config();
	
	/* 初始化调试串口，一般为串口1 */
	Debug_USART_Config();

	printf("SPIFlash文件系统模拟U盘测试程序\n");
	
	  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */ 
	
  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc,
            &USBD_MSC_cb, 
            &USR_cb);
	
	while(1)
	{
		if (i++ == 0x1000000)
    {
      LED3_TOGGLE;
      i = 0;
    }    
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
