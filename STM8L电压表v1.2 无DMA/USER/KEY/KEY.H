#ifndef __KEY_H
#define __KEY_H

#include "user.H"



typedef struct Key_Struct
{
  unsigned char down            : 1;
  unsigned char click           : 1;
  unsigned char ctn	        : 6;
  unsigned int  lclick          : 1;
  unsigned int  llock           : 1;
  unsigned int  lctn            : 14; 
} Key_TypeDef;


void Get_Key(void);
void Key1Event(void);
void Key1ClickEvent(void);
void Key1LongEvent(void);
void RelayTestMode(u16 time);




#endif /* __STM8L15x_CLK_H */




