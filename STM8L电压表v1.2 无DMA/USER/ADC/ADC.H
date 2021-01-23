#ifndef __ADC_H
#define __ADC_H

#include "IOSTM8L051F3.h"

#define u8 unsigned char
#define u16 unsigned int


#define ADC_CH14        14
#define ADC_CH15        15
#define ADC_CH16        16
#define ADC_CH17        17



void ADC_Config(void);
void ADC_ValueChange(void);

u16 GetADC14_Value(void);
void ADC_Task(void);
void ADC_Event(void);

#endif /* __STM8L15x_CLK_H */




