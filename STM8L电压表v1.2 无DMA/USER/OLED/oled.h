#ifndef __OLED_H
#define __OLED_H

#include "user.H"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 

#define OLED_CMD  0X00	//Ð´ÃüÁî
#define OLED_DATA 0X40	//Ð´Êý¾Ý

#define OLED_SCLK_Clr() PC_ODR_ODR1 = 0//SCL
#define OLED_SCLK_Set() PC_ODR_ODR1 = 1  

#define OLED_SDIN_Clr() PC_ODR_ODR0 = 0//SDA
#define OLED_SDIN_Set() PC_ODR_ODR0 = 1



//void I2C_GPIO_Config(void);
void OLED_Init(void);

void OLED_Clear(void);
void OLED_Display_Off(void);
void OLED_Display_On(void);
void OLED_Set_Pos(unsigned char x, unsigned char y); 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size);

















#endif 




