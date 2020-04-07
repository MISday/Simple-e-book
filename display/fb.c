/**********************************************************************

    > File Name: fb.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 13时19分37秒

***********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "display_manager.h"

static int 				g_fd;
static struct fb_var_screeninfo g_tFBVar;
static struct fb_fix_screeninfo g_tFBFix;
static unsigned int 	*g_puiFBMem;
static unsigned long	g_len;


static int	FBDeviceInit(void);
static int	FBShowPixel(int iX, int iY, unsigned int dwColor);
static int FBCleanScreen(unsigned int dwBackColor);

static T_DispOpr g_tFBDispOpr = {
	.name        = "fb",
	.DeviceInit  = FBDeviceInit,
	.ShowPixel   = FBShowPixel,
	.CleanScreen = FBCleanScreen,
};

static int	FBDeviceInit(void)
{
	int			err;

	g_fd = open(FB_DEVICE_NAME, O_RDWR);
	if (g_fd < 0)
	{
		DBG_PRINTF("can't open %s\n", FB_DEVICE_NAME);
		return -1;
	}

    err = ioctl(g_fd, FBIOGET_FSCREENINFO, &g_tFBFix);
	if (err < 0)
    {
		DBG_PRINTF("can't get fb's fix\n");
        goto err1;
    }   

    err = ioctl(g_fd, FBIOGET_VSCREENINFO, &g_tFBVar);
	if (err < 0)
    {
		DBG_PRINTF("can't get fb's var\n");
        goto err1;
    }   

	g_len = g_tFBVar.xres_virtual * \
		g_tFBVar.yres_virtual * \
		g_tFBVar.bits_per_pixel / 8;

	g_puiFBMem = mmap(NULL, g_len, PROT_READ | PROT_WRITE, MAP_SHARED, g_fd, 0);
    if (MAP_FAILED == g_puiFBMem)
    {
		DBG_PRINTF("can't mmap\n");
        goto err1;
    }

	g_tFBDispOpr.iXres	= g_tFBVar.xres;
	g_tFBDispOpr.iYres 	= g_tFBVar.yres;
	g_tFBDispOpr.iBpp	= g_tFBVar.bits_per_pixel;

	return 0;
	
err1:
	close(g_fd);

	return -1;
}

//								X列		 Y行
static int	FBShowPixel(int iX, int iY, unsigned int dwColor)
{
	if ((iX >= g_tFBVar.xres) || (iY >= g_tFBVar.yres))
	{
		DBG_PRINTF("out of region\n");
		return -1;
	}
	
	switch (g_tFBVar.bits_per_pixel)
	{
		case 8:
		{
			dwColor = dwColor << 8	| dwColor;
			dwColor = dwColor << 16	| dwColor;
			*(g_puiFBMem + iY * g_tFBVar.xres + iX) = dwColor;
			break;
		}

		case 32:
		{
			*(g_puiFBMem + iY * g_tFBVar.xres + iX) = dwColor;
			break;
		}

		default :
		{
			DBG_PRINTF("can't support %d bpp\n", g_tFBVar.bits_per_pixel);
			return -1;
		}
	}

	return 0;
}

static int FBCleanScreen(unsigned int dwBackColor)
{
	unsigned long i = 0;

	switch (g_tFBVar.bits_per_pixel)
	{
		case 8:
		{
			while (i < g_len)
			{
				dwBackColor = dwBackColor << 8	| dwBackColor;
				dwBackColor = dwBackColor << 16	| dwBackColor;
				*(g_puiFBMem + i) = dwBackColor;
				i++;
			}
			break;
		}

		case 32:
		{
			memset(g_puiFBMem, dwBackColor, g_len);
			break;
		}

		default :
		{
			DBG_PRINTF("can't support %d bpp\n", g_tFBVar.bits_per_pixel);
			return -1;
		}		
	}

	return 0;
}

int FBInit(void)
{
	RegisterDispOpr(&g_tFBDispOpr);

	return 0;
}

