/*
 * Drive_Encoder_Interrupt.c
 *
 * Created: 30-07-2018 19:23:29
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define indicator_1 PORTK

void port_init();
void drive_encoder();
void eint_init();
void eint_en(int eint_pin);
void eint_dis(int eint_pin);

//variable for drive_encoder
volatile int pulse_A=0,pulse_B=0,disp=0,i=0;

int main(void)
{
	sei();
	port_init();
	eint_init();

    while (1) 
    {
		drive_encoder();			
		indicator_1=disp;
    }
	
}

void port_init()
{
	DDRK=0xFF;			//indicator_1
	DDRD=0x00;			//Encoder
	PORTD=0xFF;			
}

void eint_init()
{
	EICRA|=(1<<ISC10)|(1<<ISC00);
}

void eint_en(int eint_pin)
{
	EIMSK|=(1<<eint_pin);
}

void eint_dis(int eint_pin)
{
	EIMSK&=(~(1<<eint_pin));
}

ISR(INT1_vect)
{
	if (PIND & 0x02)
	{
		pulse_A=1;
	}
	else
	{
		pulse_A=0;	
	}
}

ISR(INT0_vect)
{
	if (PIND & 0x01)
	{
		pulse_B=1;
	}
	else
	{
		pulse_B=0;
	}
}


void drive_encoder()
{
	eint_en(0);
	eint_en(1);
	
	if (i==0)
	{
		if (pulse_A==0|| pulse_B==0)
		{
			i=1;
			if (pulse_A==0)
			{
				disp++;
			}
			if (pulse_B==0)
			{
					disp--;
			}
		}
	} 
	if (pulse_A==1 && pulse_B==1)
	{
		i=0;
	}
}

