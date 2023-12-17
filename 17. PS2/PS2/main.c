/*
 * PS2.c
 *
 * Created: 19-06-2018 17:56:38
 * Author : Sahil
 */ 
#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <math.h>
#include <avr/interrupt.h>
#define BAUD 9600									//serial com;
#define BAUDRATE  ((F_CPU/(BAUD*16UL)-1))

uint8_t RX[16]={100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
int RX_range=200,RX_raw=255,RX_ad=255,RX_count=0;
uint8_t TX[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
int flag[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t TX_raw=200,TX_ad=201,TX_flag=0;

#define  indicator_1 PORTB

//variable for ps2
#define PS_L1 0
#define PS_R1 1
#define PS_L2 2
#define PS_R2 3
#define PS_L3 4
#define PS_R3 5
#define PS_TRIANGLE 6
#define PS_SQUARE 7
#define PS_CROSS 8
#define PS_CIRCLE 9
#define PS_UP 10
#define PS_LEFT 11
#define PS_DOWN 12
#define PS_RIGHT 13
#define PS_START 14
#define PS_SELECT 15

int xj1=0,yj1=0,xj2=0,yj2=0,pot1=0,pot2=0,pot3=0,pot4=0;							//analog values(serially received from remote);
int butt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};												//digital values(serially received from remote);
int js_error=20;
int pwm_range=255;

void receive();
void serialstart_3();
uint16_t ADC_read(uint8_t ch);
void ADC_initiate();
void port_init();

long map_value(long in_value, long in_min, long in_max, long out_min, long out_max);
long limit_var(long in_var, long l_limit, long h_limit);


int main(void)
{
	port_init();
    sei();
	ADC_initiate();
	serialstart_3();
	
	while (1) 
    {
		receive();
	}
}

void port_init()
{
	DDRB=0xFF;					//indicator_1
}

void receive()
{
	yj1=map_value(RX[0],0,RX_range,(-pwm_range),pwm_range);
	xj1=map_value(RX[1],0,RX_range,pwm_range,(-pwm_range));
	yj2=map_value(RX[2],0,RX_range,(-pwm_range),pwm_range);
	xj2=map_value(RX[3],0,RX_range,pwm_range,(-pwm_range));
	

	if (butt[PS_START]==1)
	{
		indicator_1^=0xFF;
		butt[PS_START]=0;
	}
	if (butt[PS_SELECT]==1)
	{
		indicator_1^=0xFF;
		butt[PS_SELECT]=0;
	}
	if (butt[PS_UP]==1)
	{
		indicator_1^=0xFF;
		butt[PS_UP]=0;
	}
	if (butt[PS_DOWN]==1)
	{
		indicator_1^=0xFF;
		butt[PS_DOWN]=0;
	}
	if (butt[PS_LEFT]==1)
	{
		indicator_1^=0xFF;
		butt[PS_LEFT]=0;
	}
	if (butt[PS_RIGHT]==1)
	{
		indicator_1^=0xFF;
		butt[PS_RIGHT]=0;
	}
	if (butt[PS_SQUARE]==1)
	{
		indicator_1^=0xFF;
		butt[PS_SQUARE]=0;
	}
	if (butt[PS_CIRCLE]==1)
	{
		indicator_1^=0xFF;
		butt[PS_CIRCLE]=0;
	}
	if (butt[PS_TRIANGLE]==1)
	{
		indicator_1^=0xFF;
		butt[PS_TRIANGLE]=0;
	}
	if (butt[PS_CROSS]==1)
	{
		indicator_1^=0xFF;
		butt[PS_CROSS]=0;
	}
	if (butt[PS_L1]==1)
	{
		indicator_1^=0xFF;
		butt[PS_L1]=0;
	}
	if (butt[PS_R1]==1)
	{
		indicator_1^=0xFF;
		butt[PS_R1]=0;
	}
	if (butt[PS_L2]==1)
	{
		indicator_1^=0xFF;
		butt[PS_L2]=0;
	}
	if (butt[PS_R2]==1)
	{
		indicator_1^=0xFF;
		butt[PS_R2]=0;
	}
	if (butt[PS_L3]==1)
	{
		indicator_1^=0xFF;
		butt[PS_L3]=0;
	}
	if (butt[PS_R3]==1)
	{
		indicator_1^=0xFF;
		butt[PS_R3]=0;
	}
}

void ADC_initiate()
{
	ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR);  // AVcc //  right adjusted
	ADCSRA = (1<<ADEN)|(0<<ADATE)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // bit4 ADC EOC flag // prescalar- 111 - 128 division factor
	ADCSRB = 0x00;
}

uint16_t ADC_read(uint8_t ch)
{
	ADMUX = ADMUX & 0b11100000;    //Clearing all the mux;
	ADCSRB = ADCSRB & 0b11110111;  //------"-"-----------
	ch = ch & 0b00001111;
	if ( ch <= 7 )
	{
		ch = ch & 0b00000111; //
		ADMUX = ADMUX | ch;
		ADCSRB=0x00;
	}
	else
	{
		ch = ch-8;
		ch = ch & 0b00000111;
		ADMUX = ADMUX | ch;
		ADCSRB=0x00;
		ADCSRB = ADCSRB | (1<<MUX5);
	}
	
	ADCSRA = ADCSRA | (1<<ADSC);    //Bit 6 to start conversion-ADSC
	
	while( !(ADCSRA & (1<<ADIF)) ); // Wait for conversion to complete
	
	return(ADC);
	
}

void serialstart_3()
{
	UBRR3H=BAUDRATE>>8;
	UBRR3L=BAUDRATE;
	UCSR3B=0b10011000;//enable RXEN TXEN
	UCSR3C=0b00000110;// UCSZ1 UCSZ0
}

ISR(USART3_RX_vect)
{
	RX_count=1;
	RX_raw=UDR3;
	if ((RX_raw>200) && (RX_raw<255))					//201 to 216 for addresses of analog values, 231 to 246 for buttons;
	{
		RX_ad=RX_raw;
		if ((RX_raw>230) && (RX_raw<247))
		{
			uint8_t r_temp0=(RX_raw-231);
			butt[r_temp0]=1;
		}
	}
	else if ((RX_raw>=0) && (RX_raw<201))
	{
		uint8_t r_temp1=(RX_ad-201);
		if (r_temp1<16)
		{
			RX[r_temp1]=RX_raw;
		}
	}
}


long map_value(long in_value, long in_min, long in_max, long out_min, long out_max)
{
	return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
