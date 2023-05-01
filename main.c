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
#include <avr/interrupt.h>
#include <util/delay.h>

void switchPressed();
void lcdSetUp();
void changeTemp(double temp[1]);
void changeHumi(double hum[1]);
void printSumError();
void printTimeError();

/*************************************************************
	GLOBAL VARIABLES
**************************************************************/
volatile int buttonFlag = 0;		// off


/* Interrupt Service Routine for INT0 */
ISR(INT0_vect) {
	switchPressed();
}


int main(void)
{	
	cli();								// Disable interrupt
	DDRC |= 0b00000110;					// set led and back light as output
	EICRA = EICRA | 0x02;				// INT0 if high --> low
	EIMSK = EIMSK | 1 << INT0;			// Enable INT0

	// LCD setup
	lcdSetUp();
	sei();								// Enable interrupt
	
	// DHT11 setup
	int8_t DHTreturnCode;		
	// Check if it's reading correctly		
	while(1) {
		DHTreturnCode = DHT11ReadData();
		if (buttonFlag == 1) {			// if the button status is on
			if (DHTreturnCode == 1) {	// read it correctly
				cli();
				// Display temperature and humidity
				DHT11DisplayTemperature();
				DHT11DisplayHumidity();			
				// Check if the temperature is over 50C
				if (currentTemp >= 50) {
					turnOnLed();
				}
				else {
					turnOffLed();
				}
				sei();
			}
			// Reading in error
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
	
}


/*************************************************************
	FUNCTIONS
**************************************************************/
/* Initialize LCD */
void lcdSetUp(){
	_delay_ms(2);
	lcd_init();
	_delay_ms(1);
	lcd_clear();
	_delay_ms(2);
	lcd_send_command(0x0E);				// Cursor no blink
	_delay_ms(2);				
	lcd_send_command(0x0C);				// No Cursor
}

/* Print out sum error on LCD */
void printSumError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word("SError");
	_delay_ms(1);
}

/* Print out time error on LCD */
void printTimeError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word("TError");
	_delay_ms(1);
}

/* Turn on LED */
void turnOnLed() {
	unsigned char temp;
	temp = PORTC;
	temp = temp | 0b00000010;
	PORTC = temp;
	
}

/* Turn off LED */
void turnOffLed() {
	unsigned char temp;
	temp = PORTC;
	temp = temp &~(0b00000010);
	PORTC = temp;
}

/* if button is off, buttonFlag = 0, turn on back light for LCD.
	if button is on, buttonFlag = 1, turn off back light for LCD, and clear
	the lowest temperature stored in DHT11sensor file, clear the LCD screen,
	turn off LED. */
void switchPressed() {
	if (buttonFlag == 0) {
		buttonFlag = 1;
		unsigned char temp;
		// Turn on LCD back light
		temp = PORTC;
		temp = temp | 0b00000100;
		PORTC = temp;
		return;
	}
	else {
		buttonFlag = 0;
		turnOffLed();					// Turn off LED
		clearLow();						// Clear the lowest temperature stored in DHT11 file
		lcd_clear();					// Clear LCD display
		unsigned char temp;
		// Turn off LCD back light
		temp = PORTC;
		temp = temp &~(0b00000100);
		PORTC = temp;
	}	
}

