/*
 * Test Motor.c
 *
 * Created: 18-06-2018 12:44:06
 * Author : Robocon
 */ 

#define F_CPU 14945600UL
#include <avr/io.h>
#include <util/delay.h>

#define motor_dir PORTA
#define motor_pwm1 OCR4A
#define motor_pwm2 OCR4B

void motor1();
void motor2();
void breaking();
void port_init();
void pwm_init();
long limit_var(long in_var, long l_limit, long h_limit);

int main(void)
{
	port_init();
	pwm_init();
    
	while (1) 
    {
		motor1(250);
		breaking();
    }
}

void port_init()
{
	DDRA=0xFF;				//direction 1
	DDRH=0xFF;				//timer port
	DDRB=0xFF;				//breaking signal
}

void pwm_init()
{
	TCCR4A |= (1<<COM4A1)|(1<<COM4B1)|(1<<COM4C1)|(1<<WGM40);
	TCCR4B |= (1<<CS42)|(1<<CS40)|(1<<WGM42);
}

void breaking()
{
	PORTB=0x01;
	_delay_ms(1000);
	PORTB=0x00;
	motor1(0);
	_delay_ms(1000);
}

void motor1(int sp_vect)
{
	sp_vect=limit_var(sp_vect,-255,255);
	if (sp_vect<0)
	{
		motor_dir |= (1<<PA2);
		motor_dir &= (~(1<<PA3));
		sp_vect=(-sp_vect);
	}
	else if (sp_vect>0)
	{
		motor_dir &= (~(1<<PA2));
		motor_dir |= (1<<PA3);
	}
	else
	{
		sp_vect=0;
	}
	motor_pwm1=sp_vect;
}

void motor2(int sp_vect)
{
	sp_vect=limit_var(sp_vect,-255,255);
	if (sp_vect<0)
	{
		motor_dir |= (1<<PA2);
		motor_dir &= (~(1<<PA3));
		sp_vect=(-sp_vect);
	}
	else if (sp_vect>0)
	{
		motor_dir &= (~(1<<PA2));
		motor_dir |= (1<<PA3);
	}
	else
	{
		sp_vect=0;
	}
	motor_pwm2=sp_vect;
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
