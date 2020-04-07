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

	
}

int SetTextDetail(char *pcHZKFile, 
					char *pcFileFreeType, 
					unsigned int dwFontSize)
{
	return 0;
}

int SelectAndInitDisplay(char *pcName)
{
	return 0;
}

int ShowNextPage(void)
{
	return 0;
}


int ShowPrePage(void)
{
	return 0;
}



/* EOF */
