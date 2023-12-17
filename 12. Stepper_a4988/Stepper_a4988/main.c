/*
 * Stepper_a4988.c
 *
 * Created: 08-12-2018 17:07:30
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

#define stepper_port PORTA
#define step_pin PINA0
#define dir_pin PINA1
#define ms1 PINA2
#define ms2 PINA3
#define ms3 PINA4

void port_init();
void stepper_clockwise(int angle);
void stepper_anticlockwisw(int angle);
long map_value(long in_value, long in_min, long in_max, long out_min, long out_max);


int main(void)
{
	port_init();
    while (1) 
    {
		stepper_clockwise(90);
		_delay_ms(1000);
		stepper_anticlockwisw(90);
		_delay_ms(1000);
    }
}

void port_init()
{
	DDRA=0xFF;				//stepper	
}

void stepper_clockwise(int angle)
{
	stepper_port&=~(1<<ms1)&(1<<ms2)&(1<<ms3);
	stepper_port|=(1<<dir_pin);
	int actual_angle=map_value(angle,0,360,0,200);
	for (int i=0;i<actual_angle;i++)
	{
		stepper_port|=(1<<step_pin);
		_delay_us(500);
		stepper_port&=~(1<<step_pin);
		_delay_us(500);
	}	
}

void stepper_anticlockwisw(int angle)
{
	stepper_port&=~(1<<ms1)&(1<<ms2)&(1<<ms3);
	stepper_port&=~(1<<dir_pin);
	int actual_angle=map_value(angle,0,360,0,200);
	for (int i=0;i<actual_angle;i++)
	{
		stepper_port|=(1<<step_pin);
		_delay_us(500);
		stepper_port&=~(1<<step_pin);
		_delay_us(500);
	}	
}

long map_value(long in_value, long in_min, long in_max, long out_min, long out_max)
{
	return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
