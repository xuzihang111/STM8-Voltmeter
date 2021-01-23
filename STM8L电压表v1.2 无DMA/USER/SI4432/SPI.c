#include "SPI.H"



void SPI_GPIO_Config(void)
{
	PB_DDR |= 0X74;
	PB_DDR &= 0X77;
	PB_CR1 |= 0Xfc;
	PB_CR2 |= 0X70;
	PB_CR2 &= 0X7f;
	
	PB_ODR |= 0Xfc;	
}

void SPI_Config(void)
{
	SPI1_CR1 = 0X2c;
	SPI1_CR2 = 0X03;
	SPI1_CR1_SPE = 1;
	SPI_GPIO_Config();
}

void SPI_WriteByte(u8 data)
{
	SPI1_DR = data;
}

u8 SPI_ReadByte(void)
{
	return SPI1_DR;
}

u8 SPI_RW(u8 data)
{
  	u8 recv;
	
	NNS_LOW;
	while(!SPI1_SR_TXE);	
	SPI1_DR = data;
	while(!SPI1_SR_RXNE);
	recv = data;
	NNS_HIGH;
	
	return recv;
}


void SPI_WriteReg(u8 addr, u8 data)
{
  	u8 recv;
	
	NNS_LOW;
	addr |= 0x80;
	while(!SPI1_SR_TXE);	
	SPI1_DR = addr;
	while(!SPI1_SR_RXNE);
	recv = SPI1_DR;

	while(!SPI1_SR_TXE);	
	SPI1_DR = data;
	while(!SPI1_SR_RXNE);
	recv = SPI1_DR;	
	delay(10);
	NNS_HIGH;
}


u8 SPI_ReadReg(u8 addr)
{
  	u8 recv;
	
	NNS_LOW;
	addr &= 0x7f;	
	while(!SPI1_SR_TXE);	
	SPI1_DR = addr;
	while(!SPI1_SR_RXNE);
	recv = SPI1_DR;

	while(!SPI1_SR_TXE);	
	SPI1_DR = 0xff;
	while(!SPI1_SR_RXNE);
	recv = SPI1_DR;	
	
	delay(10);
	NNS_HIGH;
	
	return recv;
}


void delay(unsigned int time)
{
  	while(time--);
}
