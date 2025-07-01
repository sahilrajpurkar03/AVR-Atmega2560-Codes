/*
 * Drive_Encoder_Interrupt.c
 *
 * Created: 30-07-2018 19:23:29
 * Author : Sahil
 * Description: Reads a quadrature encoder using external interrupts (INT0 and INT1),
 *              tracks pulses, and displays a value on PORTK (indicator).
 */

#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define indicator_1 PORTK  // Output port for display/indication

// Function declarations
void port_init(void);
void drive_encoder(void);
void eint_init(void);
void eint_en(int eint_pin);
void eint_dis(int eint_pin);

// Global encoder state variables
volatile int pulse_A = 0, pulse_B = 0;
volatile int disp = 0;
volatile int i = 0;

int main(void)
{
	sei();             // Enable global interrupts
	port_init();       // Initialize I/O
	eint_init();       // Initialize interrupt configuration

	while (1)
	{
		drive_encoder();       // Read encoder pulses
		indicator_1 = disp;    // Display the value
	}
}

// Configure port directions
void port_init(void)
{
	DDRK = 0xFF;     // PORTK as output for indicator
	DDRD = 0x00;     // PORTD as input for encoder signals
	PORTD = 0xFF;    // Enable pull-up resistors
}

// Set interrupt triggers for INT0 and INT1 (any logical change)
void eint_init(void)
{
	EICRA |= (1 << ISC10) | (1 << ISC00);  // INT0 & INT1 on any edge
}

// Enable specific external interrupt (0 for INT0, 1 for INT1)
void eint_en(int eint_pin)
{
	EIMSK |= (1 << eint_pin);
}

// Disable specific external interrupt
void eint_dis(int eint_pin)
{
	EIMSK &= ~(1 << eint_pin);
}

// ISR for INT1 (typically channel A of encoder)
ISR(INT1_vect)
{
	if (PIND & (1 << 1))       // PD1 (INT1) HIGH
	{
		pulse_A = 1;
	}
	else
	{
		pulse_A = 0;
	}
}

// ISR for INT0 (typically channel B of encoder)
ISR(INT0_vect)
{
	if (PIND & (1 << 0))       // PD0 (INT0) HIGH
	{
		pulse_B = 1;
	}
	else
	{
		pulse_B = 0;
	}
}

// Quadrature decoding logic to determine direction and count
void drive_encoder(void)
{
	eint_en(0);   // Enable INT0
	eint_en(1);   // Enable INT1

	if (i == 0)
	{
		// One of the encoder channels just went low
		if (pulse_A == 0 || pulse_B == 0)
		{
			i = 1;  // Debounce: block next event until full step
			if (pulse_A == 0)
			{
				disp++;  // Clockwise rotation
			}
			if (pulse_B == 0)
			{
				disp--;  // Counter-clockwise rotation
			}
		}
	}

	// Reset state when both channels go HIGH
	if (pulse_A == 1 && pulse_B == 1)
	{
		i = 0;
	}
}
