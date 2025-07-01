/*
 * Conversion_16bit_to_8bit.c
 *
 * Created: 16-10-2018 19:39:31
 * Author : Sahil
 * Description: Demonstrates reading digital signal and (optionally) ADC from a laser sensor.
 *              Outputs indicators on PORTA and PORTB based on digital input from laser.
 */

#define F_CPU 14745600UL  // Corrected typo: F_CPU14745600UL → F_CPU 14745600UL

#include <avr/io.h>

#define laser_input PINF     // Laser sensor digital input from PINF
#define indicator_1 PORTA    // Output indicator port 1
#define indicator_2 PORTB    // Output indicator port 2 (not used in this example)

// Global variable for laser ADC value (if used)
uint16_t receive_laser = 0;

// Function Prototypes
void port_init(void);
void ADC_initiate(void);
void digital_read(void);
uint16_t ADC_read(uint8_t ch);

int main(void)
{
    port_init();       // Set up I/O directions
    ADC_initiate();    // Initialize ADC

    while (1)
    {
        // Uncomment to read analog value from ADC channel 1
        // receive_laser = ADC_read(1);

        digital_read(); // Read digital laser input and set indicators
    }
}

// Configure I/O ports
void port_init(void)
{
    DDRA = 0xFF;  // Set PORTA as output (indicator_1)
    DDRB = 0xFF;  // Set PORTB as output (indicator_2)
    DDRF = 0x00;  // Set PORTF as input (laser sensor input)
}

// Read digital input from laser and light up indicators
void digital_read(void)
{
    if (laser_input & (1 << 0))  // Check if PINF0 is high
    {
        indicator_1 = 0xFF;  // All LEDs ON
    }
    else
    {
        indicator_1 = 0xAA;  // Alternate LEDs ON
    }
}

// Initialize ADC: AVCC reference, right-adjusted result, prescaler 128
void ADC_initiate(void)
{
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR);  // AVCC ref, right-adjust
    ADCSRA = (1 << ADEN)  // Enable ADC
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128
    ADCSRB = 0x00;  // Free running mode (no auto trigger)
}

// Read analog value from ADC channel 0–15 (supports extended channels)
uint16_t ADC_read(uint8_t ch)
{
    ch = ch & 0x0F; // Limit channel to 0–15

    // Clear previous MUX settings
    ADMUX &= 0b11100000;
    ADCSRB &= ~(1 << MUX5);

    if (ch <= 7)
    {
        ADMUX |= ch;          // Set lower channels (0–7)
    }
    else
    {
        ch -= 8;
        ADMUX |= ch;          // Set lower bits
        ADCSRB |= (1 << MUX5); // Enable MUX5 for channels 8–15
    }

    ADCSRA |= (1 << ADSC);          // Start conversion

    while (!(ADCSRA & (1 << ADIF))); // Wait until conversion is complete

    ADCSRA |= (1 << ADIF);          // Clear interrupt flag

    return ADC;  // Return 10-bit result
}
