#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "serial.h" 
#include "speedTrap.h"

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register
#define ADC_CHAN 0              // Buttons use ADC channel 0

void serial_init()
{
	showMessage = 0;
	sz = 0;
	startRead = 0;
    // Set up USART0 registers
		UBRR0 = MYUBRR;
		UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable RX and TX
		UCSR0C = (3 << UCSZ00); // Async., no parity,
								// 1 stop bit, 8 data bits
    // Enable tri-state buffer
		DDRB |= (1<<PB3);
		PORTB &= ~(1<<PB3);
		
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
    while (i<4) {
        serial_txchar(s[i]);  // Send the character
        i++;
    }
}

ISR(USART_RX_vect)
{
	char ch;
    ch = UDR0;// Get the received charater

	if(ch == '<'){//if start, reset and set flags
		sz = 0;
		showMessage = 0;
		startRead = 1;
	}
	else{
		if((ch == '>') || (sz == 4)){//if end or max, set flags
			if(startRead == 1){
				showMessage = 1;
				startRead = 0;
			}
		}
		else{//not start OR end
			if(startRead == 1){
				received[sz] = ch;
				sz++;
			}
		}
	}
	

}