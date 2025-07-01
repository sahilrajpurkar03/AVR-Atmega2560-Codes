/*
 * Test_Motor.c
 *
 * Created: 18-06-2018 12:44:06
 * Author : Robocon
 * Description: Controls two motors (direction + speed) using PWM on Timer 4.
 */

#define F_CPU 14945600UL

#include <avr/io.h>
#include <util/delay.h>

// Definitions
#define motor_dir PORTA        // Motor direction control pins
#define motor_pwm1 OCR4A       // PWM output for motor 1
#define motor_pwm2 OCR4B       // PWM output for motor 2

// Function declarations
void port_init(void);
void pwm_init(void);
void motor1(int sp_vect);
void motor2(int sp_vect);
long limit_var(long in_var, long l_limit, long h_limit);

int main(void)
{
	port_init();    // Set I/O directions
	pwm_init();     // Initialize Timer4 for PWM

	while (1)
	{
		// Run motors forward at speed 50
		motor1(50);
		motor2(50);
		_delay_ms(5000);

		// Run motors in reverse at speed -50
		motor1(-50);
		motor2(-50);
		_delay_ms(5000);
	}
}

// Configure ports
void port_init(void)
{
	DDRA = 0xFF;    // Set PORTA as output for motor direction
	DDRH = 0xFF;    // Set PORTH (Timer4 PWM outputs) as output
}

// Initialize Timer4 for fast PWM, non-inverted mode, prescaler 1024
void pwm_init(void)
{
	TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << COM4C1) | (1 << WGM40); // Fast PWM, non-inverted
	TCCR4B |= (1 << CS42) | (1 << CS40) | (1 << WGM42);                     // Prescaler = 1024
}

// Control motor 1: direction + PWM
void motor1(int sp_vect)
{
	sp_vect = limit_var(sp_vect, -255, 255);  // Limit input

	if (sp_vect < 0)
	{
		motor_dir |= (1 << PA0);   // Set direction bit
		sp_vect = -sp_vect;        // Make speed positive
	}
	else if (sp_vect > 0)
	{
		motor_dir &= ~(1 << PA0);  // Clear direction bit
	}
	else
	{
		sp_vect = 0;
	}

	motor_pwm1 = sp_vect;   // Set PWM duty cycle
}

// Control motor 2: direction + PWM
void motor2(int sp_vect)
{
	sp_vect = limit_var(sp_vect, -255, 255);

	if (sp_vect < 0)
	{
		motor_dir |= (1 << PA1);   // Reverse direction
		sp_vect = -sp_vect;
	}
	else if (sp_vect > 0)
	{
		motor_dir &= ~(1 << PA1);  // Forward direction
	}
	else
	{
		sp_vect = 0;
	}

	motor_pwm2 = sp_vect;
}

// Constrain input to given limits
long limit_var(long in_var, long l_limit, long h_limit)
{
	if (in_var > h_limit)
	{
		in_var = h_limit;
	}
	else if (in_var < l_limit)
	{
		in_var = l_limit;
	}
	return in_var;
}
