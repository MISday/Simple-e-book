/**********************************************************************

    > File Name: display_manager.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 13时19分31秒

***********************************************************************/

#include <string.h>

#include "config.h"
#include "display_manager.h"

//	用于管理Display设备的无头结点链表
static PT_DispOpr g_ptDispOprHead;

int RegisterDispOpr(PT_DispOpr ptDispOpr)
{
	PT_DispOpr ptTmp;

	if (!g_ptDispOprHead)			//	链表上无设备
	{
		g_ptDispOprHead   = ptDispOpr;
		ptDispOpr->ptNext = NULL;		
	}
	else
	{
		ptTmp = g_ptDispOprHead;	
		while (ptTmp->ptNext)
			ptTmp = ptTmp->ptNext;	//	找到链表尾
		
		ptTmp->ptNext	  = ptDispOpr;
		ptDispOpr->ptNext = NULL;
	}

	return 0;
}

void ShowDispOpr(void)
{
	int i = 0;
	PT_DispOpr ptTmp = g_ptDispOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

PT_DispOpr GetDispOpr(char *pcName)
{
	PT_DispOpr ptTmp = g_ptDispOprHead;
	
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

int DisplayInit(void)
{
	int iError;
	
	iError = FBInit();

	return iError;
}
