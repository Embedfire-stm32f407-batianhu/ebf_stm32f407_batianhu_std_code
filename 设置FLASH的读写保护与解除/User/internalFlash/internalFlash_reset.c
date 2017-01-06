/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   恢复选项字节为默认值范例(解除读、写、PCROP保护)
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./internalFlash/internalFlash_reset.h" 





/**
  * @brief  InternalFlash_Reset,恢复内部FLASH的默认配置
  * @param  None
  * @retval None
  */
int InternalFlash_Reset(void)
{
	
	FLASH_Status status = FLASH_COMPLETE;

	
  /* 使能访问选项字节寄存器 */
  FLASH_OB_Unlock();
		    
  /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
  /* 清除各种FLASH的标志位 */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	FLASH_INFO("\r\n");
	FLASH_INFO("正在准备恢复的条件，请耐心等待...");
	
	//确保把读保护级别设置为LEVEL1，以便恢复PCROP寄存器位
	//PCROP寄存器位从设置为0时，必须是读保护级别由LEVEL1转为LEVEL0时才有效，
	//否则修改无效
	FLASH_OB_RDPConfig(OB_RDP_Level_1);
	
	status=FLASH_OB_Launch();
	
	status = FLASH_WaitForLastOperation();

	//设置为LEVEL0并恢复PCROP位		
	
	FLASH_INFO("\r\n");
	FLASH_INFO("正在擦除内部FLASH的内容，请耐心等待...");
	
	//关闭PCROP模式
	FLASH_OB_PCROPSelectionConfig(OB_PcROP_Disable);
	FLASH_OB_RDPConfig(OB_RDP_Level_0);

	status =FLASH_OB_Launch();
	
	//设置其它位为默认值
	(*(__IO uint32_t *)(OPTCR_BYTE0_ADDRESS))=0x0FFFAAE9;
	(*(__IO uint16_t *)(OPTCR1_BYTE2_ADDRESS))=0x0FFF;
	status =FLASH_OB_Launch();
	
	
	if (status	 != FLASH_COMPLETE)
	{			
		FLASH_ERROR("恢复选项字节默认值失败，错误代码：status=%X",status);		
		
	}
	else
	{
		FLASH_INFO("恢复选项字节默认值成功！");

	}


	//禁止访问
	FLASH_OB_Lock();

			
	return status;

}




#define FLASH_WRP_SECTORS   (OB_WRP_Sector_0|OB_WRP_Sector_1) 
__IO uint32_t SectorsWRPStatus = 0xFFF;

/**
  * @brief  WriteProtect_Test,普通的写保护配置
  * @param  运行本函数后会给扇区FLASH_WRP_SECTORS进行写保护，再重复一次会进行解写保护
  * @retval None
  */
void WriteProtect_Test(void)
{
	FLASH_Status status = FLASH_COMPLETE;	
  {
    /* 获取扇区的写保护状态 */
    SectorsWRPStatus = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
		
    if (SectorsWRPStatus == 0x00)
    {
     /* 扇区已被写保护，执行解保护过程*/

      /* 使能访问OPTCR寄存器 */
      FLASH_OB_Unlock();

      /* 设置对应的nWRP位，解除写保护 */
      FLASH_OB_WRPConfig(FLASH_WRP_SECTORS, DISABLE); 
			status=FLASH_OB_Launch();
      /* 开始对选项字节进行编程 */  
      if (status	 != FLASH_COMPLETE)
      {
				FLASH_ERROR("对选项字节编程出错，解除写保护失败，status = %x",status);
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }
      /* 禁止访问OPTCR寄存器 */
      FLASH_OB_Lock();

    /* 获取扇区的写保护状态 */
      SectorsWRPStatus = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;
  
      /* 检查是否配置成功 */
      if (SectorsWRPStatus == FLASH_WRP_SECTORS)
      {
        FLASH_INFO("解除写保护成功！");
      }
      else
      {
          FLASH_ERROR("未解除写保护！");
      }
    }
    else
    { /* 若扇区未被写保护，开启写保护配置 */

      /* 使能访问OPTCR寄存器 */
      FLASH_OB_Unlock();

      /*使能 FLASH_WRP_SECTORS 扇区写保护 */
      FLASH_OB_WRPConfig(FLASH_WRP_SECTORS, ENABLE); 

			status=FLASH_OB_Launch();
      /* 开始对选项字节进行编程 */  
      if (status	 != FLASH_COMPLETE)
      {
				FLASH_ERROR("对选项字节编程出错，设置写保护失败，status = %x",status);        
        while (1)
        {
        }
      }

      /* 禁止访问OPTCR寄存器 */
      FLASH_OB_Lock();

    /* 获取扇区的写保护状态 */
      SectorsWRPStatus = FLASH_OB_GetWRP() & FLASH_WRP_SECTORS;

      /* 检查是否配置成功 */
      if (SectorsWRPStatus == 0x00)
      {
        FLASH_INFO("设置写保护成功！");
      }
      else
      {
        FLASH_ERROR("设置写保护失败！");
      }
    }
  }
}


