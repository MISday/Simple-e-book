/**********************************************************************

    > File Name: encoding_manager.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 15时13分59秒

***********************************************************************/

#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "encoding_manager.h"

static PT_EncodingOpr g_ptEncodingOprHead;

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	PT_EncodingOpr ptTmp;

	if (!g_ptEncodingOprHead)
	{
		g_ptEncodingOprHead   = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptEncodingOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	      = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	}

	return 0;
}

void ShowEncodingOpr(void)
{
	int i = 0;
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

int EncodingInit(void)
{
	int iError;

#if 0
	iError = AsciiEncodingInit();
	if (iError)
	{
		DBG_PRINTF("AsciiEncodingInit error!\n");
		return -1;
	}

	iError = Utf16leEncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf16leEncodingInit error!\n");
		return -1;
	}
	
	iError = Utf16beEncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf16beEncodingInit error!\n");
		return -1;
	}
#endif	

	iError = Utf8EncodingInit();
	if (iError)
	{
		DBG_PRINTF("Utf8EncodingInit error!\n");
		return -1;
	}

	return 0;
}

PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead)
{
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

	while (ptTmp)
	{
		if (ptTmp->isSupport(pucFileBufHead))
			return ptTmp;
		else
			ptTmp = ptTmp->ptNext;
	}

	return NULL;
}

int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr	ptTemp;

	if (!ptEncodingOpr || !ptFontOpr)
	{	
		return -1; 
	}
	else
	{
		ptTemp = malloc(sizeof(T_FontOpr));
		if (!ptTemp)
		{
			return -1;
		}
		else
		{
			memcpy(ptTemp, ptFontOpr, sizeof(T_FontOpr));
			ptTemp->ptNext = ptEncodingOpr->ptFontOprSupportedHead;		//	尾插
			ptEncodingOpr->ptFontOprSupportedHead = ptTemp;
			return 0;
		}
	}
}

int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr	ptTemp;	
	PT_FontOpr	ptPre;	


	if (!ptEncodingOpr || !ptFontOpr)
	{	
		return -1; 
	}

	ptTemp = ptEncodingOpr->ptFontOprSupportedHead;
	if (strcmp(ptTemp->name, ptFontOpr->name) == 0)
	{
		ptEncodingOpr->ptFontOprSupportedHead = ptTemp->ptNext;
		free(ptTemp);
		return 0;
	}

	ptPre 	=	ptEncodingOpr->ptFontOprSupportedHead;
	ptTemp 	=	ptPre->ptNext;
	while (ptTemp)
	{
		if (strcmp(ptTemp->name, ptFontOpr->name) == 0)
		{
			ptPre->ptNext = ptTemp->ptNext;
			free(ptTemp);
			return 0;
		}
		ptTemp = ptTemp->ptNext;
		ptPre = ptPre->ptNext;
	}
	
	return -1;

}


