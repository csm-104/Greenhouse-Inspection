#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "GY30.h"
#include "Tem.h"
#include "Timer0.h"

void main()
{
    LCD_Init();
    Delay(100);	    //—” ±100ms	
    Init_BH1750();       //≥ı ºªØBH1750

    while(1)
	 {
        Show_Light();
        Show_Tem();
        Judge_RH();
        Judge_Tem();
        Judge_Lx();
	 }		 
}
