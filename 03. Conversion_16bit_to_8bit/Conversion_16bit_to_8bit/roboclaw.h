/*
 * RoboClaw.c
 *
 * Created: 8/25/2018 5:08:21 PM
 * Author : Aakash Chothani
 */ 

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#define rc_baudrate (((F_CPU / (rc_baud * 16UL))) - 1)

#define SetWORDval(arg) (uint8_t)(((uint16_t)arg)>>8),(uint8_t)arg
#define SetDWORDval(arg) (uint8_t)(((uint32_t)arg)>>24),(uint8_t)(((uint32_t)arg)>>16),(uint8_t)(((uint32_t)arg)>>8),(uint8_t)arg

#define MAXRETRY 2

uint16_t crc = 0;
uint32_t timeout;
volatile uint16_t recvd_data = 0;	

uint8_t *status1 = 0, *status2 = 0,bufferM1 = 0,bufferM2 = 0;
bool *valid1 = NULL, *valid2 = NULL;

void driveM1(uint8_t address, int sp_vect);
void driveM2(uint8_t address, int sp_vect);
void DutyM1(uint8_t address, uint16_t duty);
void DutyM2(uint8_t address, uint16_t duty);
void DutyM1M2(uint8_t address, uint16_t duty1, uint16_t duty2);

void resetEnc(uint8_t address);
int32_t readEnc1(uint8_t address);
int32_t readEnc2(uint8_t address);
uint32_t readEnc1Raw(uint8_t address);
uint32_t readEnc2Raw(uint8_t address);

uint8_t readBufferM1(uint8_t address);
uint8_t readBufferM2(uint8_t address);
void setM1PositionPID(uint8_t address,float kp_fp,float ki_fp,float kd_fp,uint32_t kiMax,uint32_t deadzone,uint32_t min,uint32_t max);
void setM2PositionPID(uint8_t address,float kp_fp,float ki_fp,float kd_fp,uint32_t kiMax,uint32_t deadzone,uint32_t min,uint32_t max);

void setAllM1(uint8_t address,uint32_t accel,uint32_t speed,uint32_t deccel,uint32_t position,uint8_t flag);
void setAllM2(uint8_t address,uint32_t accel,uint32_t speed,uint32_t deccel,uint32_t position,uint8_t flag);
void setAll(uint8_t address,uint32_t accel1,uint32_t speed1,uint32_t deccel1,uint32_t position1,uint32_t accel2,uint32_t speed2,uint32_t deccel2,uint32_t position2,uint8_t flag);

bool write_n(uint8_t cnt, ... );
uint16_t Read2(uint8_t address,uint8_t cmd,bool *valid);
uint32_t Read4_1(uint8_t address, uint8_t cmd, uint8_t *status, bool *valid);

void clearCRC();
void updateCRC (uint8_t data);
uint16_t getCRC();

void rc_serialstart_3();
void transmit(uint8_t data);
void usart_flush();
long limit_var(long in_var, long l_limit, long h_limit);

void rc_serialstart_3()
{
	UBRR3H = rc_baudrate>>8;
	UBRR3L = rc_baudrate;
	UCSR3B = 0b00011000;//enable RXEN TXEN
	UCSR3C = 0b00000110;// UCSZ1 UCSZ0
}

void transmit(unsigned char data)
{
	while(!(UCSR3A & (1<<UDRE3)));
	UDR3 = data;
}

void usart_flush()
{
	unsigned char dummy;
	while (UCSR3A & (1<<RXC3))
	 dummy = UDR3;
}

void driveM1(uint8_t address, int sp_vect)
{
	sp_vect = limit_var(sp_vect,-127,127);
	if(sp_vect == 0 || sp_vect > 0)
	{
		write_n(3,address,0,sp_vect);
	}
	else if(sp_vect < 0)
	{
		write_n(3,address,1,(-sp_vect));
	}
}

void driveM2(uint8_t address, int sp_vect)
{
	sp_vect = limit_var(sp_vect,-127,127);
	if(sp_vect == 0 || sp_vect > 0)
	{
		write_n(3,address,4,sp_vect);
	}
	else if(sp_vect < 0)
	{
		write_n(3,address,5,(-sp_vect));
	}
}

void DutyM1(uint8_t address, uint16_t duty)
{
	write_n(4,address,32,SetWORDval(duty));
}

void DutyM2(uint8_t address, uint16_t duty)
{
	write_n(4,address,33,SetWORDval(duty));
}

void DutyM1M2(uint8_t address, uint16_t duty1, uint16_t duty2)
{
	write_n(6,address,34,SetWORDval(duty1),SetWORDval(duty2));
}

void resetEnc(uint8_t address)
{
	write_n(2,address,20);
}

int32_t readEnc1(uint8_t address)
{
	int32_t enc1_val = readEnc1Raw(address);
	if(valid1)
	{
		if(!(status1 && 0x02))
		{
			return enc1_val;
		}
		else
		{
			return (-enc1_val);
		}
	}	
}

int32_t readEnc2(uint8_t address)
{
	int32_t enc2_val = readEnc2Raw(address);
	if(valid2)
	{
		if(!(status2 && 0x02))
		{
			return enc2_val;
		}
		else
		{
			return (-enc2_val);
		}
	}
}

uint32_t readEnc1Raw(uint8_t address)
{
	return Read4_1(address,16,&status1,&valid1);
}

