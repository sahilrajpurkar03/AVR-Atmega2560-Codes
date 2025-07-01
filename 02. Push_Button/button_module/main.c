/*
 * button_module.c
 *
 * Created: 10-12-2018 14:48:13
 * Author : Sahil
 * Description: Button press detection with basic software debouncing.
 * Pressing a button lights up a corresponding LED on PORTA.
 */

#define F_CPU 14745600UL

#include <avr/io.h>
#include <util/delay.h>

// Define button input port and LED output port
#define buttonport PINE
#define indicator_1 PORTA

// Variables to store current, previous, and processed button states
uint8_t pres_butt[8] = {0};
uint8_t prev_butt[8] = {0};
uint8_t button[8]    = {0};

// Function Prototypes
void port_init(void);
void read_buttons(void);

int main(void)
{
    port_init();

    while (1)
    {
        read_buttons(); // Check for button presses and update LEDs accordingly
    }
}

// Initialize ports: PORTE as input for buttons, PORTA as output for indicators
void port_init(void)
{
    DDRE = 0x00; // Set PORTE (buttons) as input
    DDRA = 0xFF; // Set PORTA (LED indicators) as output
}

// Reads button states with debouncing and sets corresponding bits on PORTA
void read_buttons(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // Read current button state
        pres_butt[i] = (buttonport & (1 << i)) ? 1 : 0;

        if (pres_butt[i])
        {
            _delay_ms(5); // Simple debounce delay
            pres_butt[i] = (buttonport & (1 << i)) ? 1 : 0;

            // Detect new button press (rising edge)
            if (pres_butt[i] && !prev_butt[i])
            {
                button[i] = 1; // Mark button as newly pressed
            }
        }

        // Update previous state
        prev_butt[i] = pres_butt[i];
    }

    // Set indicator based on button press
    for (uint8_t i = 0; i < 8; i++)
    {
        if (button[i])
        {
            indicator_1 = (1 << i); // Light corresponding LED on PORTA
            button[i] = 0;          // Clear button state after handling
        }
    }
}
