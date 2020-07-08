
/*片上外设基地址  */
#define PERIPH_BASE           ((unsigned int)0x40000000)                          

/*总线基地址 */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)	

/*GPIO外设基地址*/
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)

/*RCC外设基地址*/
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)


/* GPIOF寄存器地址,强制转换成指针 */
//#define GPIOF_MODER				*(unsigned int*)(GPIOF_BASE+0x00)
//#define GPIOF_OTYPER			*(unsigned int*)(GPIOF_BASE+0x04)
//#define GPIOF_OSPEEDR			*(unsigned int*)(GPIOF_BASE+0x08)
//#define GPIOF_PUPDR				*(unsigned int*)(GPIOF_BASE+0x0C)
//#define GPIOF_IDR					*(unsigned int*)(GPIOF_BASE+0x10)
//#define GPIOF_ODR					*(unsigned int*)(GPIOF_BASE+0x14)
//#define GPIOF_BSRR					*(unsigned int*)(GPIOF_BASE+0x18)
//#define GPIOF_LCKR					*(unsigned int*)(GPIOF_BASE+0x1C)
//#define GPIOF_AFRL					*(unsigned int*)(GPIOF_BASE+0x20)
//#define GPIOF_AFRH					*(unsigned int*)(GPIOF_BASE+0x24)

/*RCC的AHB1时钟使能寄存器地址,强制转换成指针*/
//此处不使用，用结构体来代替
//#define RCC_AHB1ENR				*(unsigned int*)(RCC_BASE+0x30)


//寄存器的值常常是芯片外设自动更改的，即使CPU没有执行程序，也有可能发生变化
//编译器有可能会对没有执行程序的变量进行优化

//volatile表示易变的变量，防止编译器优化，
#define     __IO    volatile
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

/* GPIO寄存器列表 */
typedef struct
{
	__IO	uint32_t MODER;    /*GPIO模式寄存器						地址偏移: 0x00      */
	__IO	uint32_t OTYPER;   /*GPIO输出类型寄存器				地址偏移: 0x04      */
	__IO	uint32_t OSPEEDR;  /*GPIO输出速度寄存器				地址偏移: 0x08      */
	__IO	uint32_t PUPDR;    /*GPIO上拉/下拉寄存器			地址偏移: 0x0C      		*/
	__IO	uint32_t IDR;      /*GPIO输入数据寄存器				地址偏移: 0x10      		*/
	__IO	uint32_t ODR;      /*GPIO输出数据寄存器				地址偏移: 0x14      		*/
	__IO	uint16_t BSRRL;    /*GPIO置位/复位寄存器 低16位部分	地址偏移: 0x18 	*/
	__IO	uint16_t BSRRH;    /*GPIO置位/复位寄存器 高16位部分	地址偏移: 0x1A  */
	__IO	uint32_t LCKR;     /*GPIO配置锁定寄存器				地址偏移: 0x1C      */
	__IO	uint32_t AFR[2];   /*GPIO复用功能配置寄存器		地址偏移: 0x20-0x24 		*/
} GPIO_TypeDef;

/*RCC寄存器列表*/
typedef struct
{
__IO	uint32_t CR;            /*!< RCC 时钟控制寄存器,				地址偏移: 0x00 */
__IO	uint32_t PLLCFGR;       /*!< RCC PLL配置寄存器, 				地址偏移: 0x04 */
__IO	uint32_t CFGR;          /*!< RCC 时钟配置寄存器,   		地址偏移: 0x08 */
__IO	uint32_t CIR;           /*!< RCC 时钟中断寄存器,     		地址偏移: 0x0C */
__IO	uint32_t AHB1RSTR;      /*!< RCC AHB1 外设复位寄存器,	地址偏移: 0x10 */
__IO	uint32_t AHB2RSTR;      /*!< RCC AHB2 外设复位寄存器, 	地址偏移: 0x14 */
__IO	uint32_t AHB3RSTR;      /*!< RCC AHB3 外设复位寄存器, 	地址偏移: 0x18 */
__IO	uint32_t RESERVED0;     /*!< 保留, 										地址偏移：0x1C */
__IO	uint32_t APB1RSTR;      /*!< RCC APB1 外设复位寄存器,	地址偏移: 0x20 */
__IO	uint32_t APB2RSTR;      /*!< RCC APB2 外设复位寄存器,	地址偏移: 0x24 */
__IO	uint32_t RESERVED1[2];  /*!< 保留, 										地址偏移：0x28-0x2C*/
__IO	uint32_t AHB1ENR;       /*!< RCC AHB1 外设时钟寄存器,	地址偏移: 0x30 */
__IO	uint32_t AHB2ENR;       /*!< RCC AHB2 外设时钟寄存器,	地址偏移: 0x34 */
__IO	uint32_t AHB3ENR;       /*!< RCC AHB3 外设时钟寄存器,	地址偏移: 0x38 */
	/*RCC后面还有很多寄存器，此处省略*/
} RCC_TypeDef;


/*定义GPIOA-H 寄存器结构体指针*/
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)

/*定义RCC外设 寄存器结构体指针*/
#define RCC                 ((RCC_TypeDef *) RCC_BASE)

