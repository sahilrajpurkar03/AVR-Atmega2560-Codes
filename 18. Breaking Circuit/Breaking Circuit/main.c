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

void motor1(int sp_vect);
void motor2(int sp_vect);
void breaking(void);
void port_init(void);
void pwm_init(void);
long limit_var(long in_var, long l_limit, long h_limit);

int main(void)
{
    port_init();
    pwm_init();

    while (1) 
    {
        motor1(250);   // Run motor1 at speed 250
        breaking();    // Apply brake after running
    }
}

void port_init(void)
{
    DDRA = 0xFF;    // Port A as output for motor direction signals
    DDRH = 0xFF;    // Port H as output for PWM (Timer4 channels)
    DDRB = 0xFF;    // Port B as output for brake signal
}

void pwm_init(void)
{
    // Configure Timer4 for Fast PWM mode, non-inverting mode on channels A, B, and C
    TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << COM4C1) | (1 << WGM40);
    TCCR4B |= (1 << CS42) | (1 << CS40) | (1 << WGM42); // Prescaler = 1024, WGM mode 5 (Fast PWM 8-bit)
}

void breaking(void)
{
    PORTB = 0x01;       // Activate brake signal (set PB0 high)
    _delay_ms(1000);    // Brake for 1 second
    PORTB = 0x00;       // Release brake
    motor1(0);          // Stop motor1
    _delay_ms(1000);    // Wait 1 second before next command
}

void motor1(int sp_vect)
{
    sp_vect = limit_var(sp_vect, -255, 255);

    if (sp_vect < 0)
    {
        motor_dir |= (1 << PA2);    // Set direction pin PA2 high
        motor_dir &= ~(1 << PA3);   // Set direction pin PA3 low
        sp_vect = -sp_vect;          // Use positive PWM duty cycle
    }
    else if (sp_vect > 0)
    {
        motor_dir &= ~(1 << PA2);   // Set direction pin PA2 low
        motor_dir |= (1 << PA3);    // Set direction pin PA3 high
    }
    else
    {
        sp_vect = 0;
    }

    motor_pwm1 = sp_vect;           // Set PWM duty cycle for motor1
}

void motor2(int sp_vect)
{
    sp_vect = limit_var(sp_vect, -255, 255);

    if (sp_vect < 0)
    {
        motor_dir |= (1 << PA2);
        motor_dir &= ~(1 << PA3);
        sp_vect = -sp_vect;
    }
    else if (sp_vect > 0)
    {
        motor_dir &= ~(1 << PA2);
        motor_dir |= (1 << PA3);
    }
    else
    {
        sp_vect = 0;
    }

    motor_pwm2 = sp_vect;  // Set PWM duty cycle for motor2
}

long limit_var(long in_var, long l_limit, long h_limit)
{
    if (in_var > h_limit)
        return h_limit;
    else if (in_var < l_limit)
        return l_limit;
    else
        return in_var;
}
