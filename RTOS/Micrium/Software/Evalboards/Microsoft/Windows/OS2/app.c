#include <includes.h>
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  void  AppTaskStart(void  *p_arg);
/* Description : This is the standard entry point for C code.  It is assumed that your code will call
 *               main() once you have performed all necessary initialization.
 * Argument(s) : none.
 * Return(s)   : none.*/
int  main (void){
    OSInit();                                                   /* Init uC/OS-II.                                       */
    OSTaskCreateExt((void(*)(void *))AppTaskStart,              /* Create the start task                                */
        (void          *) 0,
        (OS_STK        *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
        (INT8U          ) APP_TASK_START_PRIO,
        (INT16U         ) APP_TASK_START_PRIO,
        (OS_STK        *)&AppTaskStartStk[0],
        (INT32U         ) APP_TASK_START_STK_SIZE,
        (void          *) 0,
        (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
}
/*Description : This is an example of a startup task.  As mentioned in the book's text, you MUST initialize the ticker only once multitasking has started.
 * Argument(s) : p_arg       is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
 * Return(s)   : none.
 * Note(s)     : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not used.  The compiler should not generate any code for this statement.*/
static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;
   (void)p_arg;
    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize uC/CPU services                           */
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif
    APP_TRACE_DBG(("uCOS-II is Running...\n\r"));
    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0, 0, 1, 0);
        APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet(&err)));
    }
}
