
#ifndef __GUI_H__
#define __GUI_H__
#include "stdlib.h"
#include "stdint.h"
#include "../../include/helpers.h"

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
void GUI_DrawFont16x20(u16 x, u16 y, u16 fc, u16 bc, u8 s);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void print_str4_v2(u16 x,u16 y,u16 fc,u16 bc,u8 *p,uint8_t size_d);
#endif

