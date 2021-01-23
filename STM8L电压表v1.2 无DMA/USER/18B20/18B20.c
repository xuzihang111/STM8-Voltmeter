#include <math.h>		
#include <18B20.H>
#include "IOSTM8L051F3.h"


void Delay15us(void)		
{
	unsigned char i;
	i = 5;
	while (--i);
}

void Delay30us(void)		
{
	unsigned char i;
	i = 12;
	while (--i);
}

void Delay60us(void)		
{
	unsigned char i;
	i = 27;
	while (--i);
}


void Delay470us(void)		
{
	unsigned char i;
	i = 220;
	while (--i);
}


void Delay750us(void)		
{
	unsigned int i;
	for(i=0;i<50;i++)
		Delay15us();
}


void Star18b20(void)
{
  	//DS_OUTPUT;
	DS_HIGH;
	Delay15us();
	DS_LOW;
	Delay750us();
	DS_HIGH;
	Delay470us();
}

void DS18B20_WriteData(uchar dat)
{
	uchar i;
	
	DS_OUTPUT;
	
	for(i=0;i<8;i++)
	{
	  	DS_LOW;DS_LOW;DS_LOW;
		
		if(dat&0x01)
			DS_HIGH;
		else
		  	DS_LOW;
		
		Delay60us();
		DS_HIGH;
		dat=dat>>1;
	}
}

uchar DS18B20_ReadData(void)
{
	uchar i,dat=0;
	
	for(i=0;i<8;i++)
	{
		DS_LOW;DS_LOW;DS_LOW;
		
		dat >>= 1;
		DS_HIGH;DS_HIGH;
		
		if(DS_GET_STATE == 1) 	
		  	dat|=0x80;

		Delay60us();
	}
	return (dat);
}

void DS18B20_ReadTemperature(void)
{
	Star18b20();
	Delay30us();
	DS18B20_WriteData(Skip_ROM);
	DS18B20_WriteData(Read_temperature);											  
}

int DS18B20_Calculate_Temperature(void)
{
	int temp;
	float f_temp;
	uchar H,L;
	
	Star18b20();
	DS18B20_WriteData(Skip_ROM); 
	DS18B20_WriteData(Read_memory);
	H = DS18B20_ReadData();
	L = DS18B20_ReadData();
	temp=L;
	temp <<= 8;
	temp = temp|H;
	f_temp = temp * 0.0625;
	temp  = f_temp * 100 + 0.5;
	
	return temp;
}


void DS18B20_Init(void)
{
	GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_OD_HiZ_Fast);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);	
		
}

char *Turn_ToString(int Temperature)
{
	static char Buf[7];
	uchar n1,n2,n3,n4;
	
	n1 = 0x30 + (Temperature / 1000);
	n2 = 0x30 + (Temperature / 100 % 10);
	n3 = 0x30 + (Temperature /10 % 10);
	n4 = 0x30 + (Temperature % 10);
	
	if(Temperature > 0)
	{
		Buf[0] = n1;
		Buf[1] = n2;
		Buf[2] = '.';
		Buf[3] = n3;
		Buf[4] = n4;
		Buf[5] = 0X30;
	}
	else
	{
		Buf[1] = n1;
		Buf[2] = n2;
		Buf[3] = '.';
		Buf[4] = n3;
		Buf[5] = n4;
		Buf[0] = '-';		
	}

	Buf[6] = 0;
	return Buf;
}













