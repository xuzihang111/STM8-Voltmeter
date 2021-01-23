#ifndef __UART_H
#define __UART_H

#include "user.H"
 


void UART1_Config(void);
void UART1_SendByte(unsigned char data);
void UART1_SendString(unsigned char * str);

#endif


























