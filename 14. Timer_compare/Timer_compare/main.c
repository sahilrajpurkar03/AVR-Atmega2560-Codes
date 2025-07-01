/*
 * Timer_compare.c
 *
 * Created: 11-08-2018 16:32:30
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define BAUD 9600                              // Serial communication baud rate
#define BAUDRATE  ((F_CPU/(BAUD*16UL))-1)

#define indicator_1 PORTK
#define IR_trigger_on (!(PINB & 0x01))        // IR trigger active low (fixed missing parenthesis)

// Function prototypes
void timer_start(void);
void timer_stop(void);
void process(void);
void port_init(void);

int s_mode = 0;
volatile int timer_count = 0;

int main(void)
{
    sei();          // Enable global interrupts
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
    indicator_1 = 0xFF;     // Set indicator full ON

    // Toggle indicator between 0xFF and 0xAA based on even/odd timer_count
    if (timer_count % 2 == 0)
    {
        indicator_1 = 0xAA;
    }

    /*
    // Example of IR trigger usage (commented out)
    if (IR_trigger_on)
    {
        timer_start();
        // Add condition on count or timer_count to stop timer and change mode
        if (timer_count >= SOME_THRESHOLD)  // Replace SOME_THRESHOLD as needed
        {
            timer_stop();
            s_mode = 8;
        }
    }
    */
}

void port_init(void)
{
    DDRK = 0xFF;            // Set PORTK as output for indicator_1
    DDRB &= ~(1 << 0);      // Set PINB0 as input (for IR trigger)
    PORTB |= (1 << 0);      // Enable pull-up on PINB0
}

void timer_start(void)
{
    // Configure Timer1 for CTC mode with OCR1A compare
    TCCR1A = 0x00;          // Normal port operation, CTC mode controlled by TCCR1B
    TCCR1B = 0x0D;          // CTC mode (WGM12=1), Prescaler 1024 (CS12=1, CS10=1)
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare A interrupt
    OCR1A = 14400;          // Set compare value (adjust as per timing requirements)
    TCNT1 = 0;              // Reset Timer1 counter
}

void timer_stop(void)
{
    TCCR1B = 0x00;          // Stop timer by clearing clock source bits
    TIMSK1 &= ~(1 << OCIE1A); // Disable Timer1 compare A interrupt
}

ISR(TIMER1_COMPA_vect)
{
    timer_count++;          // Increment timer count on compare match interrupt
}
