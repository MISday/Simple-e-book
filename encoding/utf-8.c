/**********************************************************************

    > File Name: utf-8.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 15时14分12秒

***********************************************************************/

#include <string.h>

#include "config.h"
#include "encoding_manager.h"

static int GetPreOneBits(unsigned char ucVal);

static int isUtf8Coding(unsigned char *pucBufHead);
static int GetUtf8CodeFrmBuf(unsigned char *pucBufStart, 
							unsigned char *pucBufEnd, 
							unsigned int *pdwCode);



static T_EncodingOpr g_tUtf8EncodingOpr = {
	.name          = "utf-8",
	.iHeadLen	   = 3,
	.isSupport     = isUtf8Coding,
	.GetCodeFrmBuf = GetUtf8CodeFrmBuf,
};



static int isUtf8Coding(unsigned char *pucBufHead)
{
	const char aStrUtf8[] = {0xEF, 0xBB, 0xBF, 0x00};

	if (strncmp((char *)pucBufHead, aStrUtf8, strlen(aStrUtf8)) == 0)
	{
		/* UTF-8 */
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
 * 判断ucVal中前导1的个数
 */
static int GetPreOneBits(unsigned char ucVal)
{
	int i;
	int num = 0;

	for (i = 7; i >= 0; i--)
	{
		if (!(ucVal & (1<<i)))
			break;
		else
			num++;
	}

	return num;
}


static int GetUtf8CodeFrmBuf(unsigned char *pucBufStart, 
							unsigned char *pucBufEnd, 
							unsigned int *pdwCode)
{
	/*
	 * 1字节 0xxxxxxx
	 * 2字节 110xxxxx 10xxxxxx
	 * 3字节 1110xxxx 10xxxxxx 10xxxxxx
	 * 4字节 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	 * 
	 * 给定一个Byte
	 * 如果最高位为0， 说明为ASCII码
	 * 如果前两位为1， 说明是字节编码的第一个字节
	 * 如果前两位为10，说明是字节编码的中间字节
	 * 如果前n(n<=4)位为1， 说明有有n个字节的数据
	 */

	int i;	
	int iNum;
	unsigned char ucVal;
	unsigned int dwSum = 0;

	ucVal = pucBufStart[0];					//	取出第一字节
	iNum  = GetPreOneBits(pucBufStart[0]);	

	if (pucBufStart >= pucBufEnd)
	{
		/* EOF */
		return 0;
	}

	if (iNum == 0)
	{
		/* ASCII */
		*pdwCode = pucBufStart[0];
		return 1;
	}
	else
	{
		//	读取第一字节
		ucVal = ucVal << iNum;
		ucVal = ucVal >> iNum;
		dwSum += ucVal;	

		for (i = 1; i < iNum; i++)
		{
			ucVal = pucBufStart[i] & 0x3f;
			dwSum = dwSum << 6;
			dwSum += ucVal;			
		}

		*pdwCode = dwSum;
		return iNum;
	}
	


}

int  Utf8EncodingInit(void)
{
	AddFontOprForEncoding(&g_tUtf8EncodingOpr, GetFontOpr("freetype"));
	//AddFontOprForEncoding(&g_tUtf8EncodingOpr, GetFontOpr("ascii"));
	return RegisterEncodingOpr(&g_tUtf8EncodingOpr);
}




/* EOF */
