/**********************************************************************

    > File Name: display_manager.h

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 13时20分06秒

***********************************************************************/

#ifndef __DISPLAY_MANAGER_H__
#define __DISPLAY_MANAGER_H__

typedef struct DispOpr
{
	char			*name;
	int				iXres;
	int				iYres;
	int				iBpp;

	int	(*DeviceInit)(void);
	int (*ShowPixel)(int iX, int iY, unsigned int dwColor);
	int (*CleanScreen)(unsigned int dwBackColor);

	struct DispOpr *ptNext;
} T_DispOpr, *PT_DispOpr;

//	Display设备提供的注册函数
int RegisterDispOpr(PT_DispOpr ptDispOpr);

void ShowDispOpr(void);

int DisplayInit(void);

int FBInit(void);


#endif /* __DISPLAY_MANAGER_H__ */

