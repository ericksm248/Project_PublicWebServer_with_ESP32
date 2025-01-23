//////////////////////////////////////////////////////////////////////////////////	 
//GUI control functions
//Some functions are modified from the original LCDWiki implementation.
//http://www.lcdwiki.com/
/****************************************************************************************************/
#include "string.h" 
#include "../../include/FONT.H"
#include "GUI.h"
#include "tft.h"

/*******************************************************************
 * @name       :uint8_t get_ascii(uint8_t data_ascii)
 * @date       :2023 
 * @function   :Gets ascii codes for Spanish-only characters
 * @parameters :data_ascii:Ascii code of special character
 * @author	   :Ericson
********************************************************************/
uint8_t get_ascii(uint8_t data_ascii)
{
	if(data_ascii ==0xa1)	  //á
		return 127;
	else if(data_ascii ==0xa9)//é
		return 128;
	else if(data_ascii ==0xad)//í
		return 129;
	else if(data_ascii ==0xb3)//ó
		return 130;
	else if(data_ascii ==0xba)//ú
		return 131;
	else if(data_ascii ==0x81)//Á
		return 132;
	else if(data_ascii ==0x89)//É
		return 133;
	else if(data_ascii ==0x8d)//Í
		return 134;
	else if(data_ascii ==0x93)//Ó
		return 135;
	else if(data_ascii ==0x9a)//Ú
		return 136;
	else if(data_ascii ==0xb1)//ñ
		return 137;
	else if(data_ascii ==0x91)//Ñ
		return 138;
	else return 0;
}

/*******************************************************************
 * @name       :void GUI_DrawPoint(u16 x, u16 y)
 * @date       :2018-08-09  
 * @function   :Draws one point in the SPI
 * @parameters :x: X coordinate of the point
 * 				y: Y coordinate of the point
 * @author	   :LCDWiki	
********************************************************************/
void GUI_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09  
 * @function   :Fills the specified area
 * @parameters :sx:the beginning x coordinate of the specified area
                sy:the beginning y coordinate of the specified area
				ex:the ending x coordinate of the specified area
				ey:the ending y coordinate of the specified area
				color:the filled color value
 * @modified   :Ericson
  * @from      :LCDWiki
********************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	
	u16 i,j;			
	u16 width=ex-sx+1; 
	u16 height=ey-sy+1;	
	LCD_SetWindows(sx,sy,ex,ey);
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			SPI_WriteByte(color>>8);
			SPI_WriteByte(color);		
		}			
	}
	LCD_CS_SET;
	LCD_SetWindows(0,0,479,319);
}

/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the beginning x coordinate of the line
                y1:the beginning y coordinate of the line
				x2:the ending x coordinate of the line
				y2:the ending y coordinate of the line
 * @author     :LCDWiki
********************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1;
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0; 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x;
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ ) 
	{  
		GUI_DrawPoint(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/*****************************************************************************
 * @name       :void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draws a rectangle
 * @parameters :x1:the beginning x coordinate of the rectangle
                y1:the beginning y coordinate of the rectangle
				x2:the ending x coordinate of the rectangle
				y2:the ending y coordinate of the rectangle
 * @author     :LCDWiki
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}  

/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2023 
 * @function   :Fills a rectangle
 * @parameters :x1:the beginning x coordinate of the filled rectangle
                y1:the beginning y coordinate of the filled rectangle
				x2:the ending x coordinate of the filled rectangle
				y2:the ending y coordinate of the filled rectangle
 * @author     :LCDWiki		
******************************************************************************/  
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
}

/*****************************************************************************
 * @name       :void Draw_Triangle(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2023
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the beginning x coordinate of the triangular edge 
                y0:the beginning y coordinate of the triangular edge 
				x1:the vertex x coordinate of the triangular
				y1:the vertex y coordinate of the triangular
				x2:the ending x coordinate of the triangular edge 
				y2:the ending y coordinate of the triangular edge 
 * @author     :LCDWiki
******************************************************************************/ 
void Draw_Triangle(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_DrawLine(x0,y0,x1,y1);
	LCD_DrawLine(x1,y1,x2,y2);
	LCD_DrawLine(x2,y2,x0,y0);
}

static void _swap(u16 *a, u16 *b)
{
	u16 tmp;
  	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :void Fill_Triangle(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the beginning x coordinate of the triangular edge 
                y0:the beginning y coordinate of the triangular edge 
				x1:the vertex x coordinate of the triangular
				y1:the vertex y coordinate of the triangular
				x2:the ending x coordinate of the triangular edge 
				y2:the ending y coordinate of the triangular edge 
 * @author     :LCDWiki
******************************************************************************/ 

void Fill_Triangle(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) 
	{
    	_swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) 
	{
    	_swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  	if (y0 > y1) 
	{
    	_swap(&y0,&y1); 
		_swap(&x0,&x1);
  	}
	if(y0 == y2) 
	{ 
		a = b = x0;
		if(x1 < a)
		{
			a = x1;
		}
		else if(x1 > b)
		{
			b = x1;
		}
		if(x2 < a)
		{
			a = x2;
		}
		else if(x2 > b)
		{
			b = x2;
		}
		LCD_Fill(a,y0,b,y0,POINT_COLOR);
		return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2)
	{
		last = y1; 
	}
  	else
	{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) 
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    	sb += dx02;
    	if(a > b)
   	 	{
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,POINT_COLOR);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,POINT_COLOR);
	}
}

/*****************************************************************************
 * @name       :void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Display a single English character
 * @parameters :x:the beginning x coordinate of the Character display position
                y:the beginning y coordinate of the Character display position
				fc:the color value of display character
				bc:the background color of display character
				num:the ascii code of display character(0~94)
				size:the size of display character
				mode:0-no overlying,1-overlying
 * @modified   :Ericson
  * @from      :LCDWiki
******************************************************************************/ 
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode)
{  
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;      	   
	num=num-' ';
	LCD_SetWindows(x,y,x+5,y+11);
	if(!mode)
	{		
		for(pos=0;pos<12;pos++)
		{
			temp=asc2_1206[num][pos];
			for(t=0;t<6;t++)
		    {                 
		        if(temp&0x01)Lcd_WriteData_16Bit(fc); 
				else Lcd_WriteData_16Bit(bc); 
				temp>>=1; 				
		    }			
		}	
	}else
	{
		for(pos=0;pos<12;pos++)
		{
			temp=asc2_1206[num][pos];
			for(t=0;t<6;t++)
		    {   
				POINT_COLOR=fc;              
		        if(temp&0x01)GUI_DrawPoint(x+t,y+pos); 
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	LCD_SetWindows(0,0,479,319);
}

/*****************************************************************************
 * @name       :void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
 * @date       :2018-08-09 
 * @function   :Display string
 * @parameters :x:the beginning x coordinate of the string
                y:the beginning y coordinate of the string
				p:the start address of the string
				size:the size of display character
				mode:0-no overlying,1-overlying
 * @author     :LCDWiki
******************************************************************************/   	  
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,mode);
        x+=size/2;
        p++;
    }  
} 

