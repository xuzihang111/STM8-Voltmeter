#ifndef __18B20_H__
#define __18B20_H__

#include "stm8l15x.h"

#define uchar unsigned char
#define uint  unsigned int

#define	DS_OUTPUT	GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_OD_HiZ_Fast);
#define	DS_INPUT	GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_In_PU_No_IT);

#define DS_HIGH  PC_ODR |= 0x10
#define DS_LOW   PC_ODR &= 0xEF
#define DS_GET_STATE	PC_IDR_IDR4

#define Read_ROM    		0X33
#define Conform_ROM 		0X55
#define Search_ROM  		0Xf0
#define Skip_ROM    		0Xcc
#define The_alarm   		0Xec
#define Read_temperature 	0X44
#define Read_memory 		0Xbe
#define write_memory 		0X55

void DS18B20_ReadTemperature(void);
void DS18B20_Init(void);
int DS18B20_Calculate_Temperature(void);
char *Turn_ToString(int Temperature);
void Delay470us(void);

#endif

