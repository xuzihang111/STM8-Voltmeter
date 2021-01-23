#include "user.H"



void SoftDelay(u32 t);

BIT Bit = {0};

extern u16 ADC_Current, ADC_Base, ADC_Power;
extern float powerCurrent, pwoerVoltage;

u8  reg_3ms = 0;
u16 reg_100ms = 0;
u8 displayBuff[30] = {0};
u8 len;

int main( void )
{
    u16 i;
        
    SYS_Clock_Config();
    SoftDelay(0x1000);
    
    GPIO_Config();

    TIM4_Config(0x05, 125);

    
    SoftDelay(0x1000);
    PB_ODR_ODR7 = 0;
    SoftDelay(0x1000);
    OLED_Init();
    OLED_Init();
    OLED_Display_On();
    
    
    ADC_Config();	
    //ADC_DMA_Config();

    //GPIO_Key_Config();

    OLED_ShowString(98,2, "022", 16);
    OLED_ShowString(98,0, "01", 17);
    OLED_ShowString(98,4, "3453", 17);
    OLED_ShowString(98,6, "56", 17);
/*        
    OLED_ShowString(98,2, "022", 16);
    OLED_ShowString(98,0, "01", 17);
    OLED_ShowString(98,4, "3453", 17);
    OLED_ShowString(98,6, "56", 17);
*/	
    asm("rim");	
    //ADC1_CR1_START = 1;
    reg_100ms = 400;
    while(1)
    {
        if(reg_3ms >= 1)
        {
         
            reg_3ms = 0;
            
            Get_Key();
            Key1Event();
            RelayTestMode(100);
            ADC_Task();
            ADC_Event();
        }
      
        if(reg_100ms >= 250)
        {
            reg_100ms = 0;

            
            //显示电压
            ADC_ValueChange();
           Float2String(pwoerVoltage, displayBuff+2);
            
            displayBuff[0] = 'u';
            displayBuff[1] = ':';
            displayBuff[7] = 'v';
            displayBuff[8] = 0;
            
            OLED_ShowString(0,0, displayBuff, 32);
            
            
            //显示电流
            if(Bit.powerOn)
            {
                Float2String(powerCurrent, displayBuff+2);
            }
            else    //没开显示0
            {
                displayBuff[2] = '0';displayBuff[3] = '.';
                displayBuff[4] = '0';displayBuff[5] = '0';displayBuff[6] = '0';
            }
            displayBuff[0] = 'I';
            displayBuff[1] = ':';
            displayBuff[7] = 'A';
            displayBuff[8] = 0;        
            OLED_ShowString(0,4, displayBuff, 32);
            
            //i = Long2String(GetADC14_Value(), displayBuff);
    /*        i = Long2String(ADC_Power, displayBuff);
            for(;i<4;i++)
            {
                displayBuff[i] = ' ';
            }
            
            OLED_ShowString(0,4, displayBuff, 32);

            ADC_ValueChange();
            i = Float2String(pwoerVoltage, displayBuff);
            for(;i<4;i++)
            {
                displayBuff[i] = ' ';
            }
            
            OLED_ShowString(0,0, displayBuff, 32);    */        
        }

    }       
}

void SYS_Clock_Config(void)
{
	CLK_CKDIVR = 0x00;
	CLK_ICKCR_HSION = 1;
  	CLK_PCKENR1 |= 0x24;	
  	CLK_PCKENR2 |= 0x11;		
}

void GPIO_Config(void)
{
        SYSCFG_RMPCR1 |= 0x20;
  
	PB_DDR |= 0X80;
	PB_CR1 |= 0X81;
	PB_CR2 |= 0X80;	
	PB_ODR |= 0X80;
	
        PD_ODR = 0X00;
	PD_DDR = 0X01;
	PD_CR1 = 0X01;
	PD_CR2 = 0X01;

        PC_ODR = 0Xf3;
	PC_DDR = 0Xf3;
	PC_CR1 = 0Xf0;
	PC_CR2 = 0Xf3;	

}


void SoftDelay(u32 t)
{
    while(t--);
}





