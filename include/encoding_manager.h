/**********************************************************************

    > File Name: encoding_manager.h

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 15时13分43秒

***********************************************************************/

#ifndef __ENCODING_MANAGER_H__
#define __ENCODING_MANAGER_H__

#include <fonts_manager.h>

typedef struct EncodingOpr 
{
	char		*name;
	int 		iHeadLen;				// 编码头部长度
	PT_FontOpr 	ptFontOprSupportedHead;	// 链表,表示哪些字体支持该编码

	int (*isSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, 
						unsigned char *pucBufEnd, 
						unsigned int *pdwCode);
	
	struct EncodingOpr *ptNext;
} T_EncodingOpr, *PT_EncodingOpr;

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);

void ShowEncodingOpr(void);

int EncodingInit(void);

int Utf8EncodingInit(void);

/* 从文件头中确认要使用的编码方式 */
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);

#endif	/* __ENCODING_MANAGER_H__ */







