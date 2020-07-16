#ifndef _SCREEB_DEV_H
#define _SCREEB_DEV_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "./font/fonts.h"

typedef struct _tFont sFONT;
/** 
  * @brief 触摸屏描述结构体
  */
typedef struct
{
  /*根据触摸屏类型配置*/
  uint16_t					tp_max_width;				//触点最大值，一般为显示屏的高
  uint16_t					tp_max_height;			//触点最大值，一般为显示屏的宽
  uint16_t					config_reg_addr;  	//不同类型的触摸ic配置寄存器地址可能不同
	uint16_t					ic_id;							//触摸芯片id
	const uint8_t*		config_talbe;				//触摸屏配置表
	uint8_t						config_talbe_len;		//触摸屏配置表大小
	const char*				device_name;				//触摸屏名称
}TOUCH_PARAM_TypeDef;


/** 
  * @brief 显示屏参数
  */
typedef struct
{
  /*根据显示屏类型配置*/
  uint16_t 		dp_max_width;  	//显示屏硬件高度最大值
  uint16_t 		dp_max_height;  //显示屏硬件宽度最大值
  uint16_t		device_id;			//显示屏设备id
	uint8_t			LCD_SCAN_MODE;	//显示屏扫描模式，对应驱动的.c文件中有描述
	uint16_t		LCD_X_LENGTH;		//显示屏当前显示的x轴长度
	uint16_t		LCD_Y_LENGTH;		//显示屏当前显示的y轴长度
	const char* device_name;		//显示屏名称
	const char*	size_info;			//显示屏尺寸
}DISP_PARAM_TypeDef;


/** 
  * @brief 显示屏驱动接口
  */
typedef struct
{
void 										 (*xx_send_cmd)									(uint16_t usCmd);
void 										 (*xx_write_data)								(uint16_t usData);
uint16_t 								 (*xx_read_data)								(void);
void										 (*xx_init_func)								(void);
uint32_t								 (*xx_Read_Chipid)							(void);
void                     (*xx_Rst)											(void);
void                     (*xx_BackLed_Control)					(FunctionalState enumState);
void                     (*xx_GramScan)									(uint8_t ucOtion);
void                     (*xx_OpenWindow)								(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     (*xx_Clear)										(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     (*xx_SetPointPixel)						(uint16_t usX, uint16_t usY );
uint16_t                 (*xx_GetPointPixel)						(uint16_t usX , uint16_t usY );
void                     (*xx_DrawLine)									(uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 );
void                     (*xx_DrawRectangle)						(uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight,uint8_t ucFilled );
void                     (*xx_DrawCircle)								(uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled );
void                     (*xx_DispChar_EN)							(uint16_t usX, uint16_t usY, const char cChar );
void                     (*xx_DispStringLine_EN)				(uint16_t line, char * pStr );
void                     (*xx_DispString_EN)						(uint16_t usX, uint16_t usY, char * pStr );
void 										 (*xx_DispString_EN_YDir)				(uint16_t usX,uint16_t usY ,  char * pStr );
void                     (*xx_DispChar_CH)							(uint16_t usX, uint16_t usY, uint16_t usChar );
void                     (*xx_DispString_CH)						(uint16_t usX, uint16_t usY,  char * pStr );
void                     (*xx_DispString_EN_CH)					(uint16_t usX, uint16_t usY,  char * pStr );
void 										 (*xx_DispStringLine_EN_CH)			(uint16_t line, char * pStr );
void 										 (*xx_DispString_EN_CH_YDir)		(uint16_t usX,uint16_t usY , char * pStr );
void 										 (*xx_SetFont)									(sFONT *fonts);
sFONT* 										(*xx_GetFont)									(void);
void 										 (*xx_ClearLine)								(uint16_t Line);
void 										 (*xx_SetBackColor)							(uint16_t Color);
void 										 (*xx_SetTextColor)							(uint16_t Color);
void 										 (*xx_SetColors)								(uint16_t TextColor, uint16_t BackColor);
void 										 (*xx_GetColors)								(uint16_t *TextColor, uint16_t *BackColor);
void 										 (*xx_DrawLineCircle)						(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t thick );
void 										 (*xx_DisplayStringEx)					(uint16_t x,uint16_t y,uint16_t Font_width,	uint16_t Font_Height,	uint8_t *ptr,	uint16_t DrawModel);//新成员2020.7.16 add by jc
void 										 (*xx_DisplayStringEx_YDir)			(uint16_t x,uint16_t y,uint16_t Font_width,	uint16_t Font_Height,	uint8_t *ptr,	uint16_t DrawModel);//新成员2020.7.16 add by jc
}DISP_DRIVE_PARAM_TypeDef;


/** 
  * @brief 触摸显示设备描述
  */
typedef struct
{
	TOUCH_PARAM_TypeDef					*my_touchpanel;	//触摸屏描述结构体
	DISP_PARAM_TypeDef					*my_display;		//显示屏描述结构体
	DISP_DRIVE_PARAM_TypeDef 		*my_disp_driver;//显示屏驱动接口
}DEVICE_PARAM_TypeDef;


/** 
  * @brief 声明两个LCD设备，在各自的驱动中定义。
  */
extern DISP_DRIVE_PARAM_TypeDef NT35510_DP_Driver;
extern DISP_PARAM_TypeDef NT35510_DP_Desc;

extern DISP_DRIVE_PARAM_TypeDef ILI9806G_DP_Driver;
extern DISP_PARAM_TypeDef ILI9806G_DP_Desc;

extern DEVICE_PARAM_TypeDef *LCD;

void LCD_Device_Init(DEVICE_PARAM_TypeDef* p);
#endif
