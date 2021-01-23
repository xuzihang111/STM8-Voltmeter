#include "StringTool.H"


char Float2String(float f, u8 * str)
{ 
    u8 len = 0;
    u8 s[20] = {0};       //最大整数长度20
    long i = 0;
    u8 j = 0, k;
    
    if(f < 0.0)
        *str++ = '-';
    if(f > 1.0)
    {
        i = (long)f;
        while(i > 0)
        {
            len++;
            s[j++] = i % 10 + 0x30;
            i /= 10;
        }
        for(k = 0; k < j; k++)
        {
            *str++ = s[j-1-k];
        }
    }
    else 
    {
        *str++ = 0x30;
    }
    *str++ = '.';
    
    f = f - (float)((long)f);
    for(k = 0; k < 6; k++)
    {
        f *= 10;
        *str++ = (int)(f) + 0x30;
        f = f - (int)f;
    }
    *str = 0;
    
    return len+7;
}


char Long2String(long l, u8 * str)
{
    u8 len = 0;
    u8 s[20] = {0};       //最大整数长度20
    u8 j = 0;
    
    while(l > 0)
    {
        len++;
        s[j++] = l % 10 + 0x30;
        l /= 10;
    }
    for(; j; )
    {
        *str++ = s[--j];
    }
    *str = 0;
    
    return len;
}




































