/*
 * Stepper_a4988.c
 *
 * Created: 08-12-2018 17:07:30
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

// Define stepper motor control port and pins
#define stepper_port PORTA
#define step_pin PA0
#define dir_pin PA1
#define ms1 PA2
#define ms2 PA3
#define ms3 PA4

// Function prototypes
void port_init(void);
void stepper_clockwise(int angle);
void stepper_anticlockwise(int angle);
long map_value(long in_value, long in_min, long in_max, long out_min, long out_max);

int main(void)
{
    port_init();

    while (1) 
    {
        stepper_clockwise(90);     // Rotate clockwise by 90 degrees
        _delay_ms(1000);
        stepper_anticlockwise(90); // Rotate anticlockwise by 90 degrees
        _delay_ms(1000);
    }
}

// Initialize PORTA as output for stepper control
void port_init(void)
{
    DDRA = 0xFF;   // Set PORTA pins as output
}

// Rotate stepper motor clockwise by specified angle
void stepper_clockwise(int angle)
{
    // Clear microstep pins (ms1, ms2, ms3) to select full-step mode
    stepper_port &= ~((1 << ms1) | (1 << ms2) | (1 << ms3));
    // Set direction pin HIGH for clockwise rotation
    stepper_port |= (1 << dir_pin);

    // Map angle (0-360 degrees) to step count (0-200 steps)
    int steps = map_value(angle, 0, 360, 0, 200);

    for (int i = 0; i < steps; i++)
    {
        // Generate a step pulse
        stepper_port |= (1 << step_pin);   // Step pin HIGH
        _delay_us(500);
        stepper_port &= ~(1 << step_pin);  // Step pin LOW
        _delay_us(500);
    }
}

// Rotate stepper motor anticlockwise by specified angle
void stepper_anticlockwise(int angle)
{
    // Clear microstep pins (ms1, ms2, ms3) to select full-step mode
    stepper_port &= ~((1 << ms1) | (1 << ms2) | (1 << ms3));
    // Set direction pin LOW for anticlockwise rotation
    stepper_port &= ~(1 << dir_pin);

    // Map angle (0-360 degrees) to step count (0-200 steps)
    int steps = map_value(angle, 0, 360, 0, 200);

    for (int i = 0; i < steps; i++)
    {
        // Generate a step pulse
        stepper_port |= (1 << step_pin);   // Step pin HIGH
        _delay_us(500);
        stepper_port &= ~(1 << step_pin);  // Step pin LOW
        _delay_us(500);
    }
}

// Map a value from one range to another
long map_value(long in_value, long in_min, long in_max, long out_min, long out_max)
{
    return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
