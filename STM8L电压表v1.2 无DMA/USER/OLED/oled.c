#include "OLED.h"
#include "oledfont.h"

u8 Tab_OLED_Config[27] = {
  0xAE, 0x10, 0x00, 0x40, 0xB0, 0x81, 0xFF, 0xA0, 
  0xA6, 0xA8, 0x3F, 0xC0, 0xD3, 0x00, 0xD5, 0x80, 
  0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12, 0xDB, 0x30, 
  0x8D, 0x14, 0xAF, 
};

/*void I2C_GPIO_Config(void)//C0 SDA	C1 SCL
{
	GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_Out_OD_HiZ_Fast);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
}*/

static void IIC_Delay(void)
{
//    u16 i = 10;
//    while(i--);
}

static void OLED_IIC_Start(void)
{
	OLED_SCLK_Set();
        IIC_Delay();
	OLED_SDIN_Set();
        IIC_Delay();
	OLED_SDIN_Clr();
        IIC_Delay();
	OLED_SCLK_Clr();
        IIC_Delay();
}

/**********************************************
//IIC Stop
**********************************************/
static void OLED_IIC_Stop(void)
{
	OLED_SCLK_Set() ;
        IIC_Delay();
	OLED_SDIN_Clr();
        IIC_Delay();
	OLED_SDIN_Set();
        IIC_Delay();
	
}

static void OLED_IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
        IIC_Delay();
	OLED_SCLK_Clr();
        IIC_Delay();
}
/**********************************************
// IIC Write byte
**********************************************/

static void OLED_Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da = IIC_Byte;
	
	for(i=0;i<8;i++)		
	{
		m = da;
		m &= 0x80;
		OLED_SCLK_Clr();
                IIC_Delay();
		if(m==0x80)
                    OLED_SDIN_Set();
		else 
                    OLED_SDIN_Clr();
                da=da<<1;
		
		OLED_SCLK_Set();
                IIC_Delay();
	}
	OLED_SCLK_Clr();
        IIC_Delay();

}
/**********************************************
// IIC Write Command
**********************************************/
static void I2C_Write(u8 IIC_Data, u8 Addr)
{
	OLED_IIC_Start();
	OLED_Write_IIC_Byte(0x78);            //Slave address,SA0=0
	OLED_IIC_Wait_Ack();	
	OLED_Write_IIC_Byte(Addr);			//write command
	OLED_IIC_Wait_Ack();	
	OLED_Write_IIC_Byte(IIC_Data); 
	OLED_IIC_Wait_Ack();	
	OLED_IIC_Stop();
}

/**********************************************
// IIC Write Command
**********************************************/
static void I2C_WriteBuff( u8 Addr, u8 * buf, u8 len)
{
    u8 i;
    OLED_IIC_Start();
    OLED_Write_IIC_Byte(0x78);            //Slave address,SA0=0
    OLED_IIC_Wait_Ack();	
    OLED_Write_IIC_Byte(Addr);			//write command
    OLED_IIC_Wait_Ack();	
    
    for(i = 0; i < len; i++)
    {  
        OLED_Write_IIC_Byte(*buf++); 
        OLED_IIC_Wait_Ack();
    }
    
    OLED_IIC_Stop();
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	I2C_Write(0xb0+y,OLED_CMD);
	I2C_Write(((x&0xf0)>>4)|0x10,OLED_CMD);
	I2C_Write((x&0x0f),OLED_CMD); 
}  

//开启OLED显示    
void OLED_Display_On(void)
{
	I2C_Write(0X8D,OLED_CMD);  //SET DCDC命令
	I2C_Write(0X14,OLED_CMD);  //DCDC ON
	I2C_Write(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	I2C_Write(0X8D,OLED_CMD);  //SET DCDC命令
	I2C_Write(0X10,OLED_CMD);  //DCDC OFF
	I2C_Write(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		I2C_Write (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		I2C_Write (0x00,OLED_CMD);      //设置显示位置―列低地址
		I2C_Write (0x10,OLED_CMD);      //设置显示位置―列高地址   
		for(n=0;n<128;n++)
                  I2C_Write(0,OLED_DATA); 
	} //更新显示
}



u8 FontsMap(u8 ch)
{
    switch(ch)
    {
    case 'A': return 10;
    case 'a': return 10;    
    case 'I': return 11;
    case 'i': return 11;    
    case 'U': return 12;
    case 'u': return 12;    
    case 'V': return 13;
    case 'v': return 13;    
    case ':': return 14;
    case '.': return 15;
    case ' ': return 16;    
    
    default: return 0;    
    }
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size)
{      	
    unsigned int c=0,i=0,j = 0;	
    u8 dat[32];

    
    if(Char_Size ==32)
    {
        if(chr >= 0x30 && chr <= 0x39)
            c=chr - 0x30;//得到偏移后的值
        else
            c = FontsMap(chr);
        
        for(j = 0; j < 4; j++)
        {
            OLED_Set_Pos(x,y+j);
            for(i=0;i<12;i++)
            {
                dat[i] = Fonts16x32[c*48+i+12*j];
            }
            I2C_WriteBuff(OLED_DATA, dat, 12);
        }
    }
    else if(Char_Size == 16 || Char_Size == 17)
    {	
        c=chr - 0x30;//得到偏移后的值
        OLED_Set_Pos(x,y);	
        for(i=0;i<7;i++)
        {
            j = c*14+i;
            if(Char_Size == 16)
                dat[i] = Fonts6x12A[j];
            else
                dat[i] = Fonts6x12C[j];
        }
        I2C_WriteBuff(OLED_DATA, dat, 7);

        OLED_Set_Pos(x,y+1);
        for(i=0;i<7;i++)
        {
            j = c*14+i+7;
            if(Char_Size == 16)
                dat[i] = Fonts6x12A[j];
            else
                dat[i] = Fonts6x12C[j];
        }
        I2C_WriteBuff(OLED_DATA, dat, 7);
    }
}



void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
    unsigned char i, j=0;
    if(Char_Size == 32)
        i = 12;
    else if(Char_Size >= 16)
        i = 7;
    while (chr[j]!='\0')
    {		
        OLED_ShowChar(x,y,chr[j],Char_Size);
        x+=i;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}






//初始化SSD1306					    
void OLED_Init(void)
{ 	
    u8 i;
    for(i = 0; i < 27; i++)
    {
        I2C_Write(Tab_OLED_Config[i],OLED_CMD);
    }
    OLED_Clear();
}  














