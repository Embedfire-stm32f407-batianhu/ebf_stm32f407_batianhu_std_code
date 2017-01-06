/*
*********************************************************************************************************
*
*	模块名称 : USB HOST 驱动模块,大容量存储设备
*	文件名称 : usbh_bsp_msc.h
*	版    本 : V1.0
*	说    明 : 应用程序包含本头文件，即可使用读写U盘的功能函数
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _USBH_BSP_USR_H
#define _USBH_BSP_USR_H

#include "./APP/usb_conf.h"
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"

#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usbh_msc_core.h"

#include "usbh_usr.h"

//void usbd_OpenMassStorage(void);
//void usbd_CloseMassStorage(void);

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