uint32_t readEnc2Raw(uint8_t address)
{
	return Read4_1(address,17,&status2,&valid2);
}

uint8_t readBufferM1(uint8_t address)
{
	bool *valid = NULL;
	uint16_t value = Read2(address,47,&valid);

	if(valid)
	{
		bufferM1 = value>>8;
		return bufferM1;
	}
}

uint8_t readBufferM2(uint8_t address)
{
	bool *valid = NULL;
	uint16_t value = Read2(address,47,&valid);

	if(valid)
	{
		bufferM2 = value;
		return bufferM2;
	}
}

bool write_n(uint8_t cnt, ... )
{
	uint8_t trys=MAXRETRY;
	do{
		clearCRC();
		va_list marker;
		va_start(marker,cnt);     /* Initialize variable arguments. */
		for(uint8_t index=0;index<cnt;index++)
		{
			uint8_t data = va_arg(marker, int);
			updateCRC(data);
			transmit(data);
		}
		va_end(marker);              /* Reset variable arguments.      */
		uint16_t crc = getCRC();
		transmit(crc>>8);
		transmit(crc);
		
	}while(trys--);
	return false;
}

bool read_n(uint8_t cnt,uint8_t address,uint8_t cmd,...)
{
	uint32_t value=0;
	uint8_t trys=MAXRETRY;
	int16_t data;
	do{
		usart_flush();
		
		data=0;
		clearCRC();
		transmit(address);
		updateCRC(address);
		transmit(cmd);
		updateCRC(cmd);

		va_list marker;
		va_start( marker, cmd );     /* Initialize variable arguments. */
		for(uint8_t index=0;index<cnt;index++)
		{
			uint32_t *ptr = va_arg(marker, uint32_t *);

			if(data!=-1)
			{
				
				data = read_time(timeout);
				updateCRC(data);
				value=(uint32_t)data<<24;
			}
			else{
				break;
			}
			
			if(data!=-1)
			{
				data = read_time(timeout);
				updateCRC(data);
				value|=(uint32_t)data<<16;
			}
			else
			{
				break;
			}

			if(data!=-1)
			{
				data = read_time(timeout);
				updateCRC(data);
				value|=(uint32_t)data<<8;
			}
			else
			{
				break;
			}

			if(data!=-1)
			{
				data = read_time(timeout);
				updateCRC(data);
				value|=(uint32_t)data;
			}
			else
			{
				break;
			}

			*ptr = value;
		}
		va_end( marker );              /* Reset variable arguments.      */

		if(data!=-1)
		{
			uint16_t ccrc;
			data = read_time(timeout);
			if(data!=-1)
			{
				ccrc = data << 8;
				data = read_time(timeout);
				if(data!=-1)
				{
					ccrc |= data;
					return getCRC()==ccrc;
				}
			}
		}
	}while(trys--);

	return false;
}

uint16_t Read2(uint8_t address,uint8_t cmd,bool *valid)
{
	if(valid)
	*valid = false;
	
	uint16_t value=0;
	uint8_t trys=MAXRETRY;
	int16_t data;
	
	do{
		usart_flush();

		clearCRC();
		transmit(address);
		updateCRC(address);
		transmit(cmd);
		updateCRC(cmd);
		
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);
		value = (uint16_t)data<<8;
		
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);
		value |= (uint16_t)data;
		
		uint16_t ccrc;
			
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		
		ccrc = data << 8;
				
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
	
		ccrc |= data;
		
		if(getCRC()==ccrc)
		{
			*valid = true;
			return value;
		}
	}while(trys--);
	
	return false;
}

uint32_t Read4_1(uint8_t address, uint8_t cmd, uint8_t *status, bool *valid)
{
	if(valid)
	*valid = false;
	
	uint32_t value = 0;
	uint8_t trys = MAXRETRY;
	int16_t data;
	
	do
	{
		usart_flush();

		clearCRC();
		transmit(address);
		updateCRC(address);
		transmit(cmd);
		updateCRC(cmd);
		
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);
		value=(uint32_t)data<<24;
		
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);		 
		value|=(uint32_t)data<<16;

		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);		 		
		value|=(uint32_t)data<<8;	
		
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);
		value|=(uint32_t)data;	
		
		while(!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		updateCRC(data);
		if(status)
		*status = data;
		
		uint16_t ccrc;		
		while (!((UCSR3A) & (1<<RXC3)));
		data = UDR3;
								
		ccrc = data << 8;
		while ( !((UCSR3A) & (1<<RXC3)));
		data = UDR3;
		
		ccrc |= data;
					
		if(getCRC() == ccrc)
		{
			*valid = true;
			return value;
		}
	}while(trys--);
	return false;
}	

void clearCRC()
{
	crc = 0;
}

void updateCRC (uint8_t data)
{
	int i;
	crc = crc ^ ((uint16_t)data << 8);
	for (i=0; i<8; i++)
	{
		if (crc & 0x8000)
		crc = (crc << 1) ^ 0x1021;
		else
		crc <<= 1;
	}
}

uint16_t getCRC()
{
	return crc;
}

long limit_var(long in_var, long l_limit, long h_limit)
{
	if (in_var>h_limit)
	{
		in_var=h_limit;
	}
	else if (in_var<l_limit)
	{
		in_var=l_limit;
	}
	return in_var;
}