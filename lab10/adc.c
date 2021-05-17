#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC
	//ADMUX &= 0x00;//reset everything
	ADMUX &= ~(1<<REFS1);
	ADMUX |= (1<<REFS0);
	
	//ADCSRA &= 0x00; //reset everything
	ADCSRA |= (1<<ADPS2);// set ADCSRA bit 7 to 1 to enable ADC (ADEN)
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS0);
	ADMUX |= (1<<ADLAR);
	ADCSRA |= (1<<ADEN);
}

unsigned char adc_sample(unsigned char channel)
{
    // Set ADC input mux bits to 'channel' value
		ADMUX &= ~(0x0F);
		ADMUX |= channel;


	ADCSRA |= (1<<ADSC);
	
	while((ADCSRA & (1<<ADSC)) != 0)
	{}

	unsigned char result = ADCH;
	
    // Convert an analog input and return the 8-bit result
	return result;
}