/**
  * @brief  OptionByte_Info,打印选项字节的配置信息
  * @param  None
  * @retval None
  */
void OptionByte_Info(void)
{
	uint32_t temp_optcr = *(uint32_t *)OPTCR_BYTE0_ADDRESS;
	uint32_t temp_optrc1h = *(uint16_t *)OPTCR1_BYTE2_ADDRESS;
	
	uint16_t temp;
	
	FLASH_INFO("\r\n");
	FLASH_INFO("选项字节寄存器OPTCR的内容：0x%08X",temp_optcr);
	FLASH_INFO("选项字节寄存器OPTCR1高16位的内容：0x%04X",temp_optrc1h);
	
	
	//读保护级别
	FLASH_INFO("\r\n");
	FLASH_INFO("-------------读保护级别-------------");
	if(FLASH_OB_GetRDP()==RESET)
	{
		FLASH_INFO("STM32内部FLASH读保护级别为LEVEL0");	
	}
	else 
	{
		//FLASH_OB_GetRDP函数返回值为SET时，可能为LEVEL1或LEVEL2，
		//但在LEVEL2时，连RAM调试都不能运行，即本程序都无法下载到芯片，
		//在LEVEL2时芯片无法再解锁
		FLASH_INFO("内部FLASH读保护级别为LEVEL1");
	}
	
	FLASH_DEBUG("\r\n");
	FLASH_DEBUG("-------------写保的护扇区-------------");
	temp = FLASH_OB_GetWRP();
	if(temp == 0x0FFF)
	{
		FLASH_DEBUG("默认值：无读写保护，使用single Bank模式");
	}
	else if(temp&(1<<15))
		FLASH_DEBUG("使用了PCROP代码保护模式");
	else if(temp&(1<<14))
		FLASH_DEBUG("使用了dual bank模式");


	
	FLASH_DEBUG("\r\n");	
	FLASH_DEBUG("-------------用户选项字节USER------------");
	temp = FLASH_OB_GetUser();
	
	if(temp&0x01)
		FLASH_DEBUG("默认值：软件独立看门狗");
	else
		FLASH_DEBUG("硬件独立看门狗");
	
	if(temp&(0x01<<1))
		FLASH_DEBUG("默认值：进入停止模式时不产生复位");
	else
		FLASH_DEBUG("进入停止模式时产生复位");

	if(temp&(0x01<<2))	
		FLASH_DEBUG("默认值：进入待机模式时不产生复位");
	else
		FLASH_DEBUG("进入待机模式时产生复位");
	
	FLASH_DEBUG("\r\n");	
	FLASH_DEBUG("-------------复位级别BOR_LEV------------");
	temp = FLASH_OB_GetBOR();
	switch(temp)
	{
		case OB_BOR_LEVEL3:
				FLASH_DEBUG("复位阈值电压为 2.70 V 到 3.60 V");
				break;
		
		case OB_BOR_LEVEL2:
				FLASH_DEBUG("复位阈值电压为 2.40 V 到 2.70 V");
				break;
		
		case OB_BOR_LEVEL1:
				FLASH_DEBUG("复位阈值电压为 2.10 V 到 2.40 V");
				break;
			
		case OB_BOR_OFF:
				FLASH_DEBUG("默认值：复位阈值电压为 1.80 V 到 2.10 V");
				break;	
	}			
				

}	











