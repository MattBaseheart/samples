#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "encoder.h"               			// Declarations of the functions
#include "speedTrap.h"
void encoder_init(void){
		PORTC |= (1 << PC1) | (1 << PC5);	//Enable pullup resistors, connect PC1 and PC5 to encoder
		PCICR |= (1<<PCIE1);				//Set interrupt
		PCMSK1 |= (1<<PCINT9);
		PCMSK1 |= (1<<PCINT13);
		
		rotInput = PINC;					//get first state
			if(rotInput & (1<<PC1))
				rotA = 1;
			else
				rotA = 0;
			if(rotInput & (1<<PC5))
				rotB = 1;
			else
				rotB = 0;
			if (!rotB && !rotA)
				oldRot = 0;
			else if (!rotB && rotA)
				oldRot = 1;
			else if (rotB && !rotA)
				oldRot = 2;
			else
				oldRot = 3;
		
		newRot = oldRot;
		maxSpeed = 0;
		rotChanged = 0;
}

