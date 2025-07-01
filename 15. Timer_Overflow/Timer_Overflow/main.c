/*
 * Timer_Overflow.c
 *
 * Created: 11-08-2018 16:35:34
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define BAUD 9600                               // Serial communication baud rate
#define BAUDRATE  ((F_CPU/(BAUD*16UL))-1)

#define indicator_1 PORTK
#define IR_trigger_on (!(PINB & 0x01))          // IR trigger active low (fixed missing parenthesis)

// Function prototypes
void timer_start(void);
void timer_stop(void);
void process(void);
void port_init(void);

int s_mode = 0;
volatile int timer_count = 0;

int main(void)
{
    sei();              // Enable global interrupts
    port_init();

    while (1)
    {
        process();
        // indicator_1 = s_mode;  // Uncomment to display mode on PORTK
    }
}

void process(void)
{
    timer_start();          // Start timer
    indicator_1 = 0x00;     // Turn off indicator by default

    // Toggle indicator between 0x00 and 0xFF based on even/odd timer_count
    if (timer_count % 2 == 0)
    {
        indicator_1 = 0xFF; 
    }
}

void port_init(void)
{
    DDRK = 0xFF;            // Set PORTK as output for indicator_1
    DDRB &= ~(1 << 0);      // Set PINB0 as input (for IR trigger)
    PORTB |= (1 << 0);      // Enable pull-up on PINB0
}

void timer_start(void)
{
    TCCR1A = 0x00;          // Normal mode (no PWM)
    TCCR1B = 0x05;          // Prescaler = 1024 (CS12 and CS10 bits set)
    TIMSK1 |= (1 << TOIE1); // Enable Timer1 overflow interrupt
    TCNT1 = 0;              // Reset timer counter
}

void timer_stop(void)
{
    TCCR1B = 0x00;          // Stop timer by clearing clock source bits
    TIMSK1 &= ~(1 << TOIE1);// Disable Timer1 overflow interrupt
}

ISR(TIMER1_OVF_vect)
{
    timer_count++;          // Increment timer count on overflow interrupt
}
