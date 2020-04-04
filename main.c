/**********************************************************************

    > File Name: main.c

    > Author: 0nism

    > Email: fd98shadow@sina.com

    > Description: 

    > Created Time: 2020年03月31日 星期二 21时31分07秒

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*---------------------freetype--------------------*/
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#define WIDTH   1024
#define HEIGHT  600

#define WHITE	0xffffffff
#define BLACK	0x00000000

#define FB_PATH "/dev/fb0"

typedef struct _glyph_t
{
	FT_UInt		index;	/* glyph index */ 
	FT_Vector	pos;	/* glyph origin on the baseline */ 
	FT_Glyph	image;	/* glyph image */ 
}glyph_t, *p_glyph_t;

#define MAX_GLYPHS	80




int fd_fb = -1;
struct fb_var_screeninfo var_screeninfo;
struct fb_fix_screeninfo fix_screeninfo;
unsigned int *pfb 	= NULL;

void lcd_put_pixel(int height, int width, unsigned int color);
void draw_bitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y);
void Draw_Glyphs(glyph_t glyphs[], 
				FT_UInt num_glyphs, FT_Vector pen);

int get_glyphs_from_wstr(FT_Face face, 
					wchar_t *wstr, glyph_t glyphs[]);

void compute_string_bbox(glyph_t glyphs[], 
	FT_UInt num_glyphs, FT_BBox *abbox);


int main(int argc, char *argv[])
{
	wchar_t wstr1[] = L"Origin坐标";
	wchar_t wstr2[] = L"Origin坐标";

	int 			err = -1;
    unsigned long 	len = 0;

	FT_Library		library;
	FT_Face			face;


	FT_GlyphSlot	slot;
	FT_Vector   	pen;                 /* untransformed origin */
	FT_Glyph	 	glyph;
	FT_BBox			bbox;
	int           	i;	
	
	int 			line_box_ymin = 10000;
	int 			line_box_ymax = 0;	

	int 			line_box_width;
	int 			line_box_height;

	glyph_t			glyphs[MAX_GLYPHS];
	FT_UInt			num_glyphs;			/* 确定查找glyph成功的字符数量 */
	FT_BBox			statistic_bbox;

	if (argc != 2)
	{
		printf("Usage : %s <font_file>\n", argv[0]);
		return -1;
	}

	/* fb */
	fd_fb = open(FB_PATH, O_RDWR);
	if (fd_fb < 0)
	{
		perror("main.c : open");
		return -1;
	}
	//printf("main.c : open %s success.\n", FB_PATH);

    err = ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix_screeninfo);
	if (err < 0)
    {
        perror("ioctl fix lcd");
        goto err1;
    }   

    err = ioctl(fd_fb, FBIOGET_VSCREENINFO, &var_screeninfo);
	if (err < 0)
    {
        perror("ioctl var lcd");
        goto err1;
    }   

	len = var_screeninfo.xres_virtual * \
		var_screeninfo.yres_virtual * \
		var_screeninfo.bits_per_pixel / 8;

	pfb = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if (MAP_FAILED == pfb)
    {
        perror("mmap");
        goto err1;
    }

	memset(pfb, 0, len);

	/* Freetype */
	err = FT_Init_FreeType( &library );
	if (err)
	{
		printf("FT_Init_FreeType failed\n");
		goto err2;
	}

	//	获取字体对象
	err = FT_New_Face( library,
						 argv[1],
						 0,
						 &face );
	if (err == FT_Err_Unknown_File_Format)
	{
		printf("The font file can't be known\n");
		goto err3;
	}
	else if (err)
	{
		printf("FT_New_Face failed\n");
		goto err2;
	}

	//	设置字体大小
	err = FT_Set_Pixel_Sizes(face, 24, 0);
	if (err)
	{
		printf("FT_Set_Pixel_Sizes failed\n");
		goto err3;
	}

	/* wstr1 */
	num_glyphs = get_glyphs_from_wstr(face, wstr1, glyphs);
	compute_string_bbox(glyphs, num_glyphs, &statistic_bbox);

	line_box_width  = statistic_bbox.xMax - statistic_bbox.xMin;
	line_box_height = statistic_bbox.yMax - statistic_bbox.yMin;

	pen.x = (var_screeninfo.xres - line_box_width)/2 * 64;
	pen.y = (var_screeninfo.yres - line_box_height)/2 * 64;
	
	Draw_Glyphs(glyphs, num_glyphs, pen);

	/* wstr2 */
	num_glyphs = get_glyphs_from_wstr(face, wstr2, glyphs);
	compute_string_bbox(glyphs, num_glyphs, &statistic_bbox);
	
	line_box_width	= statistic_bbox.xMax - statistic_bbox.xMin;
	line_box_height = statistic_bbox.yMax - statistic_bbox.yMin;
	
	pen.x = (var_screeninfo.xres - line_box_width)/2 * 64;
	pen.y = pen.y - 24 * 64;
	Draw_Glyphs(glyphs, num_glyphs, pen);


