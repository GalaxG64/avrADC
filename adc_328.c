// this file contains the code for easy to use ADC-code //

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc_328.h"

void init_adc_sc(void)
{
	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
	ADMUX |= (1 << ADLAR);					/*left adjust result*/
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0);	/* ADC clock prescaler 128 */
	ADCSRA |= (1 << ADEN);					/* enable ADC */
}

void init_adc_fr(void)
{
	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
	ADMUX |= (1 << ADLAR);					/*left adjust result*/
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0);	/* ADC clock prescaler 128 */
	ADCSRA |= (1 << ADEN);					/* enable ADC */
	ADCSRA |= (1 << ADATE);					/* enable the auto triggering to free running mode trigger sources can be selected with bits in the ADCSRB register */
}

uint8_t adc_read_8bit(uint8_t channel)
{
	ADMUX = ( 0b11110000 & ADMUX ) | channel;	/* select the channel */
	ADCSRA |= (1 << ADSC);				/* start the conversion */
	loop_until_bit_is_clear(ADCSRA, ADSC);		/* wait for the conversion to finish */
	ADCSRA |= (1<< ADIF);				/* clear the Interrupt Flag */
	return ADCH;					/* return the high byte of the result making it 8 bit resolution */
}
