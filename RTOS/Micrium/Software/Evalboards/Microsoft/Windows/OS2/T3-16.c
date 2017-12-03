#include "includes.h"
static CPU_STK appTaskStartSTK[APP_TASK_START_STK_SIZE];
static CPU_STK taskPrio1STK[APP_TASK_START_STK_SIZE];
static CPU_STK taskPrio2STK[APP_TASK_START_STK_SIZE];
static char output[] = "Hello,uC/OS-II!";
void appTaskStart(void* pra);
void taskPrio1(void *pra1);
void taskPrio2(void *pra2);
void main()
{
	OSInit();
	OSTaskCreateExt(appTaskStart, output, &appTaskStartSTK[APP_TASK_START_STK_SIZE - 1], 2 
		, 2, &appTaskStartSTK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSStart();
}
void appTaskStart(void *pra)
{
	OS_Sched();
	INT8U times1 = 0; 
	//INT32U ctrBuf=0;
	OS_ERR err;
	(void)pra;
	BSP_Init();
	CPU_Init();
	OSStatInit();
	while (DEF_ON)
	{
		//if (OSCtxSwCtr != ctrBuf)
		//{
		//	ctrBuf = OSCtxSwCtr;
		//	APP_TRACE_DBG(("%d,%d", OSTCBCur->OSTCBPrio,OSCtxSwCtr));
		//}
		if (OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ)
		{
			err=OSTaskDel(OS_PRIO_SELF);
		}
		else if (!((times1+1) % 5));
		else
		{
			APP_TRACE_DBG(("%c", *((char*)pra + times1)));
			OSTimeDlyHMSM(0, 0, 1, 0);
		}
		times1++;
		if (times1 == 4)
		{
			OSTaskCreateExt(taskPrio1, output, &taskPrio1STK[APP_TASK_START_STK_SIZE], 1, 
				1, &taskPrio1STK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
		}
	}
}
void taskPrio1(void *pra1)
{
	//OS_ERR err;
	INT8U times = 0;
	//INT32U ctrBuf=0;
	while (DEF_ON)
	{ 
		times++;
		//if (OSCtxSwCtr != ctrBuf)
		//{
		//	ctrBuf = OSCtxSwCtr;
		//	APP_TRACE_DBG(("\n%d\n", OSTCBCur->OSTCBPrio));
		//}
		if (times == 3)
		{
			APP_TRACE_DBG(("%c", *((char*)pra1 + times * 5 - 1)));
			OSTaskCreateExt(taskPrio2, "this twice should be deleted", &taskPrio2STK[APP_TASK_START_STK_SIZE - 1],
				0, 0, &taskPrio2STK[0], APP_TASK_START_STK_SIZE, 0, OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
		}
		if (OSTaskDelReq(OS_PRIO_SELF) == OS_ERR_TASK_DEL_REQ)
		{
			OSTaskDel(OS_PRIO_SELF);
		}
		else
		{
				APP_TRACE_DBG(("%c", *((char*)pra1 + (times) * 5-1)));
				OSTimeDlyHMSM(0, 0, 4, 0);
		}
	}
}
void taskPrio2(void *pra2)
{
	OS_ERR err=0;
	//INT32U ctrBuf=3;
	while (!err)
	{
		//if (OSCtxSwCtr != ctrBuf)
		//{
		//	ctrBuf = OSCtxSwCtr;
		//	APP_TRACE_DBG(("\n%d\n", OSTCBCur->OSTCBPrio));
		//}
		err = (OSTaskDelReq(1) != OS_ERR_TASK_NOT_EXIST);
		err |= (OSTaskDelReq(2) != OS_ERR_TASK_NOT_EXIST);
	}
}