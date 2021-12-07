#ifndef __OLED_H
#define __OLED_H	
#include "8a8k.h"
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
	
void delay_ms(unsigned int ms);

//--------------OLED参数定义---------------------
#define PAGE_SIZE    8   //循环数
#define XLevelL		   0x00  //CMD:Set Lower Column Start Address for Page Addressing Mode 
#define XLevelH		   0x10  //CMD:Set Higher Column Start Address for Page Addressing Mode  //都是从0000开始
#define YLevel       0xB0  //页地址指令   1011 0000 
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	

//-------------写命令和数据命令-------------------
#define OLED_CMD     0	//写命令
#define OLED_DATA    1	//写数据 
   						  
//-----------------OLED端口定义----------------
sbit OLED_CS = P1^3;    //片选信号         P13
sbit OLED_DC = P1^2;   //数据/命令控制信号 P12
sbit OLED_RST = P3^3;   //复位信号         P33

//-----------------OLED端口操作定义---------------- 
#define OLED_CS_Clr()  OLED_CS = 0
#define OLED_CS_Set()  OLED_CS = 1

#define OLED_DC_Clr()  OLED_DC = 0
#define OLED_DC_Set()  OLED_DC = 1
 					   
#define OLED_RST_Clr()  OLED_RST = 0
#define OLED_RST_Set()  OLED_RST = 1

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);     							   		    
//void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Reset(void);	   							   		    
void OLED_Init(void);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);  


/*
使用软件模拟SPI接口驱动
*/
sbit OLED_MOSI = P1^5;   //OLED屏SPI写数据引脚 P15  D1
sbit OLED_CLK = P1^7;    //OLED屏SPI时钟引脚   P17  D0

#define OLED_MOSI_Clr()  OLED_MOSI = 0
#define OLED_MOSI_Set()  OLED_MOSI = 1

#define OLED_CLK_Clr()   OLED_CLK = 0
#define OLED_CLK_Set()   OLED_CLK = 1


#endif