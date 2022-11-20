/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\lcd\lcd.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-05-21 21:15:41
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-26 23:10:16
 */
#ifndef __lcd_h__
#define __lcd_h__

#include "per_conf.h"

extern  u16 dma_color_buf;

#ifdef __dma_h__
//#define endma
#ifdef endma

#endif
#endif

void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color); 
void LCD_DrawPoint(u16 x, u16 y, u16 color);                    
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);    

void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 mode);
void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 mode); 
void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc);      
void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc);

void LCD_ShowChineseString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 mode);

void LCD_Clear(u16 color);
void LCD_Test_u(float t);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
u32 mypow(u8 m, u8 n); //求幂
//void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);
void LCD_Fillcolor_pal(u32 lens,u8 *color);

#ifdef OLD_SCREEN
#define WHITE (0xffff - 0xFFFF)
#define BLACK (0xffff - 0x0000)
#define BLUE (0xffff - 0x001F)
#define BRED (0xffff - 0xF81F)
#define GRED (0xffff - 0xFFE0)
#define GBLUE (0xffff - 0x07FF)
#define RED (0xffff - 0xF800)
#define MAGENTA (0xffff - 0xF81F)
#define GREEN (0xffff - 0x07E0)
#define CYAN (0xffff - 0x7FFF)
#define YELLOW (0xffff - 0xFFE0)
#define BROWN (0xffff - 0xBC40)     
#define BRRED (0xffff - 0xFC07)     
#define GRAY (0xffff - 0x8430)      
#define DARKBLUE (0xffff - 0x01CF)   
#define LIGHTBLUE (0xffff - 0x7D7C)  
#define GRAYBLUE (0xffff - 0x5458)   
#define LIGHTGREEN (0xffff - 0x841F) 
#define LGRAY (0xffff - 0xC618)      
#define LGRAYBLUE (0xffff - 0xA651)  
#define LBBLUE (0xffff - 0x2B12)     
#else
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0xF81F
#define GRED 0xFFE0
#define GBLUE 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0xBC40  
#define BRRED 0xFC07  
#define GRAY 0x8430     
#define DARKBLUE 0x01CF   
#define LIGHTBLUE 0x7D7C 
#define GRAYBLUE 0x5458   
#define LIGHTGREEN 0x841F 
#define LGRAY 0xC618    
#define LGRAYBLUE 0xA651  
#define LBBLUE 0x2B12    
#endif

#endif
