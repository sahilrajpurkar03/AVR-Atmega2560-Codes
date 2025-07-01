/*
 * SPI_SLAVE.c
 *
 * Created: 27-07-2018 18:56:16
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// SPI port and pin definitions
#define port_SPI PORTB
#define pin_MISO PINB3
#define pin_SS   PINB0

#define indicator_1 PORTH

// Function prototypes
void port_init(void);
void SPI_SlaveInit(void);
uint8_t SPI_SlaveReceive(void);

int main(void)
{
    port_init();
    SPI_SlaveInit();

    while (1)
    {
        // Receive SPI data and display on indicator port
        indicator_1 = SPI_SlaveReceive();
    }
}

// Initialize ports for SPI Slave and indicator
void port_init(void)
{
    DDRH = 0xFF;               // PORTH as output (indicator LEDs)
    DDRB &= ~((1 << pin_SS) | (1 << PINB4) | (1 << PINB5) | (1 << PINB6)); // Ensure SPI pins (except MISO) as input
    DDRB |= (1 << pin_MISO);   // Set MISO as output
    port_SPI |= (1 << pin_SS); // Set SS high by default (not selected)
}

// Initialize SPI in Slave mode
void SPI_SlaveInit(void)
{
    port_SPI = (1 << pin_MISO);  // Set MISO high initially (optional)
    
    // Enable SPI in Slave mode (SPE=1, MSTR=0)
    SPCR = (1 << SPE);
}

// Receive a byte from SPI Master
uint8_t SPI_SlaveReceive(void)
{
    // Wait for reception complete (SPIF flag set)
    while (!(SPSR & (1 << SPIF)))
        ;

    // Return received data from SPI data register
    return SPDR;
}
