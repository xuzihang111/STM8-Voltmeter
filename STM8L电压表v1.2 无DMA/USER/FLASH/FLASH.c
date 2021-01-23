#include "FLASH.H"
#include "IOSTM8L051F3.h"



u8 EEPROM_lock(void)//ÉÏËø
{
  	FLASH_PUKR = 0xae;
	FLASH_PUKR = 0x56;
	if(!FLASH_IAPSR_PUL)
		return 1;
	else
	  	return 0;
}

u8 EEPROM_Unlock(void)//½âËø
{
  	FLASH_DUKR = 0xae;
	FLASH_DUKR = 0x56;
	if(!FLASH_IAPSR_PUL)
		return 1;
	else
	  	return 0;  	
}

float GetEEPROM_Value(u16 addr)
{
	return *((float*)(addr));
}
u8 SetEEPROM_Value(u16 Eaddr, float value)
{
  	static float save;
  	u8* addr, * val;
	
	save = value;
	addr = (u8*)(Eaddr);
	val = (u8*)(&save);
	*addr++ = *val++;
	while(!FLASH_IAPSR_EOP); 
	*addr++ = *val++;
	while(!FLASH_IAPSR_EOP); 
	*addr++ = *val++;
	while(!FLASH_IAPSR_EOP); 
	*addr = *val;
	while(!FLASH_IAPSR_EOP); 
	if(value == *((float*)val))
	  	return 1;
	else
	  	return 0;
}

u8 EEPROM_FirstWrite(void)
{
	if(*((u8*)KEY1_ADDRESS) != 0x56)
	  	return 1;
	if(*((u8*)KEY2_ADDRESS) != 0xAB)
	  	return 1;	
	
	return 0;
}

void EEPROM_WriteFlag(void)
{
	*((u8*)KEY1_ADDRESS) = 0x56;
	while(!FLASH_IAPSR_EOP); 
	*((u8*)KEY2_ADDRESS) = 0xAB;
	while(!FLASH_IAPSR_EOP); 
}



















