#ifndef __WWDG_H
#define	__WWDG_H

#include "stm32f4xx.h"


void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv);
void WWDG_Feed(void);



#endif /* __WWDG_H */

