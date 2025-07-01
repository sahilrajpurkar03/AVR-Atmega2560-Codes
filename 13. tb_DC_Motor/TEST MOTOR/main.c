/*
 * Test Motor.c
 *
 * Created: 18-06-2018 12:44:06
 * Author : Robocon
 */

#define F_CPU 14945600UL
#include <avr/io.h>
#include <util/delay.h>

// Motor control port and PWM registers
#define motor_dir PORTA
#define motor_pwm1 OCR4A
#define motor_pwm2 OCR4B

// Function prototypes
void motor1(int sp_vect);
void motor2(int sp_vect);
void port_init(void);
void pwm_init(void);
long limit_var(long in_var, long l_limit, long h_limit);

int main(void)
{
    port_init();
    pwm_init();

    while (1) 
    {
        motor1(50);  // Motor1 forward at speed 50
        motor2(50);  // Motor2 forward at speed 50
        _delay_ms(5000);

        motor1(-50); // Motor1 reverse at speed 50
        motor2(-50); // Motor2 reverse at speed 50
        _delay_ms(5000);
    }
}

// Initialize motor control direction pins and timer port pins as outputs
void port_init(void)
{
    DDRA = 0xFF;  // PORTA as output (direction control pins)
    DDRH = 0xFF;  // PORTH as output (timer PWM pins)
}

// Initialize Timer4 for PWM on OCR4A and OCR4B channels
void pwm_init(void)
{
    // Configure Timer4:
    // COM4A1 and COM4B1 set for non-inverting PWM on channels A and B
    // WGM40 and WGM42 bits set for Fast PWM mode, 8-bit resolution
    // Prescaler set to clk/1024 (CS42 and CS40)
    TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << WGM40);
    TCCR4B |= (1 << CS42) | (1 << CS40) | (1 << WGM42);
}

// Control motor1 speed and direction
void motor1(int sp_vect)
{
    sp_vect = limit_var(sp_vect, -255, 255);

    if (sp_vect < 0)
    {
        motor_dir |= (1 << PA0);   // Set direction pin PA0 HIGH
        motor_dir &= ~(1 << PA1);  // Set direction pin PA1 LOW
        sp_vect = -sp_vect;        // Make speed positive for PWM
    }
    else if (sp_vect > 0)
    {
        motor_dir &= ~(1 << PA0);  // Set direction pin PA0 LOW
        motor_dir |= (1 << PA1);   // Set direction pin PA1 HIGH
    }
    else
    {
        sp_vect = 0;               // Stop motor
    }

    motor_pwm1 = sp_vect;           // Set PWM duty cycle
}

// Control motor2 speed and direction
void motor2(int sp_vect)
{
    sp_vect = limit_var(sp_vect, -255, 255);

    if (sp_vect < 0)
    {
        motor_dir |= (1 << PA2);   // Set direction pin PA2 HIGH
        motor_dir &= ~(1 << PA3);  // Set direction pin PA3 LOW
        sp_vect = -sp_vect;        // Make speed positive for PWM
    }
    else if (sp_vect > 0)
    {
        motor_dir &= ~(1 << PA2);  // Set direction pin PA2 LOW
        motor_dir |= (1 << PA3);   // Set direction pin PA3 HIGH
    }
    else
    {
        sp_vect = 0;               // Stop motor
    }

    motor_pwm2 = sp_vect;           // Set PWM duty cycle
}

// Limits input variable between lower and upper bounds
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
