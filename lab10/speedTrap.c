/********************************************
 *
 *  Name: Matt Baseheart
 *  Email: basehear@usc.edu
 *  Section: Friday 12:30
 *  Assignment: Final Project EE 109
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "speedTrap.h"
#include "encoder.h"
#include <avr/eeprom.h>
#include "serial.h"
	
void init_timer(){//DOES THE 16 AND 8bit TIMERS
	//When the TCNT1 register gets to 62500 cycles, trigger interrupt
	//Prescalar is 1024
	//Arduino is clocking at 15625 cycles per second
	
	//Set to CTC mode, Enable timer Interrupt, load MAX count 0.1 seconds (0.25 s with 16MHz clock)
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);							
	OCR1A = 62500; 									
	//TIMER 0, Prescalar
	TCCR0B |= (1 << WGM02);									
	TIMSK0 |= (1 << OCIE0A);
}

void play_note(){									
		toneTime = 0;
		hi = 0;
		finished = 0;
		OCR0A = 200; 			
		TCCR0B |= (1 << CS01);							
		while(finished == 0){}							
}

int main(void) {
	
	lcd_init();
	sei();
	
	//ROTARY ENCODER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		encoder_init(); 
		//READ CURRENT
		int temp = eeprom_read_byte((void *) 100); 
		if(0<temp && temp < 100){
			maxSpeed = temp;
		}
		else{
			maxSpeed = 0;
		}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//PHOTOTRANSISTORS
		//ENABLE PULLUP, get INITIAL INPUT
		PORTD |= (1<<PD2);							
		PORTD |= (1<<PD3);
		photInput = PIND;							
		if(photInput &(1<<PD2))
			firstP = 0;
		else
			firstP = 1;
		
		if(photInput &(1<<PD3))
			secondP = 0;
		else
			secondP = 1;
		
		PCICR |= (1<<PCIE2);							
		PCMSK2 |= (1<<PCINT18);
		PCMSK2 |= (1<<PCINT19);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	//TIMERS
		init_timer();
		//Set LED to output, turn it on and off once
		DDRB |= (1<< PB5);
		PORTB |= (1<<PB5);
		PORTB &= ~(1<<PB5);
		//Initialize timer variables
		c1 = 0; 
		c2 = 0;
		c3 = 0;
		c4 = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//BUZZER
		//Set as Output, initialize
		DDRB |= (1<<PB4);
		hi = 0;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//SERIAL LINK
		serial_init();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Write a splash screen to the LCD, then clear
	lcd_moveto(0,0);
	lcd_stringout(" Matt Baseheart");
	lcd_moveto(1,0);
	lcd_stringout(" Final Project ");
	_delay_ms(1000);
	lcd_writecommand(1);

    while (1) {
	//ENCODER
	    	//If value was changed, update eeprom
		if (rotChanged==1) {							
			rotChanged = 0;
			eeprom_update_byte((void *) 100, maxSpeed);
			if(maxSpeed<0)
				maxSpeed = 0;
			if(maxSpeed>99)
				maxSpeed = 99;
		}
		//Display updated speed limit
		lcd_moveto(1,0);
		lcd_stringout("MAX=");
		lcd_moveto(1,4);
		char buf [3];
		sprintf(buf, "%2d", maxSpeed);
		lcd_stringout(buf);
		lcd_moveto(1,6);
		lcd_stringout(" ");
			
		//PHOTOTRANSISTOR AND TIMER
	    	//Overtime = greater than 4 seconds
		if(overTime){
			lcd_moveto(0,0);
			lcd_stringout("OVER 4 SECONDS  ");
			overTime = 0;
			currTState = 0;	
			c4 = 0;
			c3 = 0;
			c2 = 0;
			c1 = 0;
			//Turn off LED and Timer
			PORTB &= ~(1<<PB5);		
			prevTState = currTState;
			TCCR1B &= ~(1 << CS10):
			TCCR1B &= ~(1 << CS12);
		}
		
	    	//STATE CONTROL, detailed notes shown to the left
		if(prevTState != currTState){						//ONLY CALLED ONCE UPON STATE CHANGE
			if(currTState == 1){						//First started
				TCNT1 = 0; 						//Clear count
				TCCR1B |= (1 << CS10);					// Set prescalar to 1024, start timer
				TCCR1B |= (1 << CS12);
				PORTB |= (1<<PB5);					//Turn on LED
			}
			if(currTState == 2){						//First stopped	
				TCCR1B &= ~(1 << CS10);					//Stop timer
				TCCR1B &= ~(1 << CS12);
				currTState = 0;		
				PORTB &= ~(1<<PB5);					//Turn off LED
				
				speed =(int)(((12*0.254)/(TCNT1/15625.0))*10.0);	//SPEED CALCULATOR	
				char theSpeed[5];					//distance = 12 rows = 1.2 inches, 0.1 inches = 0.254 cm
				sprintf(theSpeed, "%4d", speed);	
				sscanf(theSpeed, "%c%c%c%c", &s4, &s3, &s2, &s1);	//store speed digits in s variables for serial
				
				serial_txchar('<');					//BEGIN SENDING SPEED
				if(s4 != 0){
					char sHun [2];
					sprintf(sHun, "%1c", s4);
					lcd_moveto(0,7);
					lcd_stringout(sHun);
					serial_txchar(theSpeed[0]);
				}
				if(s3 != 0 || s4 != 0){
					char sTen [2];
					sprintf(sTen, "%1c", s3);
					lcd_moveto(0,8);
					lcd_stringout(sTen);
					serial_txchar(theSpeed[1]);
				}
				if(s2 != 0 || s3 != 0 || s4 != 0){
					char sOne [2];
					sprintf(sOne, "%1c", s2);
					lcd_moveto(0,9);
					lcd_stringout(sOne);
					serial_txchar(theSpeed[2]);
				}
				
				lcd_moveto(0,10);
				lcd_stringout(".");
				
				char sDec [2];
				sprintf(sDec, "%1c", s1);
				lcd_moveto(0,11);
				lcd_stringout(sDec);
				serial_txchar(theSpeed[3]);
				
				serial_txchar('>');					//FINISH SENDING SPEED

				lcd_moveto(0,12);
				lcd_stringout("cm/s");		
			}
			prevTState = currTState;					//Update state
		}
		
		if(currTState == 1){							//State 1 = Timing
			int theTime = (int)((TCNT1/15625.0)*1000);			//Time = count/(clocks/second) * 1000 to preserve the tenths decimal
			char theTimeChar[5];
			sprintf(theTimeChar, "%4d", theTime);
			sscanf(theTimeChar, "%c%c%c%c", &c4, &c3, &c2, &c1);
		
			char one [2];							//store digits of time into chars, print to LCD
			sprintf(one, "%1c", c4);
			lcd_moveto(0,0);
			lcd_stringout(one);
			
			char ten [2];
			sprintf(ten, "%1c", c3);
			lcd_moveto(0,1);
			lcd_stringout(ten);		
			
			char hun [2];
			sprintf(hun, "%1c", c2);
			lcd_moveto(0,2);
			lcd_stringout(hun);
			
			char thou [2];
			sprintf(thou, "%1c", c1);
			lcd_moveto(0,3);
			lcd_stringout(thou);
			
			lcd_moveto(0,4);
			lcd_stringout("ms=         ");
		}
	
	
	//SERIAL LINK
	    	//Show recieved speed
		if(showMessage == 1){	
			lcd_moveto(1,7);
			lcd_stringout("R");
			//lcd_moveto(1,8);
			//lcd_stringout(received);
			
			char size [2];
			sprintf(size, "%1d", sz);
			lcd_moveto(1, 8);
			lcd_stringout(size);
			
			int r4Val,r3Val, r2Val,r1Val, r0Val;
			sscanf(received, "%c%c%c%c%c", &r4, &r3, &r2, &r1, &r0);

			r4Val = r4 - '0';
			r3Val = r3 - '0';
			r2Val = r2 - '0';
			r1Val = r1 - '0';
			r0Val = r0 - '0';
			
			recSpeed = 0;
			
			if(r4 != 0){
				char rHun [2];
				sprintf(rHun, "%1c", r4);
				lcd_moveto(1, 9);
				lcd_stringout(rHun);
				if(rHun[0] != "-")
					recSpeed+=r4Val*100;
			}	
			if(r3 != 0){
				char rTen [2];
				sprintf(rTen, "%1c", r3);
				lcd_moveto(1, 10);
				lcd_stringout(rTen);
				if(rTen[0] != '-')
					recSpeed+=r3Val*10;
			}
			if(r2 != 0){
				char rOne[2];
				sprintf(rOne, "%1c", r2);
				lcd_moveto(1, 11);
				lcd_stringout(rOne);
				recSpeed+=r2Val*10;
			}
			lcd_moveto(1,12);
			lcd_stringout(".");
				
			char rDec [2];
			sprintf(rDec, "%1c", r1);
			lcd_moveto(1,13);
			lcd_stringout(rDec);
			recSpeed+=r1Val;
			
			sz = 0;
			showMessage = 0;
			
			//Determine if alarm should be played
			if((recSpeed/10.0)>maxSpeed){					
					play_note();
			}
		}
		
    }
}

//BUZZER TIMER
ISR(TIMER0_COMPA_vect) {
	if(hi == 0){
		PORTB |= (1 << PB4);
		hi = 1;
	}
	else{
		PORTB &= ~(1 << PB4);
		hi = 0;
	}
	
	toneTime++;
	if(toneTime == 3000){
		TCCR0B &= (0 << CS02);							
		TCCR0B &= (0 << CS01);
		TCCR0B &= (0 << CS00);
		finished = 1;
	}
}

//SPEED TIMER
ISR(TIMER1_COMPA_vect){									
		overTime = 1; 
}

//STATE CHANGE INTERRUPT: 0 = wait for start timing, 1 = started timing, 2 = stop timing, need to restart
ISR(PCINT2_vect){									
	photInput = PIND;
	//START state
	if((photInput &(1<<PD2)) == 0){	
		if(prevTState == 0){
			currTState = 1;
		}	
		firstP = 1;
	}
	else{
		firstP = 0;
	}	
	
	//STOP state
	if((photInput &(1<<PD3)) == 0){	
		if(prevTState == 1){
			currTState = 2;
		}	
			secondP = 1;
	}
	else{
		secondP = 0;
	}
}

//ENCODER INTERRUPT
ISR(PCINT1_vect){									

		int nmaxSpeed = 0;
		rotInput = PINC;
		if(rotInput & (1<<PC1))
			rotA = 1;
		else
			rotA = 0;
		if(rotInput & (1<<PC5))
			rotB = 1;
		else
			rotB = 0;
		
		if (oldRot == 0) { //rotA = rotB = 0
		// Handle rotA and rotB inputs for state 0
			if((rotA != 0)){
				newRot = 1;
				nmaxSpeed=1;
			}
			else
				if((rotB != 0) && (rotA == 0)){
				newRot = 2;
				nmaxSpeed=-1;
				}
		}
		else if (oldRot == 1) { //rotB = 0, rotA = 1
		// Handle rotA and rotB inputs for state 1
			if((rotB == 0) && (rotA == 0)){
				newRot = 0;
				nmaxSpeed=-1;
			}
			else
				if((rotB != 0) && (rotA != 0)){
				newRot = 3;
				nmaxSpeed=1;
				}
		}
		else if (oldRot == 2) { //rotB = 1, rotA = 0
		// Handle rotA and rotB inputs for state 2
			if((rotB != 0) && (rotA != 0)){
				newRot = 3;
				nmaxSpeed=-1;
			}
			else
				if((rotB == 0) && (rotA == 0)){
				newRot = 0;
				nmaxSpeed=1;
				}
		}
		else {   // oldRot = 3 //rotA = rotB = 1
			// Handle rotA and rotB inputs for state 3
			if((rotB != 0) && (rotA == 0)){
				newRot = 2;
				nmaxSpeed=1;
			}
			else
				if((rotB == 0) && (rotA != 0)){
				newRot = 1;
				nmaxSpeed=-1;
				}
		}
		if(nmaxSpeed != 0)									
			rotChanged = 1;
		else
			rotChanged = 0;
		maxSpeed = maxSpeed + nmaxSpeed;
		oldRot = newRot;								
}
