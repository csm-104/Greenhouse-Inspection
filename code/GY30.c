#include <REGX52.H>
//#include  <math.h>      
//#include  <stdio.h>   	
//#include  <INTRINS.H>
#include "Delay.h"
#include "LCD1602.h"

#define   uchar unsigned char
#define   uint unsigned int	
#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�

sbit	  SCL=P1^2;      //IICʱ�����Ŷ���
sbit  	  SDA=P1^1;      //IIC�������Ŷ���

int dis_data;
unsigned char BUF[8]; 
int Lx;
//void conversion(uint temp_data)  //  ����ת���� ����ʮ���٣�ǧ����
//{  
//    wan=temp_data/10000+0x30 ;
//    temp_data=temp_data%10000;   //ȡ������
//	qian=temp_data/1000+0x30 ;
//    temp_data=temp_data%1000;    //ȡ������
//    bai=temp_data/100+0x30   ;
//    temp_data=temp_data%100;     //ȡ������
//    shi=temp_data/10+0x30    ;
//    temp_data=temp_data%10;      //ȡ������
//    ge=temp_data+0x30; 	
//}

void BH1750_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
}

void BH1750_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

bit BH1750_RecvACK()
{
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

void BH1750_SendByte(unsigned char dat)
{
    unsigned char i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    BH1750_RecvACK();
}

unsigned char BH1750_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;

    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
  //  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    BH1750_Stop();                   //����ֹͣ�ź�
}

void Multiple_Read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
	 for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
    {
        BUF[i] = BH1750_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 3)
        {

           BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {		
          BH1750_SendACK(0);                //��ӦACK
       }
   }

    BH1750_Stop();                          //ֹͣ�ź�
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
    Delay(180);              //��ʱ180ms
    Multiple_Read_BH1750();       //�����������ݣ��洢��BUF��
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������    
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