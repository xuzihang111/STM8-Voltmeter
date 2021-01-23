#include "UART.H"



void UART1_Config(void)
{    
    USART1_CR1 = 0x00;  //
    
    USART1_CR3 = 0x00;  //
    USART1_CR4 = 0x00;  //
      // ���ò����ʣ�����ע�����¼��㣺
      // (1) ������дBRR2
      // (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
      // (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ
      // ����0λ
      // ������ڲ�����λ9600ʱ����Ƶϵ��=1600 0000(��Ƶ)/9600=1667
      // ��Ӧ��ʮ��������Ϊ00D0��BBR1=0D,BBR2=00    

    USART1_BRR2 = 0x03;
    USART1_BRR1 = 0x68;    
    
    USART1_CR2 = 0x08;  //b3����ʹ��
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


















