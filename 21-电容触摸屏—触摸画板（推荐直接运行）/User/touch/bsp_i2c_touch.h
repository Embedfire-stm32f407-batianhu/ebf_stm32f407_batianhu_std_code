#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "stm32f4xx.h"

/*设定使用的电容屏IIC设备地址*/
#define GTP_ADDRESS            0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*I2C引脚*/
#define GTP_I2C_SCL_PIN                  GPIO_Pin_7                 
#define GTP_I2C_SCL_GPIO_PORT            GPIOD                       
#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define GTP_I2C_SCL_SOURCE               GPIO_PinSource7

#define GTP_I2C_SDA_PIN                  GPIO_Pin_3                 
#define GTP_I2C_SDA_GPIO_PORT            GPIOD                     
#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define GTP_I2C_SDA_SOURCE               GPIO_PinSource3

/*复位引脚*/
#define GTP_RST_GPIO_PORT                GPIOD
#define GTP_RST_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GTP_RST_GPIO_PIN                 GPIO_Pin_6
/*中断引脚*/
#define GTP_INT_GPIO_PORT                GPIOG
#define GTP_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOG
#define GTP_INT_GPIO_PIN                 GPIO_Pin_8
#define GTP_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOG
#define GTP_INT_EXTI_PINSOURCE           EXTI_PinSource8
#define GTP_INT_EXTI_LINE                EXTI_Line8
#define GTP_INT_EXTI_IRQ                 EXTI9_5_IRQn
/*中断服务函数*/
#define GTP_IRQHandler                   EXTI9_5_IRQHandler


//软件IIC使用的宏
#define I2C_SCL_1()  GPIO_SetBits(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN)		/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)		/* SDA = 0 */

#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)	/* 读SDA口线状态 */

//函数接口
void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void I2C_ResetChip(void);
void I2C_GTP_IRQDisable(void);
void I2C_GTP_IRQEnable(void);

#endif /* __I2C_TOUCH_H */
