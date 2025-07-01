/*
 * timer_task.c
 *
 * Created: 17-08-2018 12:03:00
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define BAUD 9600                                     // Serial communication baud rate
#define BAUDRATE  ((F_CPU/(BAUD*16UL))-1)

#define indicator_1 PORTK
#define indicator_2 PORTC
#define IR_trigger_on (!(PINA & 0x01))                // IR sensor active low on PINA0

// Function prototypes
void timer_start(void);
void timer_stop(void);
void process(void);
void port_init(void);

// Global variables
int s_mode = 0;
volatile int timer_count = 0;
volatile int IR_count = 0;
volatile int IR_trigger = 0;

int main(void)
{
    sei();                  // Enable global interrupts
    port_init();

    while (1)
    {
        process();

        // Show IR trigger status on PORTK and state mode on PORTC
        indicator_1 = IR_trigger;
        indicator_2 = s_mode;
    }
}

void process(void)
{
    timer_start();          // Start timer and monitoring IR sensor

    s_mode = 0;
    if (IR_trigger == 5)    // If IR has been detected continuously for threshold
    {
        s_mode = 8;         // Set mode to 8
        timer_stop();       // Optional: stop timer if mode set (depends on application)
    }
}

void port_init(void)
{
    DDRK = 0xFF;            // PORTK as output for indicator_1
    DDRC = 0xFF;            // PORTC as output for indicator_2
    DDRA &= ~(1 << 0);      // PINA0 as input (IR sensor)
    PORTA |= (1 << 0);      // Enable pull-up resistor on PINA0
}

void timer_start(void)
{
    TCCR1A = 0x10;          // Clear OC1A on compare match, set at BOTTOM (non-PWM)
    TCCR1B = 0x0D;          // CTC mode (WGM12 = 1), prescaler = 1024 (CS12 and CS10)
    TIMSK1 |= (1 << OCIE1A);// Enable Timer1 compare A interrupt
    OCR1A = 14400;          // Compare value for interrupt frequency
    TCNT1 = 0;              // Reset timer count
}

void timer_stop(void)
{
    TCCR1A = 0x00;          // Stop timer: clear control registers
    TCCR1B = 0x00;
    TIMSK1 = 0x00;          // Disable all Timer1 interrupts

    // Reset IR detection counters
    IR_count = 0;
    IR_trigger = 0;
}

ISR(TIMER1_COMPA_vect)
{
    if (IR_trigger_on)      // IR sensor is triggered (active low)
    {
        IR_count++;         // Increment count of continuous IR trigger events
    }
    else
    {
        IR_count = 0;       // Reset count if IR not triggered
        IR_trigger = 0;     // Reset trigger flag
    }

    if (IR_count > 4)       // If IR detected for more than 4 consecutive timer interrupts
    {
        IR_trigger = 5;     // Set IR trigger flag
    }
}
