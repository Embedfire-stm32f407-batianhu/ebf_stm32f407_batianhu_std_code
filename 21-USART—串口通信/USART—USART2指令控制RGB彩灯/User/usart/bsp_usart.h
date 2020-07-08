#ifndef __USART_H
#define	__USART_H

#include "stm32f4xx.h"
#include <stdio.h>


//引脚定义
/*******************************************************/
#define USARTx                             USART2

/* 不同的串口挂载的总线不一样，时钟使能函数也不一样，移植时要注意 
 * 串口1和6是      RCC_APB2PeriphClockCmd
 * 串口2/3/4/5/7是 RCC_APB1PeriphClockCmd
 */
#define USARTx_CLK                         RCC_APB1Periph_USART2
#define USARTx_CLOCKCMD                    RCC_APB1PeriphClockCmd
#define USARTx_BAUDRATE                    115200  //串口波特率

#define USARTx_RX_GPIO_PORT                GPIOA
#define USARTx_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define USARTx_RX_PIN                      GPIO_Pin_3
#define USARTx_RX_AF                       GPIO_AF_USART2
#define USARTx_RX_SOURCE                   GPIO_PinSource3

#define USARTx_TX_GPIO_PORT                GPIOA
#define USARTx_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define USARTx_TX_PIN                      GPIO_Pin_2
#define USARTx_TX_AF                       GPIO_AF_USART2
#define USARTx_TX_SOURCE                   GPIO_PinSource2

/************************************************************/

void USARTx_Config(void);

#endif /* __USART_H */
