#include "includes.h"
static CPU_STK AppTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK myTaskSTK[APP_TASK_START_STK_SIZE];
void AppTaskStart(void *pra);
void myTask(void *pra1);
int main(void)
{
	OSInit();
	OSTaskCreateExt(AppTaskStart, 0, &AppTaskStartSTK[APP_TASK_START_STK_SIZE - 1], 0, 0, &AppTaskStartSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSStart();
}
void AppTaskStart(void* pra)
{
#if OS_CFG_TASK_EN > 0
	OS_ERR err;
#endif
	(void)pra;
	BSP_Init();
	CPU_Init();
#if OS_CFG_TASK_EN > 0
	OSCPUUsageInit();
#endif
	OSStatInit();
	OSTaskCreateExt(myTask, "myTask`s prament", &myTaskSTK[APP_TASK_START_STK_SIZE - 1], 1, 1, &myTaskSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	while (DEF_ON)
	{
		if (OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ)
		{
			APP_TRACE_DBG(("I`ll Del myself,but now I`m here"));
			OSTaskDel(OS_PRIO_SELF);
		}
		else
		{
			APP_TRACE_DBG(("\nthe prio is 0\n"));
			OSTimeDlyHMSM(0, 0, 3, 0);
		}
	}
}
void myTask(void *pra1)
{
	INT8U times=0;
#if OS_CFG_TASK_EN > 0
	OS_ERR err;
#endif
	(void)pra1;
	while (DEF_ON)
	{
		times++;
		if (times >= 5)
		{
			while (OSTaskDelReq(0) != OS_ERR_TASK_NOT_EXIST)
				OSTimeDly(1);
		}
		else
		{
			APP_TRACE_DBG(("\nthe prio is 1\n"));
			APP_TRACE_DBG(("\nthe cpu use percent is %%%d\n", OSCPUUsage));
			OSTimeDlyHMSM(0, 0, 1, 0);
		}
	}
}
