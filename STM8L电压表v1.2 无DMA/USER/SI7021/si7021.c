#include "SI7021.H"


static void SI7021_Delay(unsigned int time)
{
  	while(time--);
}

void SI7021_GPIO_Config(void)
{
	PC_DDR |= 0X03;
	PC_CR1 |= 0X03;	//开漏输出
	PC_CR2 |= 0X03;
	
	PC_ODR |= 0X03;
}

void I2C_Star(void)
{
  	I2C_SDA_HIGH;
	SI7021_Delay(5);
	I2C_SCL_HIGH;
	SI7021_Delay(5);
	I2C_SDA_LOW;
	SI7021_Delay(5);
	I2C_SCL_LOW;
}

void I2C_Stop(void)
{
  	I2C_SCL_LOW;	
	SI7021_Delay(5);
	I2C_SDA_LOW;
	SI7021_Delay(5);
	I2C_SCL_HIGH;
	SI7021_Delay(5);
	I2C_SDA_HIGH;	
}

static void I2C_Ask(void)
{
	I2C_SDA_LOW; 
	SI7021_Delay(5); 
	I2C_SCL_LOW; 
	SI7021_Delay(5); 
	I2C_SCL_HIGH;
	SI7021_Delay(5);
	I2C_SCL_LOW; 	
}

static void I2C_NoAsk(void)
{
	I2C_SCL_LOW; 
	SI7021_Delay(5); 
	I2C_SDA_HIGH; 
	SI7021_Delay(5); 
	I2C_SCL_HIGH;
	SI7021_Delay(5);
	I2C_SCL_LOW; 
}

void I2C_WriteByte(unsigned char dat)
{
	unsigned char i=0;

	I2C_SCL_LOW; 
	SI7021_Delay(5); 
	for( i=0; i < 8; i++ ) 
	{ 
		if( dat & 0x80 ) 
			I2C_SDA_HIGH;  
		else 
			I2C_SDA_LOW;  

		SI7021_Delay(5);     
		I2C_SCL_HIGH;       
		SI7021_Delay(5); 
		I2C_SCL_LOW; 


		dat <<= 1;
	}	
}

unsigned char I2C_ReadByte(char ack)
{
	u8 read_value=0;
	static char recv_ctn = 0;

	//I2C_SCL_HIGH; 
	I2C_SCL_LOW;
	SI7021_Delay(5);
	I2C_SDA_HIGH;
	SI7021_Delay(5); 
	for ( recv_ctn = 0; recv_ctn < 8; recv_ctn++ )
	{          
		 read_value <<= 1;
	     I2C_SCL_HIGH;       
	     SI7021_Delay(5);
		 //SI7021_Delay();
	     if ( I2C_SDA_State )
			read_value +=1;
	     I2C_SCL_LOW;       

		 SI7021_Delay(5);
	}	
	if(recv_ctn == 8)
	{
		recv_ctn = 0;
	}
	if(!ack)
	{
		I2C_NoAsk();
	}
	else
	{
		I2C_Ask();
	}
	return (read_value);	
}



unsigned char I2C_Wait_Ack()
{
  	unsigned char ucErrTime;
	SI7021_Delay (10);	
	I2C_SDA_HIGH;
	SI7021_Delay(5);
	//I2C_SDA_IN;	
	I2C_SCL_HIGH;
	SI7021_Delay(5);
	
	while(I2C_SDA_State)//BIT_ADDR(GPIOB_IDR_Addr,n)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
		SI7021_Delay(5);
	}
	I2C_SCL_LOW;//时钟输出0 	   
	return 0;  
} 

void SI7021_Transform(u8 mode)
{
	I2C_Star();
	I2C_WriteByte(SALVE_ADDR);
	if(I2C_Wait_Ack())
	{
		I2C_Stop();
		return;
	}
	
	I2C_WriteByte(mode);
	if(I2C_Wait_Ack())
	{
		I2C_Stop();
		return;
	}	
}
u16 SI7021_GetValue(void)
{
  	u16 output;
	//延时20ms等待转换完成
	I2C_Star();
	I2C_WriteByte(SALVE_ADDR + 1);
	if(I2C_Wait_Ack())
	{
		//USART_SendData8(USART1, 0x32);
		I2C_Stop();
		return 0xffff;
	}	
	SI7021_Delay (10);	
	//I2C_SDA_HIGH;
	output = I2C_ReadByte(1);
	output <<= 8;
	SI7021_Delay (10);	
	output |= I2C_ReadByte(0);
	
	I2C_Stop();
	return output;
}








