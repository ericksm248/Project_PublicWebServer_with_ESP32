
#ifndef __TFT_H
#define __TFT_H		
#include <Arduino.h>	
#include <SPI.h> 
#include "stdlib.h"
#include "stdint.h"
#include "../../include/helpers.h"

typedef struct  
{										    
	u16 width;			 
	u16 height;			
	u16 id;				   
	u8  dir;			  	
	u16	 wramcmd;		
  u16  setxcmd;
	u16  setycmd;
}_lcd_dev; 	


extern _lcd_dev lcddev;	
#define USE_HORIZONTAL  	 0
	  
#define LCD_W 320
#define LCD_H 480
   
extern u16  POINT_COLOR;   
extern u16  BACK_COLOR;



//GPIO SET
#define	LCD_CS_SET  digitalWrite(5,HIGH)   
#define	LCD_RS_SET  digitalWrite(22,HIGH)   
#define	LCD_RST_SET  digitalWrite(21,HIGH)    

//GPIO CLEAR		    
#define	LCD_CS_CLR  digitalWrite(5,LOW)   
#define	LCD_RS_CLR  digitalWrite(22,LOW)     
#define	LCD_RST_CLR  digitalWrite(21,LOW) 

//Color code RGB565
//https://chrishewett.com/blog/true-rgb565-colour-picker/
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F 
#define WBLUE       0xb75f
#define WGREEN      0x674f
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE	    0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40 
#define BRRED 		0XFC07 
#define GRAY  		0X8430 
#define BLGRAY 		0xB5B6
#define BLGREEN     0x0520
#define BLYELLOW    0xfd80
#define PINK        0xFCB2
#define WHYELLOW    0xFFD3
#define ORANGE 		0xfc42
#define BBLUE       0x000e

#define DARKBLUE    0X01CF	
#define LIGHTBLUE   0X7D7C	
#define GRAYBLUE  	0X5458 
#define LIGHTBLUE2  0x0bff
 
#define LIGHTGREEN 	0X841F 
#define LIGHTGRAY   0XEF5B 
#define LGRAY 		0XC618 

#define LGRAYBLUE  	0XA651 
#define LBBLUE      0X2B12
	    															  
void LCD_Init(void);
void LCD_Clear(u16 Color);
void SPI_WriteByte(uint8_t spidata);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_WriteRAM_Prepare(void);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_WR_REG(u8 data);		    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_direction(u8 direction );
				  		 
#endif