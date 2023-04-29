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
#include "DHT.h"
#include <avr/io.h>
#include <util/delay.h>

void changeTemp(double temp[1]);
void changeHumi(double hum[1]);
void printSumError();
void printTimeError();

int main(void)
{
	_delay_ms(2);
    lcd_init();
	_delay_ms(1);
	lcd_clear();
	_delay_ms(2);
    lcd_write_word("Temp: ");
    lcd_goto_xy(1,0);
    lcd_write_word("Humidity: ");
	_delay_ms(1);
	lcd_send_command(0x0E);
	_delay_ms(2);
	lcd_send_command(0x0C);
	
	double temperature[1];
	double humidity[1];
	
	
	DHT_Setup();
	while (1) {
		DHT_Read(temperature, humidity);
		//Read from sensor
		DHT_Read(temperature, humidity);
		
		//Check status
		switch (DHT_GetStatus())
		{
			case (DHT_Ok):
				changeTemp(temperature);
				changeHumi(humidity);
				break;
			case (DHT_Error_Checksum):
				printSumError();
				break;
			case (DHT_Error_Timeout):
				printTimeError();
				break;
		}
		
		//Sensor needs 1-2s to stabilize its readings
		_delay_ms(1000);
	}
	
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
