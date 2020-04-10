/**********************************************************************

    > File Name: draw.h

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月07日 星期二 13时33分02秒

***********************************************************************/

#ifndef __DRAW_H__
#define __DRAW_H__

typedef struct PageDesc
{
	int 			iPage;
	unsigned char 	*pucLcdFirstPosAtFile;
	unsigned char 	*pucLcdNextPageFirstPosAtFile;

	struct PageDesc *ptPrePage;
	struct PageDesc *ptNextPage;
	
} T_PageDesc, *PT_PageDesc;


int OpenTextFile(char *pcFileName);
int SetTextDetail( char *pcFileFreeType, 
					unsigned int dwFontSize);
int SelectAndInitDisplay(char *pcName);
int ShowFirstPage(void);
int ShowNextPage(void);
int ShowPrePage(void);

#endif /* __DRAW_H__ */

