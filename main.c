/*
 * TempAndHumdity.c
 *
 * Created: 4/28/2023 4:51:49 PM
 * Author : Xinyi Yuan
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL	// 16 MHz clock speed
#endif

#include "LCD.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	_delay_ms(2);
    lcd_init();
	_delay_ms(1);
	lcd_clear();
	_delay_ms(2);
    lcd_write_word("Temp: ");
    lcd_goto_xy(1,0);
    lcd_write_word("LCD is working");
	_delay_ms(1);
	lcd_send_command(0x0E);
	_delay_ms(2);
	lcd_send_command(0x0C);
}

