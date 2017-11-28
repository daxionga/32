#include "includes.h"
static CPU_STK AppTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK myTaskSTK[APP_TASK_START_STK_SIZE];
static void AppTaskStart(void* pra);
static void myTask(void *pra1);
int main(void)
{
	OSInit();
	OSTaskCreateExt(AppTaskStart,(void*)0, &AppTaskStartSTK[APP_TASK_START_STK_SIZE-1], 1, 1, &AppTaskStartSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSStart();
}
void AppTaskStart(void *pra)
{
	INT8U times = 0;
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
	OSTaskCreateExt(myTask, &OSCPUUsage, &myTaskSTK[APP_TASK_START_STK_SIZE - 1], 0, 0, &myTaskSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	while (DEF_ON)
	{
		if ((OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ) && (times != 3))//what`s the variable
		{
			APP_TRACE_DBG(("\n我不删，我不想删\n"));
			OSTimeDlyHMSM(0, 0, 1, 0);//
			times++;
		}
		else if (times == 3)
		{
			times = 0;
			APP_TRACE_DBG(("\n既然你苦苦相逼，我只好删了我自己啦，拜拜\n"));
			OSTaskDel(OS_PRIO_SELF);
		}
		else
		{
			APP_TRACE_DBG(("\nI don`t like someone forever emulating\n"));
			OSTimeDlyHMSM(0, 0, 3, 0);//
		}
	}
}
void myTask(void *pra1)
{
	INT8U times = 0;
#if OS_CFG_TASK_EN > 0
	OS_ERR err;
#endif
	while (DEF_ON)
	{
		times++;
		if (times >= 10)
		{
			times = 0;
			while (OSTaskDelReq(1) != OS_ERR_TASK_NOT_EXIST)
			{
				OSTimeDly(1);
			}
		}
		APP_TRACE_DBG(("\nthe CPU use percet %d%%\n", *(INT8U*)pra1));
		OSTimeDlyHMSM(0,0,1,0);
	}
}