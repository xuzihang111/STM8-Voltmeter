#include "SI4432.H"
#include "SPI.H"

void SI4432_Config(void)
{
  	
	
	SI4432_Reset();
  
  
	SPI_WriteReg(0x75, 0x53);
	SPI_WriteReg(0x76, 0x64);
	SPI_WriteReg(0x77, 0x00);
	
	SPI_WriteReg(0x2a, 0x14);	
	SPI_WriteReg(0x6E, 0x09);	//write 0x4E to the TXDataRate 1 register
	SPI_WriteReg(0x6F, 0xd5);	//write 0xA5 to the TXDataRate 0 register
	SPI_WriteReg(0x70, 0x2C);		

	SPI_WriteReg(0x1C, 0x1b);	//write 0x1E to the IF Filter Bandwidth register		
	SPI_WriteReg(0x20, 0x83);	//write 0xD0 to the Clock Recovery Oversampling Ratio register		
	SPI_WriteReg(0x21, 0xc0);	//write 0x00 to the Clock Recovery Offset 2 register		
	SPI_WriteReg(0x22, 0x13);	//write 0x9D to the Clock Recovery Offset 1 register		
	SPI_WriteReg(0x23, 0xa9);	//write 0x49 to the Clock Recovery Offset 0 register		
	SPI_WriteReg(0x24, 0x00);	//write 0x00 to the Clock Recovery Timing Loop Gain 1 register		
	SPI_WriteReg(0x25, 0x03);	//write 0x24 to the Clock Recovery Timing Loop Gain 0 register		
	SPI_WriteReg(0x1D, 0x40);	//write 0x40 to the AFC Loop Gearshift Override register		
	SPI_WriteReg(0x1E, 0x0A);	//write 0x0A to the AFC Timing Control register		
	SPI_WriteReg(0x2A, 0x14);	//write 0x20 to the AFC Limiter register			
	
	//SPI_WriteReg(0x72, 0x38);	//(1.2kbps)	
	//set the preamble length to 5bytes 
	SPI_WriteReg(0x34, 0x0A);															//write 0x0A to the Preamble Length register
	//set preamble detection threshold to 20bits
	SPI_WriteReg(0x35, 0x2A); 														//write 0x2A to the Preamble Detection Control  register

	//Disable header bytes; set variable packet length (the length of the payload is defined by the
	//received packet length field of the packet); set the synch word to two bytes long
	SPI_WriteReg(0x33, 0x02);															//write 0x02 to the Header Control2 register    
	
	//Set the sync word pattern to 0x2DD4
	SPI_WriteReg(0x36, 0x2D);															//write 0x2D to the Sync Word 3 register
	SPI_WriteReg(0x37, 0xD4);															//write 0xD4 to the Sync Word 2 register

	//enable the TX & RX packet handler and CRC-16 (IBM) check
	SPI_WriteReg(0x30, 0x8D);															//write 0x8D to the Data Access Control register
	//Disable the receive header filters
    SPI_WriteReg(0x32, 0x00 );														//write 0x00 to the Header Control1 register            
	//enable FIFO mode and GFSK modulation
	SPI_WriteReg(0x71, 0x63);															//write 0x63 to the Modulation Mode Control 2 register

	/*set the GPIO's according to the RF switch */
   //SPI_WriteReg(0x0C, 0x12);															//write 0x12 to the GPIO1 Configuration(set the TX state)
	//SPI_WriteReg(0x0b, 0x15);															//write 0x15 to the GPIO2 Configuration(set the RX state) 
	SPI_WriteReg(0x0C, 0x15);															//write 0x15 to the GPIO1 Configuration(set the rX state)
	SPI_WriteReg(0x0b, 0x12);															//write 0x12 to the GPIO0Configuration(set the tX state) 
	/*set the non-default Si443x registers*/
	//set  cap. bank
	SPI_WriteReg(0x09, 0xD7);															//write 0xD7 to the Crystal Oscillator Load Capacitance register
	// Set AGC Override1 Register
	SPI_WriteReg(0x69, 0x60);															//write 0x60 to the AGC Override1 register	

	//set tx power 20dbm max
	//SPI_WriteReg(0x6d, 0x1f);	 
	SPI_WriteReg(0x6d, 0x1e);	
	
	/*enable receiver chain*/
	SPI_WriteReg(0x07, 0x05);															//write 0x05 to the Operating Function Control 1 register
	//Enable two interrupts: *0
	// a) one which shows that a valid packet received: 'ipkval'
	// b) second shows if the packet received with incorrect CRC: 'icrcerror' 
	SPI_WriteReg(0x05, 0x03); 														//write 0x03 to the Interrupt Enable 1 register
	SPI_WriteReg(0x06, 0x00); 														//write 0x00 to the Interrupt Enable 2 register
	//read interrupt status registers to release all pending interrupts
	Get_IRQ_State();													//read the Interrupt Status2 register
	SPI_WriteReg(0x07, 0x05);	
	SPI_WriteReg(0x0d, 0xf4);		
}


