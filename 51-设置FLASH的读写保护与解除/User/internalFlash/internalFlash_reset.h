#ifndef __INTERNAL_FLASH_RESET_H
#define	__INTERNAL_FLASH_RESET_H

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"




/*********************************************/
/*ÐÅÏ¢Êä³ö*/
#define FLASH_DEBUG_ON         0

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)



int InternalFlash_Reset(void);
void WriteProtect_Test(void);
void SetPCROP(void);
void OptionByte_Info(void);






#endif /* __INTERNAL_FLASH_RESET_H */

