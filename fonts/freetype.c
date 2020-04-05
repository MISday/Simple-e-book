/**********************************************************************

    > File Name: freetype.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年04月04日 星期六 15时13分21秒

***********************************************************************/

#include <config.h>
#include <fonts_manager.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Library g_library;
static FT_Face g_face;
static FT_GlyphSlot g_tSlot;

static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize);	
static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

static T_FontOpr g_tFreeTypeFontOpr = {
	.name          = "freetype",
	.FontInit      = FreeTypeFontInit,
	.GetFontBitmap = FreeTypeGetFontBitmap,
};

static int FreeTypeFontInit(char *pcFontFile, unsigned int dwFontSize)
{
	int iError;

	iError = FT_Init_FreeType( &g_library );
	if (iError)
	{
		DBG_PRINTF("FT_Init_FreeType failed\n");
		goto err1;
	}

	//	获取字体对象
	iError = FT_New_Face(g_library, pcFontFile, 0, &g_face);
	if (iError == FT_Err_Unknown_File_Format)
	{
		DBG_PRINTF("The font file can't be known\n");
		goto err2;
	}
	else if (iError)
	{
		DBG_PRINTF("FT_New_Face failed\n");
		goto err1;
	}

	g_tSlot = g_tFace->glyph;

	//	设置字体大小
	iError = FT_Set_Pixel_Sizes(face, 24, 0);
	if iError
	{
		DBG_PRINTF("FT_Set_Pixel_Sizes failed\n");
		goto err2;
	}

	return 0;
	
err2:
	FT_Done_Face(g_face);
	
err1:
	FT_Done_FreeType(g_library);

	return -1;
}

static int FreeTypeGetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int iError;
	int iPenX = ptFontBitMap->iCurOriginX;
	int iPenY = ptFontBitMap->iCurOriginY;

	iError = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
	if (iError)
	{
		DBG_PRINTF("FT_Load_Char error for code : 0x%x\n", dwCode);
		return -1;
	}

	ptFontBitMap->iXLeft    = iPenX + g_tSlot->bitmap_left;
	ptFontBitMap->iYTop     = iPenY - g_tSlot->bitmap_top;
	ptFontBitMap->iXMax     = ptFontBitMap->iXLeft + g_tSlot->bitmap.width;
	ptFontBitMap->iYMax     = ptFontBitMap->iYTop  + g_tSlot->bitmap.rows;
	ptFontBitMap->iBpp      = 1;
	ptFontBitMap->iPitch    = g_tSlot->bitmap.pitch;
	ptFontBitMap->pucBuffer = g_tSlot->bitmap.buffer;
	
	ptFontBitMap->iNextOriginX = iPenX + g_tSlot->advance.x / 64;
	ptFontBitMap->iNextOriginY = iPenY;

	return 0;

}


int FreeTypeInit(void)
{
	return RegisterFontOpr(&g_tFreeTypeFontOpr);
}

