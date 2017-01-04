

#include "stm32f4xx.h"


/*GPIO引脚号定义*/
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< 选择Pin0 (1<<0) */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< 选择Pin1 (1<<1)*/
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< 选择Pin2 (1<<2)*/
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< 选择Pin3 (1<<3)*/
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< 选择Pin4 */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< 选择Pin5 */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< 选择Pin6 */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< 选择Pin7 */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< 选择Pin8 */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< 选择Pin9 */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< 选择Pin10 */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< 选择Pin11 */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< 选择Pin12 */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< 选择Pin13 */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< 选择Pin14 */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< 选择Pin15 */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< 选择全部引脚 */




/** 
  * GPIO端口配置模式的枚举定义
  */   
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< 输入模式 */
  GPIO_Mode_OUT  = 0x01, /*!< 输出模式 */
  GPIO_Mode_AF   = 0x02, /*!< 复用模式 */
  GPIO_Mode_AN   = 0x03  /*!< 模拟模式 */
}GPIOMode_TypeDef;

/** 
  * GPIO输出类型枚举定义
  */  
typedef enum
{ 
  GPIO_OType_PP = 0x00,	/*!< 推挽模式 */
  GPIO_OType_OD = 0x01	/*!< 开漏模式 */
}GPIOOType_TypeDef;


/** 
  * GPIO输出速率枚举定义
  */  
typedef enum
{ 
  GPIO_Speed_2MHz   = 0x00, /*!< 2MHz   */
  GPIO_Speed_25MHz  = 0x01, /*!< 25MHz  */
  GPIO_Speed_50MHz  = 0x02, /*!< 50MHz  */
  GPIO_Speed_100MHz = 0x03  /*!<100MHz  */
}GPIOSpeed_TypeDef;


  

/** 
  *GPIO上/下拉配置枚举定义
  */ 
typedef enum
{ 
  GPIO_PuPd_NOPULL = 0x00,/*浮空*/
  GPIO_PuPd_UP     = 0x01, /*上拉*/
  GPIO_PuPd_DOWN   = 0x02  /*下拉*/
}GPIOPuPd_TypeDef;



/** 
  * GPIO初始化结构体类型定义
  */ 
typedef struct 
{
  uint32_t GPIO_Pin;              /*!< 选择要配置的GPIO引脚
                                        可输入 GPIO_Pin_ 定义的宏 */

  GPIOMode_TypeDef GPIO_Mode;     /*!< 选择GPIO引脚的工作模式
                                       可输入 GPIOMode_TypeDef 定义的枚举值*/

  GPIOSpeed_TypeDef GPIO_Speed;   /*!< 选择GPIO引脚的速率
                                       可输入 GPIOSpeed_TypeDef 定义的枚举值 */

  GPIOOType_TypeDef GPIO_OType;   /*!< 选择GPIO引脚输出类型
                                       可输入 GPIOOType_TypeDef 定义的枚举值*/

  GPIOPuPd_TypeDef GPIO_PuPd;     /*!<选择GPIO引脚的上/下拉模式
                                       可输入 GPIOPuPd_TypeDef 定义的枚举值*/
}GPIO_InitTypeDef;



void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);


