
/*片上外设基地址  */
#define PERIPH_BASE           ((unsigned int)0x40000000)                          

/*总线基地址 */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)	

/*GPIO外设基地址*/
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)


/* GPIOH寄存器地址,强制转换成指针 */
#define GPIOF_MODER				*(unsigned int*)(GPIOF_BASE+0x00)
#define GPIOF_OTYPER			*(unsigned int*)(GPIOF_BASE+0x04)
#define GPIOF_OSPEEDR			*(unsigned int*)(GPIOF_BASE+0x08)
#define GPIOF_PUPDR				*(unsigned int*)(GPIOF_BASE+0x0C)
#define GPIOF_IDR					*(unsigned int*)(GPIOF_BASE+0x10)
#define GPIOF_ODR					*(unsigned int*)(GPIOF_BASE+0x14)
#define GPIOF_BSRR					*(unsigned int*)(GPIOF_BASE+0x18)
#define GPIOF_LCKR					*(unsigned int*)(GPIOF_BASE+0x1C)
#define GPIOF_AFRL					*(unsigned int*)(GPIOF_BASE+0x20)
#define GPIOF_AFRH					*(unsigned int*)(GPIOF_BASE+0x24)

/*RCC外设基地址*/
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)

/*RCC的AHB1时钟使能寄存器地址,强制转换成指针*/
#define RCC_AHB1ENR				*(unsigned int*)(RCC_BASE+0x30)
	

