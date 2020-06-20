/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用1.5.1版本库建的工程模板
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

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/* --------------- APPLICATION GLOBALS ---------------- */
static  OS_TCB       AppTaskStartTCB;
static  CPU_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB       AppTaskObj0TCB;
static  CPU_STK      AppTaskObj0Stk[APP_CFG_TASK_OBJ_STK_SIZE];

static  OS_TCB       AppTaskObj1TCB;
static  CPU_STK      AppTaskObj1Stk[APP_CFG_TASK_OBJ_STK_SIZE];


#if (OS_CFG_SEM_EN > 0u)
static  OS_SEM       AppTraceSem;
#endif

#if (OS_CFG_SEM_EN > 0u)
static  OS_SEM       AppTaskObjSem;
#endif

#if (OS_CFG_MUTEX_EN > 0u)
static  OS_MUTEX     AppTaskObjMutex;
#endif

#if (OS_CFG_Q_EN > 0u)
static  OS_Q         AppTaskObjQ;
#endif

#if (OS_CFG_FLAG_EN > 0u)
static  OS_FLAG_GRP  AppTaskObjFlag;
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
static  void  AppTaskObj1    (void  *p_arg);


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

    while (DEF_TRUE)
		{                                          /* Task body, always written as an infinite loop.       */
        LED3_TOGGLE;
				if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN)==KEY_ON)
				{
					LED1_TOGGLE;
					
				}
				if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN)==KEY_ON)
				{
					LED2_TOGGLE;					
				}				
        OSTimeDlyHMSM(0u, 0u, 0u, 200u,
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
	 OSTaskCreate(&AppTaskObj1TCB,
                 "Kernel Objects Task 0",
                  AppTaskObj1,
                  0,
                  APP_CFG_TASK_OBJ_PRIO,
                 &AppTaskObj1Stk[0],
                  AppTaskObj1Stk[APP_CFG_TASK_OBJ_STK_SIZE / 10u],
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
	OS_ERR  os_err;


#if (OS_CFG_SEM_EN > 0u)
    OSSemCreate(&AppTaskObjSem,
                "Sem Test",
                 0u,
                &os_err);

    OSSemCreate(&AppTraceSem,
                "Trace Lock",
                 1u,
                &os_err);
#endif

#if (OS_CFG_MUTEX_EN > 0u)
    OSMutexCreate(&AppTaskObjMutex,
                  "Mutex Test",
                  &os_err);
#endif

#if (OS_CFG_Q_EN > 0u)
    OSQCreate(&AppTaskObjQ,
              "Queue Test",
               1,
              &os_err);
#endif

#if (OS_CFG_FLAG_EN > 0u)
    OSFlagCreate(&AppTaskObjFlag,
                 "Flag Test",
                  DEF_BIT_NONE,
                 &os_err);
#endif
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
#if (OS_CFG_SEM_EN > 0u)
        OSSemPost(&AppTaskObjSem,
                   OS_OPT_POST_1,
                  &os_err);
#endif

#if (OS_CFG_MUTEX_EN > 0u)
        OSMutexPend(&AppTaskObjMutex,
                     0,
                     OS_OPT_PEND_BLOCKING,
                     0,
                    &os_err);

        OSTimeDlyHMSM( 0u, 0u, 0u, 100u,
                       OS_OPT_TIME_HMSM_STRICT,
                      &os_err);

        OSMutexPost(&AppTaskObjMutex,
                     OS_OPT_POST_NONE,
                    &os_err);
#endif

#if (OS_CFG_Q_EN > 0u)
        OSQPost(        &AppTaskObjQ,
                (void *) 1u,
                         1u,
                         OS_OPT_POST_FIFO,
                        &os_err);

#endif

#if (OS_CFG_FLAG_EN > 0u)
        OSFlagPost(&AppTaskObjFlag,
                    DEF_BIT_00,
                    OS_OPT_POST_FLAG_SET,
                   &os_err);
#endif
        OSTimeDlyHMSM( 0u, 0u, 1u, 0u,
                       OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        APP_TRACE_INFO(("Object test task 0 running ....\n"));
    }
}


/*
*********************************************************************************************************
*                                          AppTaskObj1()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskObj1' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskObj1 (void  *p_arg)
{
    OS_ERR       os_err;
    OS_MSG_SIZE  msg_size;


    (void)p_arg;

    while (DEF_TRUE) {

#if (OS_CFG_SEM_EN > 0u)
        OSSemPend(&AppTaskObjSem,
                   0,
                   OS_OPT_PEND_BLOCKING,
                   0,
                  &os_err);
#endif

#if (OS_CFG_MUTEX_EN > 0u)
        OSMutexPend(&AppTaskObjMutex,
                     0,
                     OS_OPT_PEND_BLOCKING,
                     0,
                    &os_err);

        OSMutexPost(&AppTaskObjMutex,
                     OS_OPT_POST_NONE,
                    &os_err);

#endif

#if (OS_CFG_Q_EN > 0u)
        OSQPend(&AppTaskObjQ,
                 0,
                 OS_OPT_PEND_BLOCKING,
                &msg_size,
                 0,
                &os_err);
#endif

#if (OS_CFG_FLAG_EN > 0u)
        OSFlagPend(&AppTaskObjFlag,
                    DEF_BIT_00,
                    0,
                    OS_OPT_PEND_FLAG_SET_ALL + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_BLOCKING,
                    0,
                   &os_err);
#endif
        OSTimeDlyHMSM( 0u, 0u, 1u, 0u,
                       OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
        APP_TRACE_INFO(("Object test task 1 running ....\n"));
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
