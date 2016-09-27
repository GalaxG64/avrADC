// this file contains the code for easy to use ADC-code //

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc_328.h"


volatile uint8_t adcresults[ADC_CHANNELAMOUNT*CHANELDEPTH];
volatile uint8_t channelstatus[ADC_CHANNELAMOUNT];
volatile uint8_t adcstatusreg;
volatile uint8_t adcchannelcounter;

void init_adc_sc(void)
{
	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
	ADMUX |= (1 << ADLAR);					/*left adjust result*/
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0);	/* ADC clock prescaler 128 */
	ADCSRA |= (1 << ADEN);					/* enable ADC */
}

void init_adc_fr(uint8_t reference, uint8_t clocksetting, uint8_t setchannels)
{
	uint8_t j=0;
	uint8_t i=0;
	DDRC|= setchannels & ADC_INPUTPIN_MASK
	// This Code translates the set bits into the Channelstatus bytes so that the Library can work with it.
	while(j<ADC_CHANNELAMOUNT){
		if(setchannels != 0){
			while(!(setchannels & (1<<i))){		/* go through the setchannels byte indicatig that a channel is selected*/
				i++;
			}
			setchannels &= ~(1<<i);
			channelstatus[j] |= (i & ADC_CHANNEL_SELECTION_MASK);
			i=0;
		}
		j++;
	}

	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
	// TODO write atomatic frequency scailing code
		ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0);	/* ADC clock prescaler 128 */
	// TODO end
	ADCSRA |= (1 << ADEN);					/* enable ADC */
	ADCSRA |= (1 << ADIE);					/* set the interrupt enable Flag */

	if(setchannels==0){
		ADCSRA |= (1 << ADATE);				/* enable the auto triggering to free running mode for there a no channels to sample*/
		ADCSRA |= (1 << ADIE);				/* set the interrupt enable Flag */
	}
	else{
	ADCSRA |= (1 << ADATE);					/* enable the auto triggering to free running mode */
	ADMUX &= 0b11110000;					/* clear the admux bits */
	ADMUX |= (channelstatus[0] & 0b00001111);		/* set the first channel to sample */
	adcchannelcounter |= (j & ADC_CHANNEL_SELECTION_MASK);	/* set the amout of channels to be sampled*/
	}
}

void set_adc_reference( uint8_t reference)
{
	ADMUX &= ~((1<<REFS1) | (1<<REFS0));		/* clear the old reference Selection */
	ADMUX |= (reference & 0b11000000);		/* Set the new one */

}

uint8_t adc_read_8bit(uint8_t channel)
{
	ADMUX = ( 0b11110000 & ADMUX ) | channel;	/* select the channel */
	ADCSRA |= (1 << ADSC);				/* start the conversion */
	loop_until_bit_is_clear(ADCSRA, ADSC);		/* wait for the conversion to finish */
	ADCSRA |= (1<< ADIF);				/* clear the Interrupt Flag */
	return ADCH;					/* return the high byte of the result making it 8 bit resolution */
}

ISR(ADC_vect)
{
	// First we want to select the next channel to sample so we shift
	while(!((channelflagreg>>channel) & 0b00000001)){	/* Select the first channel to sample */
		channel++;
	}
	
}
