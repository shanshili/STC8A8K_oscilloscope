/*
  建立代码框架：
			底层驱动SDK、
			GUI、
			数据——收发字节转换函数、
			采样数据处理、
			ADC滤波算法、
	设定刷新频率
*/

#include "8a8k.h"
#include <stdio.h>
#include <intrins.h>
#include "oled.h"
#include "uart.h"

unsigned char a[1],b[1],d=0,e=0,x,y;
unsigned char ybucketh[130],ybucketl[130];

void Delay3000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 18;
	j = 150;
	k = 212;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/*中断法
    串口3模式0
    TxD3 P0.1
		RxD3 P0.0
	  功能角映射：P_SW2=0x00
		波特率设置
		时钟
		中断方式的ADC
		
		OLED I/O口工作方式
*/
void ADC_Isr() interrupt 5 using 1 ///question：using 1？？
{
		ADC_CONTR &= ~0x20;
	  a[0]=ADC_RES;    
	  b[0]=ADC_RESL;  ///????char形数组，一个位置能放几个数
	  ADC_CONTR|=0X40;
	  d++;
}

void main() 
{ 
	 OLED_Init();	//初始化OLED  
	 P1M0 = 0x00; //设置 P1.0 为 ADC 口
	 P1M1 = 0x01;  //高阻输入
	 ADCCFG = 0x00; 
	//设置 ADC 时钟为系统时钟/2/16/0  SYSclk/2/16/SPEED  采样750000HZ 1.3us 被采最高频率375000hz
	//左对齐
	 ADC_CONTR = 0x80; //使能 ADC 模块
	 EADC=1;
	 EA=1;
	 ADC_CONTR |= 0x40; //启动 AD 转换
	 OLED_Clear(1);
	 Delay3000ms();
	 while(1)//等待中断
	 {
	     UartInit(); //串口打印
       printf("a=%x\nb=%x\n",a[0],b[0]);
		   OLED_Clear(0);
//		   OLED_Set_Pos(1,1);
/*
		 x与d(中断次数）之间的关系:128像素 0.0013ms每像素
		 
		 y与ab(采样值之间的关系)：64/8=8  像素/伏 (每伏占4个像素)(区间-1.5~6.5V)
															5/4096=0.001220703125 精度/伏   819.2伏/精度
															3276.8  像素/精度
															（以上方案废弃）
		 新方案：page[0,6:7]不显示，仅在page1~5显示
		 采样
		 存储==>要显示的
		 按照一定时间速率显示
*/		 
//________存储程序//存满一页
		  if(d==1000)
			{
				 if(e<130)
				 {
						ybucketh[e]=a[0];
					  ybucketl[e]=b[0];
//					  xbucket[e]=e;
						e++;
				 }
				 else
				 {
					 e=0;
					 ybucketh[e]=a[0];
					 ybucketl[e]=b[0];
//					 xbucket[e]=e;
					 e++;
				 }
			}
//			OLED_Clear(1);	
//________显示程序///E控制时钟，y分7页,先取出e对应y量
			/*
		 for(xpoint=0;xpoint<128;xpoint++) //选择bucket横坐标（提取存储的数据）
 		{
			 
			 ydec[xpoint]=(ybucketh[xpoint]*16+ybucketl[xpoint]) //12位二进制位转换
		   for(page=1;page<6;page++) //先选择页，从1~5
			{
				if ( ydec[xpoint]>=(819.2*(5-page)) && ydec[xpoint]<(819.2*(6-page)) ) //判断是否在本页有值
		    {
						ydeviation[xpoint] = ydec[xpoint]-(819.2*(5-page)); //取出与本页底值的差值
					  for(ypoint=0;ypoint<8;ypoint++)      //判断是本页（本字节）第几位有值 ！上l下m  //求出在本页的位置，即传输的数据
					  {
							  tage=0x01;
							  color=0x00;
								if ( ylevel[xpoint]>=(102.4*(7-ypoint)) && ylevel[xpoint]<(102.4*(8-ypoint)) )  //判断是否在本字节有值 //ypoint控制循环左移？
								{
									  color=tage;
								}
								tage<<=1; 
						}
						OLED_WR_Byte(color,OLED_DATA); 
				}
				else  //若不在本页有值
				{
						color=0x00;
						for(n=0;n<WIDTH;n++)   //填充行
						{
							OLED_WR_Byte(color,OLED_DATA); 
						}					
				}
//	      OLED_WR_Byte(color,OLED_DATA); 
		 }	
		 */
/////////////////////////////////////////////////////////////////				
				x=e;
				y=;
				if(e==129)
				{
						
				}
				else
				{
						OLED_Set_Pos(x,y);
				}
		
		}
	 }  
}
		}	
}	 
 

/*
 查询法
 tips:
 1、while(0)表示不执行语句 
 2、&：实现逐个位的检测
*/
/*
void main() 
{ 
	 P1M0 = 0x00; //设置 P1.0 为 ADC 口
	 P1M1 = 0x01;  //高阻输入
	 ADCCFG = 0x00; //设置 ADC 时钟为系统时钟/2/16/16 SYSclk/2/16/SPEED  46875HZ 被采最高频率23437.5
	 ADC_CONTR = 0x80; //使能 ADC 模块
	 while (1) 
	 { 
	 ADC_CONTR |= 0x40; //启动 AD 转换
		 _nop_(); 
		 _nop_();  //跑一个机器周期，相当于0.5us延时
		 while (!(ADC_CONTR & 0x20)); 	 
		   查询 ADC 完成标志：ADC_FLAG=1,&结果为1，!1=0, 
		   while(0)表示不执行语句 直接跳过    while(1)循环等待转换完成
			 &：实现逐个位的检测	 
		 ADC_CONTR &= ~0x20; //清完成标志
		 a = ADC_RES; //读取 ADC 结果
		 b = ADC_RESL;
		 P2=a[0];
		 Delay3000ms();
		 P2=b[0]; 
} 
*/
