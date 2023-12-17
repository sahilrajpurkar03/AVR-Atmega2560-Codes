/*
 * adcccc.c
 *
 * Created: 27-07-2017 19:08:22
 * Author : Sahil
 */ 
#define F_CPU 14947600UL
#include <avr/io.h>
#include <util/delay.h>

void timer();
void adc_init();
uint16_t ReadADC(uint8_t ch);
long map_value(long in_value, long in_min, long in_max, long out_min, long out_max);

int main(void)
{
	uint16_t adc_result;
	DDRB=0xff;//output kiya
	//PORTB=(1<<PINB7);
    adc_init();//initilised adc
	timer();
	
	while (1)
	{
		adc_result=ReadADC(0);
		PORTB=adc_result;
		//long s=map_value(adc_result,0,1023,0,255);
		//OCR1A=s;
	}
}

void adc_init()//adc chalu kiya
{
	ADMUX|=(1<<REFS0);//ref is equal to vcc kiya
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//prescaler 128 kiya
}

uint16_t ReadADC(uint8_t ch)//adc value read kiya
{
	ch = ch &0b00000111;//channel select kiya 0-7
	ADMUX|=ch;
	ADCSRA|=(1<<ADSC);//conversion start kiya
	while (!(ADCSRA &(1<<ADIF)));//wait for conversion
	ADCSRA |=(1<<ADIF);
	return(ADC);
}

long map_value(long in_value, long in_min, long in_max, long out_min, long out_max)
{
	return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void timer()
{
	 TCCR1A=(1<<COM1A1)|(1<<WGM10);//fast PWM
	 TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);//Prescalar 64
}