/*
 * SPI.c
 *
 * Created: 19-07-2018 21:01:04
 * Author : Sahil
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// SPI port and pins definitions
#define port_SPI PORTB
#define pin_MOSI PINB2
#define pin_SCK  PINB1
#define pin_SS   PINB0

#define indicator_1 PORTH

// Function prototypes
void port_init(void);
void SPI_MasterInit(void);
void SPI_MasterTransmit(uint8_t data);

int main(void)
{
    int transmit_value;

    port_init();
    SPI_MasterInit();

    while (1)
    {
        for (transmit_value = 0; transmit_value < 16; transmit_value++)
        {
            SPI_MasterTransmit(transmit_value);
            indicator_1 = transmit_value;  // Show transmitted value on indicator port
            _delay_ms(2000);
        }
    }
}

// Initialize ports: MOSI, SCK, SS as output; indicator port as output
void port_init(void)
{
    DDRH = 0xFF;          // PORTH as output for indicator LEDs
    DDRB |= (1 << pin_MOSI) | (1 << pin_SCK) | (1 << pin_SS);  // MOSI, SCK, SS outputs
    port_SPI |= (1 << pin_SS);   // Set SS high (deselect slave)
}

// Initialize SPI in Master mode with fosc/8 clock
void SPI_MasterInit(void)
{
    port_SPI = (1 << pin_MOSI) | (1 << pin_SCK); // Set MOSI and SCK high initially (optional)
    
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  // Enable SPI, Master mode, fclk/16 (SPR0=1)
    SPSR = (1 << SPI2X);                            // Double SPI speed for fclk/8
}

// Transmit a byte via SPI
void SPI_MasterTransmit(uint8_t data)
{
    port_SPI &= ~(1 << pin_SS);  // Pull SS low to select slave
    
    SPDR = data;                 // Load data into SPI data register to start transmission
    
    while (!(SPSR & (1 << SPIF)))   // Wait for transmission complete
        ;
    
    port_SPI |= (1 << pin_SS);   // Pull SS high to deselect slave
}
