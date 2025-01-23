//////////////////////////////////////////////////////////////////////////////////	 
//LCD primitives control functions
//Some functions are modified from the original LCDWiki implementation.
//http://www.lcdwiki.com/
/****************************************************************************************************/
#include "TFT.h"
#include "stdlib.h" 	   

_lcd_dev lcddev;
extern SPIClass * hspi;
uint8_t data_color[980];
u16 POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;

/*****************************************************************************
 * @name       :void SPI_Transmit_buffer(uint8_t* data,uint16_t size_buffer)
 * @date       :2023
 * @function   :Transmits SPI buffer for long rows
 * @parameters :data:Pointer to the data array being sent
 * 				size_buffer: Size of the data array
 * @author     :Ericson
******************************************************************************/

void SPI_Transmit_buffer(uint8_t* data,uint16_t size_buffer)
{
	uint16_t i = 0;
	for(i=0;i<size_buffer;i++)
	{
		hspi->transfer(data[i]);
	}
}

/*****************************************************************************
 * @name       :void SPI_WriteByte(uint8_t spidata)
 * @date       :2018-08-09 
 * @function   :Write a byte of data using ESP32's hardware SPI
 * @parameters :spidata: Data to be sent
 * @modified   :Ericson
  * @from      :LCDWiki
******************************************************************************/
void SPI_WriteByte(uint8_t spidata)
{
	hspi->transfer(spidata);
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the TFT screen
 * @parameters :data:Command value to be written
 * @author     :LCDWiki
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
   LCD_CS_CLR;     
   LCD_RS_CLR;
   SPI_WriteByte(data);
   LCD_CS_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @author     :LCDWiki
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteByte(data);
   LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @author     :LCDWiki
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @author     :LCDWiki
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the TFT screen
 * @parameters :Data:Data to be written
 * @author     :LCDWiki
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{
	LCD_CS_CLR;
	LCD_RS_SET;
	SPI_WriteByte(Data>>8);
	SPI_WriteByte(Data);
	LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled TFT screen
 * @parameters :color:Filled color
 * @modified   :Ericson
  * @from      :LCDWiki
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	uint16_t i,m;
	uint8_t ab,bc,cd;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   
	LCD_CS_CLR;
	LCD_RS_SET;

	ab = Color>>8;
	bc = Color;
	for(m=0;m<640;m+=2)
	{
		data_color[m] = ab;
		data_color[m+1] = bc;
	}
	for(i=0;i<480;i++)
	{
		SPI_Transmit_buffer(data_color,640);
	}

	 LCD_CS_SET;
} 

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @author     :LCDWiki
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay(100);
	LCD_RST_SET;
	delay(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @modified   :Ericson
  * @from      :LCDWiki
******************************************************************************/	 	 
void LCD_Init(void)
{  
	LCD_RS_SET;
	LCD_CS_SET;
 	LCD_RESET(); 
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0xC3);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x96);
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x68);	
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x05);	
	LCD_WR_REG(0xB0);
	LCD_WR_DATA(0x80);	
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x02);	
	LCD_WR_REG(0xB5);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x80);	
	LCD_WR_DATA(0x10);	
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0xC6);
	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x24);
	LCD_WR_REG(0xE4);
	LCD_WR_DATA(0x31);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x40);
	LCD_WR_DATA(0x8A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0xA5);
	LCD_WR_DATA(0x33);
	LCD_WR_REG(0xC2);
	LCD_WR_REG(0xA7);
	
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0xF0);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1D);
	LCD_WR_DATA(0x21);

	LCD_WR_REG(0XE1);
	LCD_WR_DATA(0xF0);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x3B);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x4D);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1D);
	LCD_WR_DATA(0x21);

  	LCD_WR_REG(0X36);
	LCD_WR_DATA(0xEC);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0xC3);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA(0x69);
	LCD_WR_REG(0X13);
	LCD_WR_REG(0X11);
	LCD_WR_REG(0X29);
	
  	LCD_direction(1);
  	digitalWrite(4,HIGH);
}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the beginning x coordinate of the LCD display window
				yStar:the beginning y coordinate of the LCD display window
				xEnd:the ending x coordinate of the LCD display window
				yEnd:the ending y coordinate of the LCD display window
 * @author     :LCDWiki
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();		
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
				Ypos:the  y coordinate of the pixel
 * @author     :LCDWiki
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
						  2-180 degree
						  3-270 degree
 * @author     :LCDWiki
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(1<<6));
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5));
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7));
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
		break;	
		default:break;
	}		
}	 
