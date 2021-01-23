#include "KEY.H"

#define KEY1            PB_IDR_IDR0
#define RELAY_OUT       PD_ODR_ODR0




u8 key1Count = 0;
u16 key1LongCount = 0;

void Get_Key(void)
{
    if(Bit.key1Down)	//已经按下
    {
        if(KEY1)
        {
            if(++key1Count >= 60)
            {
                if(!Bit.key1Lock)
                {
                    Bit.key1Click = 1;
                }
                Bit.key1Lock = 0;
                Bit.key1Down = 0;
                Bit.key1Long = 0;
                key1LongCount = 0;
            }
        }
        else
        {
            key1Count = 0;
        }
    }
    else	//未按下
    {
        if(!KEY1)
        {
            if(++key1Count >= 60)
            {
                Bit.key1Down = 1;
            }
        }
        else
        {
            key1Count = 0;
        }	
    }    
    
    if(Bit.key1Down)
    {
        if(++key1LongCount >= 1200)
        {
            if(!Bit.key1Lock)
            {
                Bit.key1Lock = 1;
                Bit.key1Long = 1;
            }
        }
    }
}

/**
void Get_Key(Key_TypeDef * keyInfo)
{
    if(keyInfo -> down)	//已经按下
    {
        if(KEY1)
        {
            keyInfo -> ctn++;
            if(keyInfo -> ctn > 6)
            {
                if(!keyInfo -> llock)
                {
                    keyInfo -> click = 1;
                }
                keyInfo -> lclick = 0;
                keyInfo -> llock = 0;
                keyInfo -> lctn = 0;
                keyInfo -> down = 0;
            }
        }
        else
        {
            keyInfo -> ctn = 0;
        }
    }
    else	//未按下
    {
        if(!KEY1)
        {
            keyInfo -> ctn++;
            if(keyInfo -> ctn > 6)
            {		
                keyInfo -> down = 1;
            }
        }
        else
        {
            keyInfo -> ctn = 0;
        }	
    }
    
    if(keyInfo -> down)
    {
        keyInfo -> lctn++;
        if(keyInfo -> lctn > 120)
        {
            if(!keyInfo -> llock)
            {
                keyInfo -> llock = 1;
                keyInfo -> lclick = 1;
            }
        }
    }
}
*/


void Key1Event(void)
{
    if(Bit.key1Click)
    {
        Bit.key1Click = 0;
        Key1ClickEvent();
    }
    if(Bit.key1Long)
    {
        Bit.key1Long = 0;
        Key1LongEvent();
    }
}


void Key1ClickEvent(void)
{
    if(Bit.relayTest)
        return;
    
    if(Bit.powerOn)
    {
        Bit.powerOn = 0;
        RELAY_OUT = 0;
        OLED_ShowString(98,0, "01", 17);
        OLED_ShowString(98,2, "022", 16);
        
    }
    else 
    {
        Bit.powerOn = 1;
        RELAY_OUT = 1;
        OLED_ShowString(98,0, "01", 16);
        OLED_ShowString(98,2, "022", 17);
    }
}


void Key1LongEvent(void)
{
    if(!Bit.powerOn)
    {
        Bit.relayTest = 1;

    }
}



void RelayTestMode(u16 time)
{
    static u8 init = 0;
    static u8 task = 0;
    static u16 targetTime, nowTime;
    
    if(init != Bit.relayTest)
    {
        if(Bit.relayTest)
        {
            nowTime = 0;
            targetTime = time;
            task = 0;
        }
        init = Bit.relayTest;
    }
    
    if(Bit.relayTest)
    {
        if(task == 0)
        {
            RELAY_OUT = 1;
            task = 1;

            OLED_ShowString(98,0, "01", 17);
            OLED_ShowString(98,2, "022", 17);
            OLED_ShowString(98,4, "3453", 16);
        }
        else if(task == 1)
        {
            if(++nowTime >= targetTime)
            {
                RELAY_OUT = 0;
                Bit.relayTest = 0;

                OLED_ShowString(98,2, "022", 16);
                OLED_ShowString(98,4, "3453", 17);
            }
        }
    }
}











































