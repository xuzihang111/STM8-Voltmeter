#ifndef __SI7021_H
#define __SI7021_H

#include "IOSTM8L051F3.h"

#define u8 unsigned char
#define u16 unsigned int

#define I2C_SDA_HIGH	(PC_ODR |= 0X01)
#define I2C_SDA_LOW		(PC_ODR &= 0Xfe)
#define I2C_SCL_HIGH	(PC_ODR |= 0X02)
#define I2C_SCL_LOW		(PC_ODR &= 0Xfd)
#define I2C_SDA_State	(PC_IDR & 0X01)

#define	WRITE_CMD		0x80
#define	READ_CDM		0x81
#define	SALVE_ADDR		0x80

#define	HUMI_HOLD_MASTER	0xE5
#define	TEMP_HOLD_MASTER	0xE3
		
#define	HUMI_NOHOLD_MASTER	0xF5
#define	TEMP_NOHOLD_MASTER	0xF3

void SI7021_GPIO_Config(void);
void SI7021_Transform(u8 mode);
u16 SI7021_GetValue(void);

#endif /* __STM8L15x_CLK_H */















