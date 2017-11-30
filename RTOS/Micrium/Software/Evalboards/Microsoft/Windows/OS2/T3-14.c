#include "includes.h"
static CPU_STK AppTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK myTaskSTK[APP_TASK_START_STK_SIZE];
static CPU_STK taskQuerySTK[APP_TASK_START_STK_SIZE];
static CPU_STK theFouthTaskSTK[APP_TASK_START_STK_SIZE];
void AppTaskStart(void* pra);
void myTask(void* pra1);
void taskQuery(void* pra2);
void theFouthTask(void *pra3);
OS_TCB testResult;
void main(void)
{
	OSInit();
	OSTaskCreateExt(AppTaskStart, 0, &AppTaskStartSTK[APP_TASK_START_STK_SIZE - 1], 0, 0, &AppTaskStartSTK[0], APP_TASK_START_STK_SIZE, "AppTaskStart", OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSStart(); 
}
void AppTaskStart(void *pra)
{
	OS_ERR err;
	(void)pra;
	BSP_Init();
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0
	OSCPUUsageInit();
#endif
	OSStatInit();
	OSTaskCreateExt(myTask, &OSCPUUsage, &myTaskSTK[APP_TASK_START_STK_SIZE - 1], 1, 1, &myTaskSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(taskQuery, &testResult, &taskQuerySTK[APP_TASK_START_STK_SIZE - 1], 2, 2, &taskQuerySTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	while (DEF_ON)
	{
		APP_TRACE_DBG(("\nthe first prio task, but it not useful\n"));
		OS_Sched();
		OSTimeDlyHMSM(0, 0, 4, 0);
	}
}
void myTask(void *pra1)
{
	while (DEF_ON)
	{
		APP_TRACE_DBG(("\nthe CPUUsage is %d%%\n", *(INT8U*)pra1));
		OSTimeDlyHMSM(0, 0, 3, 0);
	}
}
void taskQuery(void* pra2)
{
	OS_ERR err;
	while (DEF_ON)
	{
		OSTaskQuery(0,pra2);
		APP_TRACE_DBG(("\n%d,%d\n", ((OS_TCB*)pra2)->OSTCBPrio, ((OS_TCB*)pra2)->OSTCBDly));
		err=OSTaskQuery(1, pra2);
		APP_TRACE_DBG(("\n%d,%d\n", ((OS_TCB*)pra2)->OSTCBPrio, ((OS_TCB*)pra2)->OSTCBDly));
		OSTimeDlyHMSM(0, 0, 2, 0);
	}
}