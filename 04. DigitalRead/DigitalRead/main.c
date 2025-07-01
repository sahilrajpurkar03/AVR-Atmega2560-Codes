/*
 * DigitalRead.c
 *
 * Created: 13-01-2018 14:08:38
 * Author : Sahil
 * Description: Detects rising and falling digital inputs on PINB and PIND,
 *              and reflects them on PORTA and PORTC respectively.
 */

#include <avr/io.h>

// Define output indicators
#define indicator_1 PORTA
#define indicator_2 PORTC

// Function Prototypes
void port_init(void);
void digitalread_rising(void);
void digitalread_falling(void);  // Fixed typo: real → falling

int main(void)
{
    port_init(); // Initialize I/O directions

    while (1) 
    {
        digitalread_rising();   // Check for rising inputs on PINB
        digitalread_falling();  // Check for falling inputs on PIND
    }
}

// Set I/O port directions
void port_init(void)
{
    DDRB = 0x00;  // Set PORTB as input
    DDRD = 0x00;  // Set PORTD as input
    DDRA = 0xFF;  // Set PORTA as output (indicator_1)
    DDRC = 0xFF;  // Set PORTC as output (indicator_2)
}

// Reads rising edge logic levels on PORTB (active HIGH) and sets PORTA
void digitalread_rising(void)
{
    if (PINB & (1 << 0))        // Check if PB0 is HIGH
    {
        indicator_1 = 0x01;
    }
    else if (PINB & (1 << 1))   // Check if PB1 is HIGH
    {
        indicator_1 = 0x02;
    }
    else if (PINB & (1 << 2))   // Check if PB2 is HIGH
    {
        indicator_1 = 0x04;
    }
    else
    {
        indicator_1 = 0x00;     // No signal
    }
}

// Reads falling edge logic levels on PORTD (active LOW) and sets PORTC
void digitalread_falling(void)
{
    if (!(PIND & (1 << 0)))     // Check if PD0 is LOW
    {
        indicator_2 = 0x01;
    }
    else if (!(PIND & (1 << 1)))// Check if PD1 is LOW
    {
        indicator_2 = 0x02;
    }
    else if (!(PIND & (1 << 2)))// Check if PD2 is LOW
    {
        indicator_2 = 0x04;
    }
    else
    {
        indicator_2 = 0x00;     // No signal
    }
}
