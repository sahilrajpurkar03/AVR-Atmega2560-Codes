#include "lcd.h"

// Initialize LCD ports
void init_ports(void)
{
    DDRC = DDRC | 0xF7;    // Set PC0-PC2, PC4-PC7 as output
    PORTC = PORTC & 0x80;  // Clear all LCD bits except PC7
}

// Set LCD to 4-bit mode
void lcd_set_4bit(void)
{
    _delay_ms(1);

    for (int i = 0; i < 3; i++) {
        cbit(lcd_port, RS);
        cbit(lcd_port, RW);
        lcd_port = 0x30;
        sbit(lcd_port, EN);
        _delay_ms(5);
        cbit(lcd_port, EN);
        _delay_ms(1);
    }

    cbit(lcd_port, RS);
    cbit(lcd_port, RW);
    lcd_port = 0x20;    // 4-bit mode
    sbit(lcd_port, EN);
    _delay_ms(5);
    cbit(lcd_port, EN);
}

// Initialize LCD
void lcd_init(void)
{
    lcd_set_4bit();
    _delay_ms(1);

    lcd_wr_command(0x28);  // 4-bit mode, 2-line, 5x8 font
    lcd_wr_command(0x01);  // Clear display
    lcd_wr_command(0x06);  // Increment cursor
    lcd_wr_command(0x0E);  // Display ON, cursor ON
    lcd_wr_command(0x80);  // Set DDRAM address to 0
}

// Send command to LCD
void lcd_wr_command(unsigned char cmd)
{
    lcd_port = (lcd_port & 0x0F) | (cmd & 0xF0);  // Send high nibble
    cbit(lcd_port, RS);
    cbit(lcd_port, RW);
    sbit(lcd_port, EN);
    _delay_ms(5);
    cbit(lcd_port, EN);

    lcd_port = (lcd_port & 0x0F) | ((cmd << 4) & 0xF0);  // Send low nibble
    cbit(lcd_port, RS);
    cbit(lcd_port, RW);
    sbit(lcd_port, EN);
    _delay_ms(5);
    cbit(lcd_port, EN);
}

// Write character to LCD
void lcd_wr_char(char letter)
{
    lcd_port = (lcd_port & 0x0F) | (letter & 0xF0);  // High nibble
    sbit(lcd_port, RS);
    cbit(lcd_port, RW);
    sbit(lcd_port, EN);
    _delay_ms(5);
    cbit(lcd_port, EN);

    lcd_port = (lcd_port & 0x0F) | ((letter << 4) & 0xF0);  // Low nibble
    sbit(lcd_port, RS);
    cbit(lcd_port, RW);
    sbit(lcd_port, EN);
    _delay_ms(5);
    cbit(lcd_port, EN);
}

// Set cursor to given row and column
void lcd_cursor(char row, char column)
{
    switch (row) {
        case 1: lcd_wr_command(0x80 + column - 1); break;
        case 2: lcd_wr_command(0xC0 + column - 1); break;
        case 3: lcd_wr_command(0x94 + column - 1); break;
        case 4: lcd_wr_command(0xD4 + column - 1); break;
        default: break;
    }
}

// Write string to LCD
void lcd_string(char *str)
{
    while (*str != '\0') {
        lcd_wr_char(*str++);
    }
}

// Print integer value at specific LCD location with fixed digits
void lcd_print(char row, char column, unsigned int value, int digits)
{
    if (row == 0 || column == 0)
        lcd_wr_command(0x80);
    else
        lcd_cursor(row, column);

    if (digits > 5) {
        lcd_wr_char('E');
        return;
    }

    char buffer[6] = {0};  // Max 5 digits + null terminator
    for (int i = digits - 1; i >= 0; i--) {
        buffer[i] = (value % 10) + '0';
        value /= 10;
    }

    for (int i = 0; i < digits; i++) {
        lcd_wr_char(buffer[i]);
    }
}
