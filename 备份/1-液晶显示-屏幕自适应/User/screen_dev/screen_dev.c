#include "./screen_dev/screen_dev.h"

void LCD_Device_Init(DEVICE_PARAM_TypeDef* p)
{
	
	//参数检查，只可输入0-7
	if(p->my_display->LCD_SCAN_MODE >7 )
		return;
	
	//根据模式更新XY方向的像素宽度
	if(p->my_display->LCD_SCAN_MODE%2 == 0)	
	{
		//0 2 4 6模式下X方向像素宽度为480，Y方向为854
		p->my_display->LCD_X_LENGTH = p->my_display->dp_max_width;
		p->my_display->LCD_Y_LENGTH =	p->my_display->dp_max_height;
	}
	else				
	{
		//1 3 5 7模式下X方向像素宽度为854，Y方向为480
		p->my_display->LCD_X_LENGTH = p->my_display->dp_max_height;
		p->my_display->LCD_Y_LENGTH =	p->my_display->dp_max_width; 
	}
}



