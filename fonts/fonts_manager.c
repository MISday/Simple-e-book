/**********************************************************************

    > File Name: fonts_manager.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 15时13分12秒

***********************************************************************/

#include <string.h>

#include "config.h"
#include "fonts_manager.h"

//	用于管理Display设备的无头结点链表
static PT_FontOpr g_ptFontOprHead;

int RegisterFontOpr(PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptTmp;

	if (!g_ptFontOprHead)
	{
		g_ptFontOprHead   = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptFontOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext     = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	}

	return 0;

}

void ShowFontOpr(void)
{
	int i = 0;
	PT_FontOpr ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}

}

int FontsInit(void)
{
	int iError;
	
#if 0	
	iError = ASCIIInit();
	if (iError)
	{
		DBG_PRINTF("ASCIIInit error!\n");
		return -1;
	}

	iError = GBKInit();
	if (iError)
	{
		DBG_PRINTF("GBKInit error!\n");
		return -1;
	}
#endif

	iError = FreeTypeInit();
	if (iError)
	{
		DBG_PRINTF("FreeTypeInit error!\n");
		return -1;
	}

	return 0;
}

PT_FontOpr GetFontOpr(char *pcName)
{
	PT_FontOpr	ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
			return ptTmp;

		ptTmp = ptTmp->ptNext;
	}
	
	return NULL;
}



