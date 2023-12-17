/*
 * UART.c
 *
 * Created: 04-07-2018 13:26:25
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUD 9600									//serial com;
#define BAUDRATE  ((F_CPU/(BAUD*16UL)-1))

#define indicator_1 PORTB

void serialstart_1();
void port_init();
int value_1=0;
int main(void)
{
	sei();
	port_init();
	serialstart_1();
    while (1) 
    {
		indicator_1=value_1;
		
    }
}

void port_init()
{
	DDRB=0xFF;			//indiacator_1
}

void serialstart_1()
{
	UBRR1H=BAUDRATE>>8;
	UBRR1L=BAUDRATE;
	UCSR1B=0b10011000;//enable RXEN TXEN
	UCSR1C=0b00000110;// UCSZ1 UCSZ0
}

ISR(USART1_RX_vect)
{
	value_1=UDR1;
}

