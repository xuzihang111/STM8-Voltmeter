#include "UART.H"



void UART1_Config(void)
{    
    USART1_CR1 = 0x00;  //
    
    USART1_CR3 = 0x00;  //
    USART1_CR4 = 0x00;  //
      // 设置波特率，必须注意以下几点：
      // (1) 必须先写BRR2
      // (2) BRR1存放的是分频系数的第11位到第4位，
      // (3) BRR2存放的是分频系数的第15位到第12位，和第3位
      // 到第0位
      // 例如对于波特率位9600时，分频系数=1600 0000(主频)/9600=1667
      // 对应的十六进制数为00D0，BBR1=0D,BBR2=00    

    USART1_BRR2 = 0x03;
    USART1_BRR1 = 0x68;    
    
    USART1_CR2 = 0x08;  //b3发送使能
}



void UART1_SendByte(unsigned char data)
{
      while((USART1_SR & 0x80)==0x00);
      USART1_DR = data;
}



void UART1_SendString(unsigned char * str)
{
    while(*str != 0)
    {
        while((USART1_SR & 0x80)==0x00);
        USART1_DR = *str++;
    }
}


















