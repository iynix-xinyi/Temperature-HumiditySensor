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
#include "DHT11sensor v1.0.h"
#include <avr/io.h>
#include <util/delay.h>

void lcdSetUp();
void changeTemp(double temp[1]);
void changeHumi(double hum[1]);
void printSumError();
void printTimeError();

int main(void)
{	
	// LCD setup
	lcdSetUp();
	
	// DHT11 setup
	int8_t DHTreturnCode;
	while(1) {
		DHTreturnCode = DHT11ReadData();
		if (DHTreturnCode == 1) {
			// need to clear lcd from (0, 6) to (0, 13) here
			/* add code here to clear */
			DHT11DisplayTemperature();
			DHT11DisplayHumidity();
		}
		else {
			if (DHTreturnCode == -1) {
				printSumError();	// print out error starting on (0, 6)
			}
			else {
				printTimeError();	// print out error starting on (0, 6)
			}
		}
	}
	
}

void lcdSetUp(){
	_delay_ms(2);
	lcd_init();
	_delay_ms(1);
	lcd_clear();
	_delay_ms(2);
	lcd_write_word("Temp: NA");
	lcd_goto_xy(1,0);
	lcd_write_word("Humidity: NA");
	_delay_ms(1);
	lcd_send_command(0x0E);
	_delay_ms(2);
	lcd_send_command(0x0C);
}

void changeTemp(double temp[1]) {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	char strTemp[3];
	sprintf(strTemp, "%f", temp[0]);
	lcd_write_word(strTemp);
	_delay_ms(1);
}

void changeHumi(double hum[1]) {
	lcd_goto_xy(1, 10);
	_delay_ms(1);
	char strHum[2];
	sprintf(strHum, "%f", hum[0]);
	lcd_write_word(strHum);
	_delay_ms(1);
}

void printSumError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word("SError");
	_delay_ms(1);
}

void printTimeError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word("TError");
	_delay_ms(1);
}
