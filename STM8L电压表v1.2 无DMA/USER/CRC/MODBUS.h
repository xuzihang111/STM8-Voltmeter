#ifndef __MODBUS_H__
#define __MODBUS_H__

#define u16 unsigned int
#define u8 unsigned char 


typedef struct _TyteCRC{
	unsigned char High;
	unsigned char Low;
}TYPE_CRC;

TYPE_CRC modbusCRC16(u8 *updata,u16 len);

#endif






























