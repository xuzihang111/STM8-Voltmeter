#ifndef __FLASH_H
#define __FLASH_H

#define u8 unsigned char
#define u16 unsigned int

#define KEY1_ADDRESS 	0x1000
#define KEY2_ADDRESS 	0x1001
#define TEMPERATURE_CALIBRATION_ADDRESS 0x1010
#define HUMIDITY_CALIBRATION_ADDRESS 	0x1014
#define VOLTAGE_CALIBRATION_ADDRESS 	0x1018
#define MESSAGE_ADDRESS 				0x1040


u8 EEPROM_lock(void);
u8 EEPROM_Unlock(void);
float GetEEPROM_Value(u16 addr);
u8 SetEEPROM_Value(u16 Eaddr, float value);
u8 EEPROM_FirstWrite(void);
void EEPROM_WriteFlag(void);


#endif






