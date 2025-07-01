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

#define BAUD 9600
#define BAUDRATE  ((F_CPU/(BAUD*16UL))-1)

#define indicator_1 PORTK    // Output port to display encoder value

// Function prototypes
void port_init(void);
void ADC_initiate(void);
uint16_t ADC_read(uint8_t ch);
void Encoder_Start(void);

// Encoder variables
int curA = 0;       // Current ADC value for encoder
int preA = 0;       // Previous ADC value for encoder
int disp1 = 0;      // Display value (accumulated encoder counts)

int main(void)
{
    port_init();        // Initialize ports
    ADC_initiate();     // Initialize ADC

    preA = ADC_read(1); // Read initial ADC value from channel 1 (rotary encoder)

    while (1) 
    {
        Encoder_Start();         // Process encoder changes
        indicator_1 = disp1;     // Output accumulated value to PORTK (e.g. LEDs)
    }
}

// Initialize ports: PORTK as output, PORTF as input (ADC for encoder)
void port_init(void)
{
    DDRK = 0xFF;    // Set PORTK as output for indicator
    DDRF = 0x00;    // Set PORTF as input for ADC channels
}

// Reads ADC value and updates encoder display count
void Encoder_Start()
{
    curA = ADC_read(1);  // Read ADC channel 1 (encoder position)

    disp1 += curA - preA;   // Increment display value by difference

    // Handle wrap-around / large jumps due to ADC rollover or noise
    if (curA - preA <= -150)
    {
        disp1 -= (curA - preA);
        disp1++;  // Increment by 1 for a rollover case
    }
    if (curA - preA >= 150)
    {
        disp1 -= (curA - preA);
        disp1--;  // Decrement by 1 for rollover in opposite direction
    }

    preA = curA;   // Update previous reading
}

// Initialize ADC for single conversion on selected channel
void ADC_initiate()
{
    // AVcc reference, right-adjusted result
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR);
    // Enable ADC, disable auto trigger & interrupt, prescaler = 128 (approx 115kHz ADC clock)
    ADCSRA = (1 << ADEN) | (0 << ADATE) | (0 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = 0x00; // Free running mode disabled
}

// Read ADC value from given channel (0-15)
uint16_t ADC_read(uint8_t ch)
{
    // Clear previous channel selection bits (MUX4..0)
    ADMUX &= 0xE0;
    ADCSRB &= ~(1 << MUX5);

    ch &= 0x0F;  // Mask to 4 bits

    if (ch <= 7)
    {
        ADMUX |= ch;      // Set MUX bits for channels 0-7
        ADCSRB = 0x00;
    }
    else
    {
        ch -= 8;
        ADMUX |= ch;      // Set MUX bits for channels 8-15 (lower 3 bits)
        ADCSRB = (1 << MUX5);  // Set MUX5 for higher channels
    }

    ADCSRA |= (1 << ADSC);    // Start conversion

    // Wait for conversion to complete (ADIF set)
    while (!(ADCSRA & (1 << ADIF)));

    ADCSRA |= (1 << ADIF);    // Clear ADIF by writing one to it

    return ADC;               // Return 10-bit ADC result
}
