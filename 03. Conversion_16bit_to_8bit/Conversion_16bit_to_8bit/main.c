/*
 * Conversion_16bit_to_8bit.c
 *
 * Created: 16-10-2018 19:39:31
 * Author : Sahil
 */ 

#define F_CPU14745600UL
#include <avr/io.h>

#define laser_input PINF
#define indicator_1 PORTA
#define indicator_2 PORTB

void port_init();
void ADC_initiate();
void digital_read();
uint16_t ADC_read(uint8_t ch);
uint16_t receive_laser=0;

int main(void)
{
	port_init();
	ADC_initiate();

	while (1) 
    {
//		receive_laser=ADC_read(1);
		digital_read();
	}
}

void port_init()
{
	DDRA=0xFF;			//indicator_1
	DDRB=0xFF;			//indicator_2
	DDRF=0x00;			//input_laser
}
void digital_read()
{
	if (laser_input & 0x01)
	{
		indicator_1=0xFF;
	}
	else
	{
		indicator_1=0xAA;
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
	ADMUX = ADMUX & 0b11100000;    //Clearing all the mux
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
