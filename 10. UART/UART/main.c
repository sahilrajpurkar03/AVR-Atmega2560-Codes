/*
 * UART.c
 *
 * Created: 04-07-2018 13:26:25
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600                           // UART baud rate
#define BAUDRATE ((F_CPU / (BAUD * 16UL)) - 1)

#define indicator_1 PORTB                   // Output port for indication

// Function prototypes
void port_init(void);
void serialstart_1(void);

// Global variable to hold received UART data
volatile int value_1 = 0;

int main(void)
{
    sei();               // Enable global interrupts
    port_init();         // Initialize ports
    serialstart_1();     // Initialize UART

    while (1) 
    {
        indicator_1 = value_1;    // Display received value on PORTB LEDs
    }
}

// Initialize PORTB as output for indicator LEDs
void port_init(void)
{
    DDRB = 0xFF;           // Set PORTB as output
}

// Initialize UART1 for 9600 baud, 8 data bits, no parity, 1 stop bit
void serialstart_1(void)
{
    UBRR1H = (BAUDRATE >> 8);           // Set baud rate high byte
    UBRR1L = BAUDRATE;                  // Set baud rate low byte
    
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1); // Enable RX, TX and RX interrupt
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);               // 8-bit data frame
}

// UART RX Complete Interrupt Service Routine
ISR(USART1_RX_vect)
{
    value_1 = UDR1;          // Read received data from UART data register
}
