/**********************************************************************

    > File Name: draw.h

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月07日 星期二 13时33分02秒

***********************************************************************/

#ifndef __DRAW_H__
#define __DRAW_H__

int OpenTextFile(char *pcFileName);
int SetTextDetail(char *pcHZKFile, 
					char *pcFileFreeType, 
					unsigned int dwFontSize);
int SelectAndInitDisplay(char *pcName);
int ShowNextPage(void);
int ShowPrePage(void);

#endif /* __DRAW_H__ */

