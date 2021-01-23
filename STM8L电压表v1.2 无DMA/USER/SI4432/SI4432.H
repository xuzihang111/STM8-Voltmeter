#ifndef __SI4432_H
#define __SI4432_H

#include "IOSTM8L051F3.h"

#define u8 unsigned char
#define u16 unsigned int


#define SDN_HIGH (PB_ODR |= 0X04)
#define SDN_LOW (PB_ODR &= 0XFb)

#define IRQ_STATE ((PB_IDR & 0X08))


void SI4432_Config(void);
u16 Get_IRQ_State(void);
void SI4432_Send(u8* buf, u8 len);
u8 SI4432_Recv(u8 * buf, u8 len);
void SI4432_ClrBuf(void);
void SI4432_Reset(void);

#endif /* __STM8L15x_CLK_H */