#if 0 /* 逐行显示 */
	/*	Origin坐标
	 *	LCD坐标系：
	 *	lcd_x = var_screeninfo.xres/2
	 *	lca_y = var_screeninfo.yres/2
	 *	笛卡尔坐标系：
	 *	x = var_screeninfo.xres/2
	 *	y = var_screeninfo.yres - lcd_y = var_screeninfo.yres/2
	 */
	pen.x = 0 * 64;
	pen.y = (var_screeninfo.yres - 24) * 64;

	for (i = 0; i < wcslen(wstr1); i++)
	{
		FT_Set_Transform(face, NULL, &pen);
		
		err = FT_Load_Char(face, wstr1[i], FT_LOAD_RENDER);
		if (err)
		{
			printf("FT_Load_Char failed\n");
			continue;
		}

		err = FT_Get_Glyph(slot, &glyph);
		if (err)
		{
			printf("FT_Get_Glyph failed\n");	
			continue;
		}

		FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);
		if (bbox.yMin < line_box_ymin)
			line_box_ymin = bbox.yMin;
		if (bbox.yMax > line_box_ymax)
			line_box_ymax = bbox.yMax;

		draw_bitmap(&slot->bitmap, 
					slot->bitmap_left, 
					var_screeninfo.yres - slot->bitmap_top);
					
		/* increment pen position */
		pen.x += slot->advance.x;
		//pen.y += slot->advance.y;
	}

	/* 确定座标:
	 * lcd_x = 0
	 * lcd_y = line_box_ymax - line_box_ymin + 24
	 * 笛卡尔座标系:
	 * x = lcd_x = 0
	 * y = var.yres - lcd_y = var.yres - (line_box_ymax - line_box_ymin + 24)
	 */
	pen.x = 0 * 64;
	pen.y = (var_screeninfo.yres - (line_box_ymax - line_box_ymin + 24)) * 64;

	for (i = 0; i < wcslen(wstr2); i++)
	{
		FT_Set_Transform(face, NULL, &pen);
		
		err = FT_Load_Char(face, wstr2[i], FT_LOAD_RENDER);
		if (err)
		{
			printf("FT_Load_Char failed\n");
			continue;
		}

		err = FT_Get_Glyph(slot, &glyph);
		if (err)
		{
			printf("FT_Get_Glyph failed\n");	
			continue;
		}

		FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);
		if (bbox.yMin < line_box_ymin)
			line_box_ymin = bbox.yMin;
		if (bbox.yMax > line_box_ymax)
			line_box_ymax = bbox.yMax;

		draw_bitmap(&slot->bitmap, 
					slot->bitmap_left, 
					var_screeninfo.yres - slot->bitmap_top);
					
		/* increment pen position */
		pen.x += slot->advance.x;
		//pen.y += slot->advance.y;
	}
#endif /* 逐行显示 */

err3:
	FT_Done_Face(face);

err2:
	FT_Done_FreeType(library);

err1:
	close(fd_fb);
	
    return 0;
}

/*
 *	表示height行width列的像素被赋值
 *	color : 0x00RRGGBB	
 *	
 */
void lcd_put_pixel(int height, int width, unsigned int color)
{
	//	灰度转RGB888
	color = color << 8 | color;
	color = color << 16 | color;
	
	*(pfb + height * WIDTH + width) = color;
}

