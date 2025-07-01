/*
 * adcccc.c
 *
 * Created: 27-07-2017 19:08:22
 * Author : Sahil
 * Description: ADC read example using AVR ATmega2560. Reads analog input from channel 0
 * and sends the 8 MSBs to PORTB. Timer1 is configured for Fast PWM (though not used in this code).
 */

#define F_CPU 14947600UL

#include <avr/io.h>
#include <util/delay.h>

// Function Prototypes
void timer_init(void);
void adc_init(void);
uint16_t read_adc(uint8_t ch);
long map_value(long in_value, long in_min, long in_max, long out_min, long out_max);

int main(void)
{
    uint16_t adc_result;

    DDRB = 0xFF; // Set PORTB as output

    adc_init();     // Initialize ADC
    timer_init();   // Initialize Timer1 for Fast PWM

    while (1)
    {
        adc_result = read_adc(0); // Read analog value from channel 0
        PORTB = adc_result >> 2;  // Output 8 MSBs of 10-bit ADC result to PORTB
                                  // (ADC is 10-bit, PORTB is 8-bit)
    }
}

// Initialize ADC: AVCC as reference, ADC enabled, prescaler set to 128
void adc_init(void)
{
    ADMUX = (1 << REFS0); // Reference voltage = AVCC
    ADCSRA = (1 << ADEN)  // Enable ADC
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler = 128
}

// Read analog value from the specified ADC channel (0-7)
uint16_t read_adc(uint8_t ch)
{
    ch = ch & 0b00000111;      // Ensure channel is within 0-7
    ADMUX = (ADMUX & 0xF8) | ch; // Select ADC channel, preserving REFS bits

    ADCSRA |= (1 << ADSC);     // Start conversion
    while (!(ADCSRA & (1 << ADIF))); // Wait for conversion to complete

    ADCSRA |= (1 << ADIF);     // Clear ADIF by writing 1

    return ADC;                // Return 10-bit ADC result
}

// Map a value from one range to another
long map_value(long in_value, long in_min, long in_max, long out_min, long out_max)
{
    return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Initialize Timer1 in 8-bit Fast PWM mode with Prescaler = 64
void timer_init(void)
{
    TCCR1A = (1 << COM1A1) | (1 << WGM10); // Non-inverting Fast PWM, 8-bit
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescaler = 64
}
