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
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void lcdSetUp();
void changeTemp(double temp[1]);
void changeHumi(double hum[1]);
void printSumError();
void printTimeError();
unsigned char Temp[] = "Temp: NA ";
unsigned char Humidity[] = "Humidity: NA";
unsigned char sErr[] = "SError";
unsigned char tErr[] = "TError";

int buttonPress = 0;


ISR(PCINT8_vect) {
	buttonPress = 1;
}


int main(void)
{	
	//DDRD |= 0x01; // Port C LSB is output
	//PCICR |= 0x04;
	//PCMSK2 |= 0x04;
	//EIMSK = 0x01; // Enable INT0
	//EICRA = 0x01; // INT0 on falling edge
	//DDRC |= 0x00; //Port C LSB in input?
	PCICR |= 0x02;						// turn on port C
	PCMSK1 |= 0x01;
	// LCD setup
	lcdSetUp();
	
	// DHT11 setup
	int8_t DHTreturnCode;
	while(1) {
		DHTreturnCode = DHT11ReadData();
		if (DHTreturnCode == 1 && buttonPress == 1) {
			// need to clear lcd from (0, 6) to (0, 13) here
			/* add code here to clear */
			cli();
			DHT11DisplayTemperature();
			DHT11DisplayHumidity();
			buttonPress = 0;
			//_delay_ms(5000);
			//printNA();
			sei();
		}
		if (DHTreturnCode == -1) {
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
	cli();
	_delay_ms(2);
	lcd_init();
	_delay_ms(1);
	lcd_clear();
	_delay_ms(2);
	lcd_write_word(Temp);
	lcd_goto_xy(1,0);
	lcd_write_word(Humidity);
	_delay_ms(1);
	lcd_send_command(0x0E);
	_delay_ms(2);
	lcd_send_command(0x0C);
	sei();
}

void printNA(){
	_delay_ms(2);
	lcd_write_word(Temp);
	lcd_goto_xy(1,0);
	lcd_write_word(Humidity);
	_delay_ms(1);
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
	lcd_write_word(sErr);
	_delay_ms(1);
}

void printTimeError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word(tErr);
	_delay_ms(1);
}


