#include "ADC.H"
#include "user.H"

u16 ADC_Buff[8] = {0};
u8 ADC_Buff_Cnt = 0;
u16 ADC_Current, ADC_Base, ADC_Power;
u8 lastCurrent, lastBase, lastPower;
float ADC_CurrentBuff = 0.0, ADC_BaseBuff = 0.0, ADC_PowerBuff = 0.0;
float powerCurrent = 0.0, pwoerVoltage = 0.0;
u8 taskCnt = 0;


extern BIT bit;
void ADC_Config(void)
{
	ADC1_CR1_CONT = 1;//0 单次转换模式
	ADC1_CR2 |= 0x07;
	ADC1_CR3 |= 0xe0;
	ADC1_CR1_ADON = 1;
	
        
 
        ADC1_SQR3_CHSEL_S14 = 1;//
        //ADC1_SQR2_CHSEL_S16 = 1;//       
        //ADC1_SQR2_CHSEL_S17 = 1;//           
}



/*
u16 GetADC14_Value(void)
{
    u16 val;
    
    ADC1_CR1_START = 1;
    while(!ADC1_SR_EOC);
    ADC1_SR_EOC = 0;
    val = ADC1_DRH;
    val  <<= 8;
    val  |= ADC1_DRL;
    return val;
}*/

void ADC_TaskSave(void)
{
    ADC1_SR_EOC = 0;
    ADC_Buff[ADC_Buff_Cnt] = ADC1_DRH;
    ADC_Buff[ADC_Buff_Cnt] <<= 8;
    ADC_Buff[ADC_Buff_Cnt] |= ADC1_DRL; 
    taskCnt--;
}

void ADC_Task(void)
{

    
    switch(taskCnt)
    {
        case 4:
        case 2:
        case 0:
            ADC1_CR1_START = 1;
            taskCnt++;
            break;
            
        case 1:
            if(ADC1_SR_EOC)
            {
                ADC_TaskSave();

                if(++ADC_Buff_Cnt == 8)
                {
                    Bit.ADC_CH14_Ok = 1;
                    taskCnt = 2;
                    ADC_Buff_Cnt = 0;
                    ADC1_SQR3_CHSEL_S14 = 0;
                    ADC1_SQR2_CHSEL_S16 = 1;
                }
            }
            break;

        case 3:
            if(ADC1_SR_EOC)
            {
                ADC_TaskSave();

                if(++ADC_Buff_Cnt == 8)
                {
                    Bit.ADC_CH16_Ok = 1;
                    taskCnt = 4;
                    ADC_Buff_Cnt = 0;
                    ADC1_SQR2_CHSEL_S16 = 0;
                    ADC1_SQR2_CHSEL_S17 = 1;
                }
            }  
            break;

        case 5:
            if(ADC1_SR_EOC)
            {
                ADC_TaskSave();

                if(++ADC_Buff_Cnt == 8)
                {
                    Bit.ADC_CH17_Ok = 1;
                    taskCnt = 0;
                    ADC_Buff_Cnt = 0;
                    ADC1_SQR2_CHSEL_S17 = 0;
                    ADC1_SQR3_CHSEL_S14 = 1;
                }
            }      
            break;
        default:
            break;
    }
}

float adcBuff;
u16 adcValue;
u8 lastError;
void ADC_EventStabilizer(void)
{
    u8 i = 0;
    u16 sum = 0;
    float error, buff;
    
    for(i = 0; i < 8; i++)
    {
        sum += ADC_Buff[i];
    }
    sum /= 8;
    error = ((float)sum - adcValue) / 256; //
    if(error < 0)
        error = -error;
    
    if(error > 1)
        error = 1;

    
    buff = (1.0-error*0.5) * adcBuff + error *  sum / 2;
    adcBuff = buff;
    adcValue = (int)buff;
    
    error = buff - (int)buff;
    if(error > 0.7)
    {
        (adcValue)++;
        lastError = 1;
    }
    else if(error >= 0.3)
    {
        if(lastError)
        {
            (adcValue)++;
        }
    }
    else
    {
        lastError = 0;
    }
      
}



/*void ADC_EventStabilizer(float *adcBuff, u16 *adcValue)
{
    u8 i = 0;
    u16 sum = 0;
    float error, buff;
    
    for(i = 0; i < 8; i++)
    {
        sum += ADC_Buff[i];
    }
    sum /= 8;
    error = ((float)sum - *adcValue) / 512; //
    if(error < 0)
        error = -error;
    
    if(error > 1)
        error = 1;

    
    buff = (1.0-error*0.125) * (*adcBuff) + error *  sum / 8;
    *adcBuff = buff;
    *adcValue = (int)buff;
    if(buff - (int)buff > 0.5)
    {
        (*adcValue)++;
    }
}


*/
void ADC_Event(void)
{   
    if(Bit.ADC_CH14_Ok)
    {
        Bit.ADC_CH14_Ok = 0;
        adcBuff = ADC_CurrentBuff;
        adcValue = ADC_Current;
        lastError = lastCurrent;
        
        ADC_EventStabilizer();
        
        ADC_CurrentBuff = adcBuff;
        ADC_Current = adcValue;
        lastCurrent = lastError;

    }
    else if(Bit.ADC_CH16_Ok)
    {
        Bit.ADC_CH16_Ok = 0;
        adcBuff = ADC_BaseBuff;
        adcValue = ADC_Base;
        lastError = lastBase;
        
        ADC_EventStabilizer();
        
        ADC_BaseBuff = adcBuff;
        ADC_Base = adcValue;
        lastBase = lastError;
    }    
    else if(Bit.ADC_CH17_Ok)
    {
        adcBuff = ADC_PowerBuff;
        adcValue = ADC_Power;
        lastError = lastPower;
        
        Bit.ADC_CH17_Ok = 0;
        ADC_EventStabilizer();
        
        ADC_PowerBuff = adcBuff;
        ADC_Power = adcValue;
        lastPower = lastError;
    }     
}


void ADC_ValueChange(void)
{
    pwoerVoltage = (float)ADC_Power * 1.25 * 51.1 * 1.012/ ADC_Base / 2.61;
    
    powerCurrent = (float)ADC_Current *1.25 * 2 / ADC_Base / 1.038000;
}



float VoltageCompensation(float v)
{
/*    const float buf[] = {0.0, 0.0, 0.0, 0.0, -0.06, -0.05, -0.03, -0.03, 0.0, 0.0, 
                         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.11, 0.15, 0.15, 
                         0.2, 0.25, 0.3, 0.35, 0.4, 0.45};*13.9 /3.9
    return buf[(int)v];*/
}
















