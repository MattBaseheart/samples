/********************************************
 *
 *  Name: Matt Baseheart
 *  Email: basehear@usc.edu
 *  Section: F 12:30
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "adc.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register

// ADC functions and variables

#define ADC_CHAN 0              // Buttons use ADC channel 0


char *messages[] = {
    "Hello           ",
    "How are you?    ",
	"I'm ok          ",
	"Is this lab done",
	"Nope not yet    ",
	"I'm tired       ",
	"Will I pass?    ",
	"Not really sure ",
	"What time is it ",
	"Too early       "
    // Add more messgages
};

	//GLOBAL VARIABLES
	volatile unsigned char count = 0;
	volatile unsigned char received[16];
	volatile unsigned char sz = 0;
	volatile unsigned char showMessage = 0;
	volatile unsigned char added = 0;
	
int main(void) {

    // Initialize the LCD, ADC and serial modules
		lcd_init();
		lcd_writecommand(1); //clear LCD

		adc_init();
		
		serial_init(MYUBRR);
		
		sei();

    // Show the splash screen
		lcd_moveto(0,1);
		lcd_stringout("Matt Baseheart");
		lcd_moveto(1,4);
		lcd_stringout("Lab 8");
		_delay_ms(1000);
		lcd_writecommand(1); //clear LCD
		
	//Variables
		unsigned char up = 0;
		unsigned char down = 0;
		unsigned char select = 0;
		unsigned char lastPressed = 0;
		unsigned char mIndex = 0;
		unsigned char send = 0;
		
    while (1) {                 // Loop forever

	// Get an ADC sample to read buttons
		unsigned char bVal = adc_sample(0);
			
        // Up button pressed? (50 ish), No button = 255
		if(bVal >= 45 && bVal <= 55 && lastPressed == 0){
			up = 1; down = 0; select = 0;
			lastPressed = 1;
			if(mIndex == 9)
				mIndex = -1;
			mIndex ++;
		}
		else{
        // Down button pressed? (100 ish)
			if(bVal >= 95 && bVal <= 105 && lastPressed == 0){
				up = 0; down = 1; select = 0;
				lastPressed = 1;
				if(mIndex == 0)
					mIndex = 10;
				mIndex --;
			}
			else{
        // Select button pressed? (205 ish)
				if(bVal >= 200 && bVal <= 210 && lastPressed == 0){
					up = 0; down = 0; select = 1;
					lastPressed = 1;
					send = 1;
				}
				else{
					if(bVal>210)
						lastPressed = 0;
					up = 0; down = 0; select = 0;
				}
			}
		}
		
		//Display Message Top (Sending)
		lcd_moveto(0,0);
		lcd_stringout(*(messages + mIndex));
		
		if(select == 1){//Send message
			select = 0;
			serial_stringout(*(messages + mIndex)); //might be & instead of *
		}
		
        // Message received from remote device?
		if(showMessage == 1){
			lcd_moveto(1,0);
			char buf2 [16];
			sprintf(buf2, "%16s", received);
			lcd_stringout(buf2);
			sz = 0;
			showMessage = 0;
		}

    }
}

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short ubrr_value)
{
    // Set up USART0 registers
		UBRR0 = MYUBRR;
		UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable RX and TX
		UCSR0C = (3 << UCSZ00); // Async., no parity,
								// 1 stop bit, 8 data bits
    // Enable tri-state buffer
		DDRD |= (1<<PD3);
		PORTB &= ~(1<<PD3);
		
	// Enable receiver interrupts
		UCSR0B |= (1 << RXCIE0);   
		

}

void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0){}
    UDR0 = ch;
}

void serial_stringout(char *s)
{
    // Call serial_txchar in loop to send a string
    // (Very similar to lcd_stringout in lcd.c)
	int i = 0;
    while (i<16) {
        serial_txchar(s[i]);  // Send the character
        i++;
    }
}

ISR(USART_RX_vect)
{
	char ch;

    ch = UDR0;                  // Get the received charater

    // Store in buffer
	received[sz] = ch;
	sz++;
    // If message complete, set flag
	if(sz == 15){
		showMessage = 1;
	}

}
