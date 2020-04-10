/**********************************************************************

    > File Name: draw.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月07日 星期二 13时36分51秒

***********************************************************************/

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "config.h"
#include "draw.h"
#include "encoding_manager.h"
#include "display_manager.h"
#include "fonts_manager.h"


static int				g_iFdTextFile;
static unsigned char	*g_pucTextFileMem;
static unsigned char	*g_pucTextFileMemEnd;

static unsigned char	*g_pucLcdFirstPosAtFile;
static unsigned char	*g_pucLcdNextPosAtFile;

//	编码、字体、大小、显示相关
static PT_EncodingOpr	g_ptEncodingOprForFile;
static PT_DispOpr		g_ptDispOpr;
static int				g_dwFontSize;

//	页面相关
static PT_PageDesc 		g_ptPages   = NULL;
static PT_PageDesc 		g_ptCurPage = NULL;


int OpenTextFile(char *pcFileName)
{
	int			ret;
	struct stat tStat;

	g_iFdTextFile = open(pcFileName, O_RDONLY);
	if (g_iFdTextFile < 0)
	{
		DBG_PRINTF("open %s failed\n", pcFileName);
		return -1;
	}

	ret = fstat(g_iFdTextFile, &tStat);
	if (ret < 0)
	{
		DBG_PRINTF("fstat failed\n");
		return -2;
	}

	g_pucTextFileMem = (unsigned char *)mmap(NULL, 
											tStat.st_size,
											PROT_READ, 
											MAP_SHARED, 
											g_iFdTextFile,
											0);
	if (g_pucTextFileMem == MAP_FAILED)
	{
		DBG_PRINTF("mmap failed\n");
		return -3;		
	}
	
	g_pucTextFileMemEnd = g_pucTextFileMem + tStat.st_size;	

	g_ptEncodingOprForFile = SelectEncodingOprForFile(g_pucTextFileMem);
	if (g_ptEncodingOprForFile)
	{
		g_pucLcdFirstPosAtFile = g_pucTextFileMem + g_ptEncodingOprForFile->iHeadLen;
		return 0;
	}
	else
	{
		return -1;
	}
}

int SetTextDetail(char *pcFileFreeType, 
					unsigned int dwFontSize)
{
	int	iError;
	int iRet;
	PT_FontOpr ptFontOpr;
	PT_FontOpr ptTmp;

	g_dwFontSize = dwFontSize;

	ptFontOpr = g_ptEncodingOprForFile->ptFontOprSupportedHead;

	while (ptFontOpr)
	{
		if (strcmp(ptFontOpr->name, "freetype") == 0)
		{
			iError = ptFontOpr->FontInit(pcFileFreeType, dwFontSize);
		}

		DBG_PRINTF("%s, %d\n", ptFontOpr->name, iError);

		ptTmp = ptFontOpr->ptNext;

		if (iError == 0)
		{
			iRet = 0;
		}
		else
		{
			DelFontOprFrmEncoding(g_ptEncodingOprForFile, ptFontOpr);
		}
		ptFontOpr = ptTmp;

	}

	return iRet;
}

int SelectAndInitDisplay(char *pcName)
{
	int	iError;

	g_ptDispOpr = GetDispOpr(pcName);
	if (!g_ptDispOpr)
	{
		return -1;
	}

	iError = g_ptDispOpr->DeviceInit();

	return 0;
}

int ShowOnePage(unsigned char *pucTextFileMemCurPos)
{
	int iError;
	unsigned char *pucBufStart;	

	pucBufStart = pucTextFileMemCurPos;


	return 0;
}

static void RecordPage(PT_PageDesc ptPageNew)
{
	
}


int ShowNextPage(void)
{
	int	iError;
	PT_PageDesc ptPage;
	unsigned char *pucTextFileMemCurPos;

	if (g_ptCurPage)
	{
		pucTextFileMemCurPos = g_ptCurPage->pucLcdFirstPosAtFile;
	}
	else
	{
		pucTextFileMemCurPos = g_pucLcdFirstPosAtFile;
	}

	iError = ShowOnePage(pucTextFileMemCurPos);	

	if (iError == 0)
	{
		if (g_ptCurPage && g_ptCurPage->ptNextPage)
		{
			g_ptCurPage = g_ptCurPage->ptNextPage;
			return 0;			
		}

		ptPage = malloc(sizeof(T_PageDesc));
		if (ptPage)
		{
			ptPage->pucLcdFirstPosAtFile 			= pucTextFileMemCurPos;
			ptPage->pucLcdNextPageFirstPosAtFile 	= g_pucLcdNextPosAtFile;
			ptPage->ptPrePage						= NULL;
			ptPage->ptNextPage						= NULL;
			g_ptCurPage = ptPage;
			RecordPage(ptPage);
			return 0;
		}
		else
		{
			return -1;
		}
	}

	return iError;
}


int ShowPrePage(void)
{
	return 0;
}



/* EOF */
