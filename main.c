/**********************************************************************

    > File Name: main.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年03月31日 星期二 21时31分07秒

***********************************************************************/

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "draw.h"
#include "encoding_manager.h"
#include "display_manager.h"
#include "fonts_manager.h"

static struct option long_options[] = 
{
	{"size", 	required_argument,	0,	's'},
	{"freetype",required_argument,	0,	'f'},
	{0,			0,					0,	0},
};

int main(int argc, char *argv[])
{
	int 			iError;
	int				c;
	int				option_index = 0;
	char			*long_opt_arg;
	unsigned int 	dwFontSize = 16;
	
	char 			acFreetypeFile[128];
	char 			acTextFile[128];
	char			acDisplay[128];

	acFreetypeFile[0] = '\0';
	acTextFile[0] = '\0';

	strcpy(acDisplay, "fb");

	while ((c = getopt_long(argc, argv, "s:f:", long_options, NULL)) != -1)
	{
		switch(c)
		{
			case 's':
			{
				dwFontSize = strtoul(optarg, NULL, 0);
				break;
			}
			
			case 'f':
			{
				strncpy(acFreetypeFile, optarg, 128);
				acFreetypeFile[127] = '\0';
				break;
			}
			default:
			{
				printf("Usage: %s [-s size] [-f font_file] <text_file>", argv[0]);
				return -1;
			}
		}
	}

	static void RecordPage(PT_PageDesc ptPageNew)
	{
		PT_PageDesc ptPage;
			
		if (!g_ptPages)
		{
			g_ptPages = ptPageNew;
		}
		else
		{
			ptPage = g_ptPages;
			while (ptPage->ptNextPage)
			{
				ptPage = ptPage->ptNextPage;
			}
			ptPage->ptNextPage	 = ptPageNew;
			ptPageNew->ptPrePage = ptPage;
		}
	}
	iError = DisplayInit();
	if (iError)
	{
		printf("DisplayInit error!\n");
		return -1;
	}

	iError = FontsInit();
	if (iError)
	{
		printf("FontsInit error!\n");
		return -1;
	}

	iError = EncodingInit();
	if (iError)
	{
		printf("EncodingInit error!\n");
		return -1;
	}

	printf("supported display:\n");
	ShowDispOpr();
	
	printf("supported font:\n");
	ShowFontOpr();
	
	printf("supported encoding:\n");
	ShowEncodingOpr();

	strncpy(acTextFile, argv[optind], 128);
	acTextFile[127] = '\0';

	//	打开文本文件，设置文件头
	iError = OpenTextFile(acTextFile);
	if (iError)
	{
		printf("OpenTextFile error!\n");
		return -1;
	}

	//	设置文件字体以及大小
	iError = SetTextDetail(acFreetypeFile, dwFontSize);
	if (iError)
	{
		printf("SetTextDetail error!\n");
		return -1;
	}

	DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	//	选择显示设备
	iError = SelectAndInitDisplay(acDisplay);
	if (iError)
	{
		printf("SelectAndInitDisplay error!\n");
		return -1;
	}

	iError = ShowNextPage();
	if (iError)
	{
		printf("Error to show first page\n");
		return -1;
	}


	return 0;
}

/* EOF */
