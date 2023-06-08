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
        while((OneWire)&&FLAG++);//超时则跳出for循环	
        	  
        if(FLAG==1)break;//判断数据位是0还是1,如果高电平高过预定0高电平值则数据位为 1	                  
         
        comdata<<=1;
        comdata|=temp;        
     }
}

void Get_Tem(void)
{
   //主机拉低18ms 
   OneWire=0;
   Delay(20);
   OneWire=1;
   //总线由上拉电阻拉高 主机延时20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
   //主机设为输入 判断从机响应信号 
   OneWire=1;
   //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
   if(!OneWire)		 //T !	  
   {
       FLAG=2;
       //判断从机是否发出 80us 的低电平响应信号是否结束	 
       while((!OneWire)&&FLAG++);
       FLAG=2;
       //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
       while((OneWire)&&FLAG++);
       //数据接收状态		 
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
     
       //数据校验      
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