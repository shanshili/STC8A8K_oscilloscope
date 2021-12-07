/*
  串口打印配置
	串口1 模式1
	波特率9600
	波特率发生器：定时器1
*/

#include "UART.H"
#include "8a8k.h"

void UartInit(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率 串口1 模式1
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

char putchar(char c)
{
    ES=0;
	SBUF = c;  
	TI = 0; 
	while(TI == 0);
	TI = 0; 
	ES=1;
	
	return c;
}
