#include <REGX52.H>
//#include  <math.h>      
//#include  <stdio.h>   	
//#include  <INTRINS.H>
#include "Delay.h"
#include "LCD1602.h"

#define   uchar unsigned char
#define   uint unsigned int	
#define	  SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改

sbit	  SCL=P1^2;      //IIC时钟引脚定义
sbit  	  SDA=P1^1;      //IIC数据引脚定义

int dis_data;
unsigned char BUF[8]; 
int Lx;
//void conversion(uint temp_data)  //  数据转换出 个，十，百，千，万
//{  
//    wan=temp_data/10000+0x30 ;
//    temp_data=temp_data%10000;   //取余运算
//	qian=temp_data/1000+0x30 ;
//    temp_data=temp_data%1000;    //取余运算
//    bai=temp_data/100+0x30   ;
//    temp_data=temp_data%100;     //取余运算
//    shi=temp_data/10+0x30    ;
//    temp_data=temp_data%10;      //取余运算
//    ge=temp_data+0x30; 	
//}

void BH1750_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}

void BH1750_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

bit BH1750_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

void BH1750_SendByte(unsigned char dat)
{
    unsigned char i;

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    BH1750_RecvACK();
}

unsigned char BH1750_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;

    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    BH1750_Stop();                   //发送停止信号
}

void Multiple_Read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	
	 for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
    {
        BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 3)
        {

           BH1750_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {		
          BH1750_SendACK(0);                //回应ACK
       }
   }

    BH1750_Stop();                          //停止信号
    Delay(5);
}

void Init_BH1750()
{
   Single_Write_BH1750(0x01);  

}

void Show_Light()
{
    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
    Delay(180);              //延时180ms
    Multiple_Read_BH1750();       //连续读出数据，存储在BUF中
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据    
    Lx=dis_data/1.2;
    
    LCD_ShowString(1,1,"Light:");
    LCD_ShowString(1,12,"Lx");
    LCD_ShowNum(1,7,Lx,5);
}

void Judge_Lx(void)
{
    if(Lx<100){P2_0=0;P2_1=1;}
    else if(Lx>8000){P2_0=1;P2_1=0;}
    else{P2_0=1;P2_1=1;}
}