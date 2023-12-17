/*
 * LCD.c
 *
 * Created: 20-06-2018 14:22:29
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"



int main(void)
{
  init_ports();
  lcd_init();
  lcd_cursor(1,1);
  lcd_string("KJSCE");
  lcd_cursor(2,3);
  lcd_string("Sahil");
 }

