/*
 * main.c
 * LCD Display Test
 * Author: Sahil
 * Date: 20-06-2018
 */

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"   // Include custom LCD header

int main(void)
{
    init_ports();      // Initialize data direction for LCD port
    lcd_init();        // Initialize LCD in 4-bit mode

    lcd_cursor(1, 1);  // Set cursor to row 1, column 1
    lcd_string("KJSCE");   // Print first line

    lcd_cursor(2, 3);  // Set cursor to row 2, column 3
    lcd_string("Sahil");   // Print second line

    while (1);         // Infinite loop
    return 0;
}
