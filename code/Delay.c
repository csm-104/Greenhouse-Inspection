#include <REGX52.H>

void Delay(unsigned int xms)		//@12.000MHz
{
	unsigned char i, j;
	unsigned int a;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
		a = xms-1;
  }
}

void Delay5us()		//@11.0592MHz
{
	unsigned char i;	
	i = 12;
	while (--i);
}

void  Delay_10us(void)
{
    unsigned char i;
    i--;
    i--;
    i--;
    i--;
    i--;
    i--;
}
