#ifndef __USER_H
#define __USER_H


#include "IOSTM8L051F3.h"
#include "ADC.H"
#include "KEY.H"
#include "TIM.H" 
#include "StringTool.H"
#include "OLED.h"


#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

#define TEST    PC_ODR_ODR6




typedef struct _Bit
{
	unsigned char Unpack_Flag      : 1;
	unsigned char Send_EN	       : 1;
	unsigned char Send_Over	       : 1;
	unsigned char Shot_Down	       : 1;
	unsigned char _1ms	       : 1;
        unsigned char powerOn	       : 1;
	unsigned char relayTest	       : 1;
        unsigned char shortCircuit     : 1;
        unsigned char ADC_CH14_Ok      : 1;
        unsigned char ADC_CH16_Ok      : 1;
        unsigned char ADC_CH17_Ok      : 1;
        
        unsigned char key1Down         : 1;
        unsigned char key1Click        : 1;
        unsigned char key1Long         : 1;
        unsigned char key1Lock         : 1;
        
}BIT;

extern BIT Bit;

void SYS_Clock_Config(void);
void GPIO_Config(void);
void GPIO_Key_Config(void);



#endif










