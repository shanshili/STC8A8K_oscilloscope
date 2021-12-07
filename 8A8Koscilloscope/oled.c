#include "oled.h"
#include "stdlib.h"
#include "string.h" 	 

/*OLED显存总共分为8页
  每页8行
	OLED的显存
	存放格式如下
	[0]0 1 2 3 ... 127 (0~7)行   
	[1]0 1 2 3 ... 127 (8~15)	
	[2]0 1 2 3 ... 127 (16~23)	
	[3]0 1 2 3 ... 127 (24~31)	
	[4]0 1 2 3 ... 127 (32~39)
	[5]0 1 2 3 ... 127 (40~47)
	[6]0 1 2 3 ... 127 (48~55)	
	[7]0 1 2 3 ... 127 (56~63)
*/	

//system
void delay_ms(unsigned int ms)
{                         
  int i,j;
  for(i=0;i<ms;i++)
     for(j=0;j<120;j++);
}

//SPI操作函数
void SPI_WriteByte(unsigned char byte)    /*SPI字节写函数*///把data微观成针脚上每个时钟每个高低电平
{	
	unsigned char i;
  for(i=0;i<8;i++)
	{			  
		if(byte&0x80)    //按位与,从最高位开始判断，高电平写高，低电平写低
		{
		  OLED_MOSI_Set();  //数据高电平
		}
		else
		{
		  OLED_MOSI_Clr();  //数据低电平
		}
		OLED_CLK_Clr();    //时钟0
		OLED_CLK_Set();    //时钟1
		byte<<=1;          //一个时钟写一位
	}				 		   
}

//OLED操作函数

void OLED_WR_Byte(unsigned dat,unsigned cmd)   /*字节写函数*/
{
	if(cmd)  
	{
		OLED_DC_Set();  //数据
	}
	else  ///cmd必为0，写命令
	{
		OLED_DC_Clr();   //命令
	}
	OLED_CS_Clr();    //低电平使能
	SPI_WriteByte(dat);
	OLED_CS_Set();
}

void OLED_Set_Pos(unsigned char x, unsigned char y) ////Set coordinates in the OLED screen
{
 	OLED_WR_Byte(YLevel+y,OLED_CMD);  //页地址指令
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);  
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}  

void OLED_Clear(unsigned dat)   /*清屏函数*/
{ 
	u8 i,n,color;	
	if(dat)
	{
		color = 0xff;
	}
	else
	{
		color = 0;
	}
	for(i=0;i<PAGE_SIZE;i++)    //页扫描
	{  
		OLED_WR_Byte(YLevel+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte(XLevelL,OLED_CMD);     //设置显示位置—列低地址
		OLED_WR_Byte(XLevelH,OLED_CMD);     //设置显示位置—列高地址
		for(n=0;n<WIDTH;n++)   //填充行
		{
			OLED_WR_Byte(color,OLED_DATA); 
		}
	}   //更新显示
}

void OLED_Reset(void)     /*复位函数*/
{
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
}	

void OLED_Init(void)   /*初始化函数*/
{
	OLED_Reset();     //复位OLED

/**************³初始化SSD1306*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0xFF,OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0XF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); /*set charge pump disable*/ 
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}  


