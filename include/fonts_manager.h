/**********************************************************************

    > File Name: fonts_manager.h

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 15时12分39秒

***********************************************************************/

#ifndef __FONTS_MANAGER_H__
#define __FONTS_MANAGER_H__

typedef struct FontBitMap
{
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	int iPitch;   /* 对于单色位图, 两行象素之间的跨度 */
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;	
} T_FontBitMap, *PT_FontBitMap;

typedef struct FontOpr
{
	char		*name;

	int (*FontInit)(char *pcFontFile, unsigned int dwFontSize);	
	int (*GetFontBitmap)(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

	struct FontOpr *ptNext;
} T_FontOpr, *PT_FontOpr;

int RegisterFontOpr(PT_FontOpr ptFontOpr);
void ShowFontOpr(void);
int FontsInit(void);
int FreeTypeInit(void);
PT_FontOpr GetFontOpr(char *pcName);

#endif /* __FONTS_MANAGER_H__ */


