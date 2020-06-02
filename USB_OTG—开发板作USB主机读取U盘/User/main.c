/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   USB 主机例程 USB MSC HOST
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                              头文件
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h" 
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/key/bsp_key.h"
#include "ff.h"
#include "./APP/usbh_bsp.h"
#include "string.h"
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
FRESULT result;
FATFS fs;
FIL file;

//递归扫描文件时使用的路径
char scan_path[255] = "0:";

UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 F407开发板 今天是个好日子，新建文件系统测试文件\r\n";  


KEY Key1,Key2;

static void read_write_flie(void);
static FRESULT scan_files (char* path) ;
/**
  ******************************************************************************
  *                                任务函数
  ******************************************************************************
  */
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* 初始化LED */
	LED_GPIO_Config();
	Key1_GPIO_Config();
	Key2_GPIO_Config();
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
	/* 初始化调试串口，一般为串口1 */
	Debug_USART_Config();
	
  printf("\r\n这是一个USB OTG例程，STM32作USB主机读取U盘 \r\n");
	
	/* Init Host Library */
		USBH_Init(&USB_OTG_Core,
			USB_OTG_FS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb,
            &USR_cb);
  
  printf("\r\n按下KEY1按键可扫描U盘中的文件，");
  printf("\r\n按下KEY2按键可进行文件读写测试。\r\n");

	while(1)
	{
    //USB处理过程
		USBH_Process(&USB_OTG_Core, &USB_Host);
		
		Key_RefreshState(&Key1);//刷新按键状态
		Key_RefreshState(&Key2);//刷新按键状态
		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)//按键被按下过
		{
			LED2_TOGGLE;

      result = f_mount(&fs,"0:",1);	/* Mount a logical drive */
      if(result != FR_OK)
      {
        printf("\r\n文件系统挂载失败！\r\n");
        continue; 
      }
      printf("\r\n扫描到的U盘文件：\r\n");
      
      scan_files(scan_path);
      
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);

		}
		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)//按键被按下过
		{
			LED3_TOGGLE;
      
      
      result = f_mount(&fs,"0:",1);	/* Mount a logical drive */
      if(result != FR_OK)
      {
        printf("\r\n文件系统挂载失败！\r\n");
        continue; 
      }
			
      read_write_flie();		/* 读取根目录下test.txt的内容 */
			
      Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}
	}
}




/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //打开目录
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (;;) 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	
      //目录，递归读取      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //合成完整目录名        
        sprintf(&path[i], "/%s", fn); 		
        //递归遍历         
        res = scan_files(path);	
        path[i] = 0;         
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}

/*
*********************************************************************************************************
*	函 数 名: read_write_flie
*	功能说明: 读取文件test.txt前128个字符，并打印到串口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void read_write_flie(void)
{	

/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
	result = f_open(&file, "0:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( result == FR_OK )
	{
		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		result=f_write(&file,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(result==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",result);
    }    
		/* 不再读写，关闭文件 */
    f_close(&file);
	}
	else
	{	
		printf("！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	result = f_open(&file, "0:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(result == FR_OK)
	{
		printf("》打开文件成功。\r\n");
		result = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(result==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",result);
    }		
	}
	else
	{
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&file);	
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