/*****************************************************************************
 * @name       :void print_str4_v2(u16 x,u16 y,u16 fc,u16 bc,u8 *p,uint8_t size_d)
 * @date       :2023 
 * @function   :Display string
 * @parameters :x:the beginning x coordinate of the string
                y:the beginning y coordinate of the string
				fc: foreground (text) color
				bc: background color
				*p: pointer to the string being sent
				size_d: size of the string sent
 * @author     :Ericson
******************************************************************************/   	  
void print_str4_v2(u16 x,u16 y,u16 fc,u16 bc,u8 *p,uint8_t size_d)
{
	uint16_t gg = x;
	uint8_t size_aux = size_d;
	uint8_t indice = 0;
	if(size_d==0)return;
	while(*p!=0)
	{
		if(*p==0xc3)p++;
		GUI_DrawFont16x20(gg,y,fc,bc,*p);
		p++;
		gg+=17;
		size_aux--;
		if(size_aux==0)break;
	}
	if (size_aux>0)
	{
		for(indice=0;indice<size_aux;indice++)
		{
			GUI_DrawFont16x20(gg,y,fc,bc,' ');
			gg+=17;
		}
	}
}


/*****************************************************************************
 * @name       :u32 mypow(u8 m,u8 n)
 * @date       :2018-08-09 
 * @function   :get the nth power of m (internal call)
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
 * @author     :LCDWiki
******************************************************************************/ 
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*****************************************************************************
 * @name       :void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
 * @date       :2018-08-09 
 * @function   :Display number
 * @parameters :x:the beginning x coordinate of the number
                y:the beginning y coordinate of the number
				num:the number(0~4294967295)
				len:the length of the display number
				size:the size of display number
 * @author     :LCDWiki
******************************************************************************/  			 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',0); 
	}
} 


/*****************************************************************************
 * @name       :void GUI_DrawFont16x20(u16 x, u16 y, u16 fc, u16 bc, u8 s)
 * @date       :2021-06-11 
 * @function   :Display a single 16x20 character
 * @parameters :x:the beginning x coordinate of the character
                y:the beginning y coordinate of the character
				fc:the color value of character
				bc:the background color of character
				s:character
 * @modified   :Ericson
  * @from      :LCDWiki 
******************************************************************************/ 
void GUI_DrawFont16x20(u16 x, u16 y, u16 fc, u16 bc, u8 s)
{
	u8 i,j;
	uint8_t aux_ascii = 0;
	u8 k = 0;
	// ASCII Table:
	// https://design215.com/toolbox/ascii-utf8.php

	// Special characters
	if((s < 32) || (s > 126))
	{
		aux_ascii = get_ascii(s);
		if(aux_ascii>0)
			s = aux_ascii;
		else
			return;
	}

	// Normal characters
	k = s-32;
	LCD_SetWindows(x,y,x+16-1,y+20-1);
	for(i=0;i<40;i++)
	{
		for(j=0;j<8;j++)
		{
			if(tfont16x20[k][i]&(0x80>>j))	Lcd_WriteData_16Bit(fc);
			else Lcd_WriteData_16Bit(bc);
		}
	}
	POINT_COLOR = bc;
	LCD_DrawLine(x+16,y,x+16,y+19);
	LCD_SetWindows(0,0,479,319);
}

