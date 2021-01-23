#ifndef __STM8L_SPI_H
#define __STM8L_SPI_H

#include "IOSTM8L051F3.h"

#define u8 unsigned char
#define u16 unsigned int

#define NNS_HIGH (PB_ODR |= 0X10)
#define NNS_LOW (PB_ODR &= 0XEF)



void SPI_Config(void);
void SPI_WriteByte(unsigned char data);
unsigned char SPI_ReadByte(void);
u8 SPI_RW(u8 data);
void SPI_WriteReg(u8 addr, u8 data);
u8 SPI_ReadReg(u8 addr);
void delay(unsigned int time);

#endif /* __STM8L15x_CLK_H */




