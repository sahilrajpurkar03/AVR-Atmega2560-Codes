/*
 * lcd.h
 * LCD Interface Library for AVR in 4-bit Mode
 * Author: Sahil
 */

#ifndef LCD_H_
#define LCD_H_

#include <util/delay.h>
#include <avr/io.h>

// LCD pin definitions
#define RS 0
#define RW 1
#define EN 2
#define lcd_port PORTC

// Macros for setting and clearing bits
#define sbit(reg, bit)   (reg |= (1 << bit))
#define cbit(reg, bit)   (reg &= ~(1 << bit))

// Function Declarations
void init_ports(void);
void lcd_set_4bit(void);
void lcd_init(void);
void lcd_wr_command(unsigned char);
void lcd_wr_char(char);
void lcd_cursor(char row, char column);
void lcd_string(char *);
void lcd_print(char row, char column, unsigned int value, int digits);

// Optional: Global digits for lcd_print
unsigned int temp, unit, tens, hundred, thousand, million;

#endif