u16 Get_IRQ_State(void)
{
  	u16 state;
	
	state = SPI_ReadReg(0x03) << 8;
	state |= SPI_ReadReg(0x04);
	
	return state;
}

void SI4432_Send(u8* buf, u8 len)
{
  	u8 i;
	SPI_WriteReg(0x07, 0x01);	
	SPI_WriteReg(0x3E, len);//write 8 to the Transmit Packet Length register	
	for(i = 0; i < len; i++)
	{
		SPI_WriteReg(0x7f, buf[i]);
	}
	SPI_WriteReg(0x05, 0x04);	//write 0x04 to the Interrupt Enable 1 register	
	SPI_WriteReg(0x06, 0x00);	//write 0x03 to the Interrupt Enable 2 register		
	
	SPI_WriteReg(0x07, 0x09);	
	while(IRQ_STATE);
	Get_IRQ_State();
	delay (5000);
	SPI_WriteReg(0x05, 0x03); 	//write 0x03 to the Interrupt Enable 1 register
	SPI_WriteReg(0x06, 0x00); 												//write 0x00 to the Interrupt Enable 2 register
	//read interrupt status registers to release all pending interrupts
	Get_IRQ_State();	//read the Interrupt Status2 register

	/*enable receiver chain again*/
	SPI_WriteReg(0x07, 0x05);	  	  
}


u8 SI4432_Recv(u8 * buf, u8 len)
{
	u8 pack_len, i;
	u16 state;
	
	if(!IRQ_STATE)
	{
		SPI_WriteReg(0x07, 0x01);//配置为预备模式
		state = Get_IRQ_State();
		if( (state & 0x0001) == 0x0001 )
		{
			//reset the RX FIFO
			SPI_WriteReg(0x08, 0x02);		//write 0x02 to the Operating Function Control 2 register
			SPI_WriteReg(0x08, 0x00);		//write 0x00 to the Operating Function Control 2 register
			return 0x00;
		}		
		else if( (state & 0x0002) == 0x0002 )
		{
			//Read the length of the received payload
			pack_len = SPI_ReadReg(0x4B);	//获取数据包长度
			if(pack_len > len)
			{
				//Get the reeived payload from the RX FIFO
				for(i = 0; i < len; i++)
				{
					buf[i] = SPI_ReadReg(0x7F);						//read the FIFO Access register
				}	
				return pack_len | 0x80;				
			}
			else
			{
				//Get the reeived payload from the RX FIFO
				for(i = 0; i < pack_len; i++)
				{
					buf[i] = SPI_ReadReg(0x7F);						//read the FIFO Access register
				}						
				SI4432_ClrBuf();
				return pack_len & 0x7f;
			}
		}
	}
	return 0x00;
}

void SI4432_ClrBuf(void)
{
	SPI_WriteReg(0x08, 0x02);	
	SPI_WriteReg(0x08, 0x00);	
}


void SI4432_Reset(void)
{
	Get_IRQ_State();
	SPI_WriteReg(0x08, 0x03);		
	SPI_WriteReg(0x08, 0x00);		
	SPI_WriteReg(0x07, 0x80);
	while(IRQ_STATE);
	Get_IRQ_State();
	while(IRQ_STATE);
	Get_IRQ_State();	
}









