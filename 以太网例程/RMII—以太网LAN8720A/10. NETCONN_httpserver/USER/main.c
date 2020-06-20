/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   http 网页服务器 ucos版本
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "includes.h"
#include "lwipopts/netconf.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/* --------------- APPLICATION GLOBALS ---------------- */
static  OS_TCB                        AppTaskStartTCB;
static  CPU_STK                       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB                        AppTaskObj0TCB;
static  CPU_STK                       AppTaskObj0Stk[APP_CFG_TASK_OBJ_STK_SIZE];


#ifdef USE_DHCP
#define APP_CFG_TASK_DHCP_PRIO        6
#define APP_CFG_TASK_DHCP_STK_SIZE    512
static  OS_TCB                        AppTaskDHCPTCB;
static  CPU_STK                       AppTaskDHCPStk[APP_CFG_TASK_DHCP_STK_SIZE];
#endif

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  AppTaskStart (void  *p_arg);
static  void  AppTaskCreate(void);
static  void  AppObjCreate (void);

static  void  AppTaskObj0    (void  *p_arg);

extern void http_server_netconn_init(void);


extern struct netif gnetif;

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
*********************************************************************************************************
*/

int main(void)
{
	OS_ERR   err;
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
	CPU_ERR  cpu_err;
#endif                 

	Mem_Init();                                                 /* Initialize Memory Managment Module                   */
	Math_Init();                                                /* Initialize Mathematical Module                       */

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
	CPU_NameSet((CPU_CHAR *)"STM32F429II",
							(CPU_ERR  *)&cpu_err);
#endif

	BSP_IntDisAll();                                            /* Disable all Interrupts.                              */

	OSInit(&err);                                               /* Init uC/OS-III.                                      */
	App_OS_SetAllHooks();

	OSTaskCreate(&AppTaskStartTCB,                              /* Create the start task                                */
								"App Task Start",
								AppTaskStart,
								0u,
								APP_CFG_TASK_START_PRIO,
								&AppTaskStartStk[0u],
								AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
								APP_CFG_TASK_START_STK_SIZE,
								0u,
								0u,
								0u,
							  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							  &err);

	OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

	while (DEF_ON) {                                            /* Should Never Get Here.                               */
			;
	}
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
	OS_ERR      err;
 (void)p_arg;

	BSP_Init();                                                 /* Initialize BSP functions                             */
	CPU_Init();                                                 /* Initialize the uC/CPU services                       */

#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
	CPU_IntDisMeasMaxCurReset();
#endif

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
	APP_TRACE_DBG(("Creating Application kernel objects\n\r"));
#endif	
	AppObjCreate(); 
/* Create Applicaiton kernel objects                    */
#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
	APP_TRACE_DBG(("Creating Application Tasks\n\r"));
#endif	
	AppTaskCreate();                                            /* Create Application tasks                             */

	/* Initilaize the LwIP stack */
	LwIP_Init();
	
#ifdef USE_DHCP
	 /* Start DHCPClient */
	 OSTaskCreate(&AppTaskDHCPTCB,"DHCP",
								LwIP_DHCP_task,
								&gnetif,
								APP_CFG_TASK_DHCP_PRIO,
								&AppTaskDHCPStk[0],
								AppTaskDHCPStk[APP_CFG_TASK_DHCP_STK_SIZE / 10u],
								APP_CFG_TASK_DHCP_STK_SIZE,
								0u,
								0u,
								0,
							 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							 &err);
#endif  //#ifdef USE_DHCP

	http_server_netconn_init();
  //本地IP地址可在netconf.h文件修改
  printf("在电脑端浏览器直接输入地址：%d.%d.%d.%d，既可访问\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  
	while (DEF_TRUE)
	{                                          
		OSTimeDlyHMSM(0u, 0u, 1u, 0u,
									OS_OPT_TIME_HMSM_STRICT,
									&err);
	}
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    OS_ERR  os_err;

    /* ---------- CREATE KERNEL OBJECTS TEST TASK --------- */
    OSTaskCreate(&AppTaskObj0TCB,
                 "Kernel Objects Task 0",
                  AppTaskObj0,
                  0,
                  APP_CFG_TASK_OBJ_PRIO,
                 &AppTaskObj0Stk[0],
                  AppTaskObj0Stk[APP_CFG_TASK_OBJ_STK_SIZE / 10u],
                  APP_CFG_TASK_OBJ_STK_SIZE,
                  0u,
                  0u,
                  0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
}


/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create Application Kernel Objects.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{

}
/*
*********************************************************************************************************
*                                          AppTaskObj0()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskObj0' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskObj0 (void  *p_arg)
{
	OS_ERR  os_err;
	(void)p_arg;

	while (DEF_TRUE) {
		  LED3_TOGGLE;
			OSTimeDlyHMSM( 0u, 0u, 1u, 0u,
										 OS_OPT_TIME_HMSM_STRICT,
										&os_err);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
