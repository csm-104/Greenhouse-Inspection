#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"

unsigned char FLAG,temp,comdata;
unsigned char RH_data_H_temp,RH_data_L_temp,T_data_H_temp,T_data_L_temp,checkdata_temp;
unsigned char RH_data_H,RH_data_L,T_data_H,T_data_L,checkdata;
sbit OneWire=P1^0;
void COM(void)
{
    unsigned char i; 
    
    for(i=0;i<8;i++)	   
    {
        FLAG=2;	
        while((!OneWire)&&FLAG++);
        Delay_10us();
        Delay_10us();
        Delay_10us();
        temp=0;
        if(OneWire)temp=1;
        FLAG=2;
        while((OneWire)&&FLAG++);//��ʱ������forѭ��	
        	  
        if(FLAG==1)break;//�ж�����λ��0����1,����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1	                  
         
        comdata<<=1;
        comdata|=temp;        
     }
}

void Get_Tem(void)
{
   //��������18ms 
   OneWire=0;
   Delay(20);
   OneWire=1;
   //������������������ ������ʱ20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
   //������Ϊ���� �жϴӻ���Ӧ�ź� 
   OneWire=1;
   //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
   if(!OneWire)		 //T !	  
   {
       FLAG=2;
       //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
       while((!OneWire)&&FLAG++);
       FLAG=2;
       //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
       while((OneWire)&&FLAG++);
       //���ݽ���״̬		 
       COM();
       RH_data_H_temp = comdata;
       COM();
       RH_data_L_temp = comdata;
       COM();
       T_data_H_temp = comdata;
       COM();
       T_data_L_temp = comdata;
       COM();
       checkdata_temp = comdata;
       OneWire=1;
     
       //����У��      
       temp=(T_data_H_temp+T_data_L_temp+RH_data_H_temp+RH_data_L_temp);
       if(temp==checkdata_temp)
       {
          RH_data_H = RH_data_H_temp;
          RH_data_L = RH_data_L_temp;
          T_data_H = T_data_H_temp;
          T_data_L = T_data_L_temp;
          checkdata = checkdata_temp;
       }
   }

}

void Show_Tem(void)
{
    Get_Tem();
    LCD_ShowString(2,1,"T:");
    LCD_ShowNum(2,3,T_data_H,2);
    LCD_ShowNum(2,6,T_data_L,1);
    LCD_ShowString(2,5,".");
    LCD_ShowString(2,7,"C");
    LCD_ShowString(2,9,"H:");
    LCD_ShowNum(2,11,RH_data_H,2);
    LCD_ShowNum(2,14,RH_data_L,1);
    LCD_ShowString(2,13,".");
    LCD_ShowString(2,15,"%");
}

void Judge_Tem(void)
{
    unsigned char temp;
    temp = T_data_H+T_data_L;  
    
    if(temp<12.5){P2_2=0;P2_3=1;}
    else if(temp>30){P2_2=1;P2_3=0;}
    else{P2_2=1;P2_3=1;}
}

void Judge_RH(void)
{
    unsigned char temp;
    temp = RH_data_H+RH_data_L;
    
    if(temp<20){P2_4=0;P3_7=1;}
    else if(temp>80){P2_4=1;P3_7
    =0;}
    else{P2_4=1;P3_7=1;}
}