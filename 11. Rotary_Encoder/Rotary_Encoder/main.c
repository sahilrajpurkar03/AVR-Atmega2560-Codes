/*
 * Rotary_Encoder.c
 *
 * Created: 17-08-2018 16:48:17
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define BAUD 9600									//serial com;
#define BAUDRATE  ((F_CPU/(BAUD*16UL)-1))

#define indicator_1 PORTK

void port_init();
void ADC_initiate();
uint16_t ADC_read(uint8_t ch);
void Endcoder_Start();

//Variable for Encoder
int curA=0,preA=0,disp1=0;

int main(void)
{
	port_init();
	ADC_initiate();
	preA=ADC_read(1);
    
	while (1) 
    {
		Endcoder_Start();
		indicator_1=disp1;
    }
}

void port_init()
{
	DDRK=0xFF;				//indicator_1
	DDRF=0x00;				//Rotary_Encoder
}

void Endcoder_Start()
{
	curA=ADC_read(1);

	disp1+= curA - preA;
	if(curA - preA <=-150)
	{
		disp1 -= (curA-preA);
		disp1++;
	}
	if(curA - preA >=150)
	{
		
		disp1 -= (curA-preA);
		disp1--;
	}
	preA = curA;
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
