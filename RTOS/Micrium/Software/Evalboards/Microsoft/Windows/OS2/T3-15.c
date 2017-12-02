#include "includes.h"
static CPU_STK appTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK taskPrio1STK[APP_TASK_START_STK_SIZE];
static CPU_STK taskPrio2STK[APP_TASK_START_STK_SIZE];
void appTaskStart(void* pra);
void taskPrio1(void* pra1);
void taskPrio2(void* pra2);
void main()
{
	OSInit();
	OSTaskCreateExt(appTaskStart, "it`s the task prio by 0", &appTaskStartSTK[APP_TASK_START_STK_SIZE - 1], 0 \
		, 0, &appTaskStartSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSStart();
}
void appTaskStart(void* pra)
{
	OS_ERR err=0;
	(void)pra;
	BSP_Init();
	CPU_Init();
	OSTaskCreateExt(taskPrio1, "\nIt`s the task prio by 1st\n", &taskPrio1STK[APP_TASK_START_STK_SIZE - 1], 1, 1 \
		, &taskPrio1STK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(taskPrio2, "\nIt`s the task prio by 2nd\n", &taskPrio2STK[APP_TASK_START_STK_SIZE - 1], 2, 2, \
		&taskPrio2STK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSStatInit();
	while (DEF_ON)
	{
		if (OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ)
		{
			//APP_TRACE_DBG(("\nthe Task`s prio by 0 is deleting now\n"));
			err = OSTaskDel(OS_PRIO_SELF);
			APP_TRACE_DBG(("\nthe prio by 0 and err=%d\n", err));
		}
		else
		{
			APP_TRACE_DBG(("\n%s\n", (char *)pra));
			OSTimeDlyHMSM(0, 0, 3, 0);
		}
	}
}
void taskPrio1(void * pra1)
{
	OS_ERR err=0;
	while (DEF_ON)
	{
		if (OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ)
		{
			//APP_TRACE_DBG(("\nthe Task`s prio by 1 is deleting now\n"));
			err=OSTaskDel(OS_PRIO_SELF);
			APP_TRACE_DBG(("\nthe prio by 1 and err=%d\n", err));
		}
		else
		{
			APP_TRACE_DBG(("\n%s\n", (char*)pra1));
			OSTimeDlyHMSM(0, 0, 2, 0);
		}

	}
}
void taskPrio2(void * pra2)
{
	INT8U times=0;
	OS_ERR err;
	while (DEF_ON)
	{
		times++;
		if (times == 10)
		{
			while (((err = OSTaskDelReq(0)) != OS_ERR_TASK_NOT_EXIST) || ((err = OSTaskDelReq(1)) != OS_ERR_TASK_NOT_EXIST))
			{
				//APP_TRACE_DBG(("\n%d\n", err));
				OSTimeDly(1);
			}
		}
		else
		{
			APP_TRACE_DBG(("\n%s\n", (char*)pra2));
			err = OSTimeDlyHMSM(0, 0, 1, 0);
		}
	}
}