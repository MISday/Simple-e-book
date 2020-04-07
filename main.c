/**********************************************************************

    > File Name: main.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年03月31日 星期二 21时31分07秒

***********************************************************************/

#include <unistd.h>
#include <getopt.h>

#include "config.h"
#include "draw.h"
#include "encoding_manager.h"
#include "display_manager.h"
#include "fonts_manager.h"

static struct option long_options[] = 
{
	{"size", 	required_argument,	0,	's'},
	{"freetype",optional_argument,	0,	'f'},
	{0,			0,					0,	0},
};

int main(int argc, char *argv[])
{
	int iError;

	//iError = getopt_long(argc, argv, "", );

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

	return 0;
}

/* EOF */