/*	
 *	需要传输的数据是矢量图左上角点在LCD中的位置
 */
void draw_bitmap(FT_Bitmap *bitmap, FT_Int x, FT_Int y)
{
	FT_Int	i, j, p, q;
	FT_Int	x_max = x + bitmap->width;		//	位图宽度
	FT_Int	y_max = y + bitmap->rows;		//	位图高度
	
	for ( i = x, p = 0; i < x_max; i++, p++ )
	{
	  for ( j = y, q = 0; j < y_max; j++, q++ )
	  {
		if ( i < 0		|| j < 0	   ||
			 i >= WIDTH || j >= HEIGHT )
		  continue;

		//	buffer中装的就是具体数据
		//	q行p列
		//image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		//	j行i列
		lcd_put_pixel(j, i, bitmap->buffer[q * bitmap->width + p]);
	  }
	}

}

int get_glyphs_from_wstr(FT_Face face, 
					wchar_t *wstr, glyph_t glyphs[])
{
	FT_UInt				glyph_index = 0;
	p_glyph_t 			glyph = glyphs;
	FT_GlyphSlot  		slot;
	int 				i;
	FT_Vector     		pen;
	int					error;

	memset(glyph, 0, sizeof(glyph_t) * MAX_GLYPHS);

	pen.x = 0 * 64;
	pen.y = 0 * 64;

	slot = face->glyph; 
	
	for (i = 0; i < wcslen(wstr); i++)
	{
		glyph->index = FT_Get_Char_Index(face, wstr[i]);
		if (glyph->index == 0)
			continue;

		glyph->pos.x = pen.x;
		glyph->pos.y = pen.y;
		
		error = FT_Load_Glyph(face, glyph->index, FT_LOAD_DEFAULT);
		if (error)
			continue;

		error = FT_Get_Glyph(face->glyph, &glyph->image);
		if (error)
			continue;

		/* 使image中包含有位置信息 */
		FT_Glyph_Transform(glyph->image, 0, &glyph->pos );

		pen.x += slot->advance.x;

		glyph++;
	}

	return (glyph - glyphs);
}

void compute_string_bbox(glyph_t glyphs[], 
			FT_UInt num_glyphs, FT_BBox *abbox)
{
	FT_BBox			bbox;
	FT_BBox			glyph_bbox;
	int				i;

	bbox.xMin = bbox.yMin =  32000;
	bbox.xMax = bbox.yMax = -32000;

	for (i = 0; i < num_glyphs; i++)
	{
	    FT_Glyph_Get_CBox( glyphs[i].image, FT_GLYPH_BBOX_TRUNCATE,
	                       &glyph_bbox);

	    if (glyph_bbox.xMin < bbox.xMin)
	      bbox.xMin = glyph_bbox.xMin;

	    if (glyph_bbox.yMin < bbox.yMin)
	      bbox.yMin = glyph_bbox.yMin;

	    if (glyph_bbox.xMax > bbox.xMax)
	      bbox.xMax = glyph_bbox.xMax;

	    if (glyph_bbox.yMax > bbox.yMax)
	      bbox.yMax = glyph_bbox.yMax;
	}

	*abbox = bbox;	

}

void Draw_Glyphs(glyph_t glyphs[], 
			FT_UInt num_glyphs, FT_Vector pen)
{
	int					i;
	int					error;

	for (i = 0; i < num_glyphs; i++)
	{
		FT_Glyph_Transform(glyphs[i].image, 0, &pen);

		error = FT_Glyph_To_Bitmap(&glyphs[i].image, 
									FT_RENDER_MODE_NORMAL, 
									0, 		/* no additional translation */ 
                              		1 ); 	/* destroy copy in "image" */
		if ( !error ) 
		{ 
			FT_BitmapGlyph bit = (FT_BitmapGlyph)glyphs[i].image; 
			draw_bitmap(&bit->bitmap, bit->left, var_screeninfo.yres - bit->top); 
			FT_Done_Glyph(glyphs[i].image); 
		}

	}
}


/* EOF */
