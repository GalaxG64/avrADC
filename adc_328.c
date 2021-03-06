// this file contains the code for easy to use ADC-code //

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc_328.h"


volatile uint8_t adcresults[ADC_CHANNELAMOUNT*CHANELDEPTH];
volatile uint8_t channelstatus[ADC_CHANNELAMOUNT];
volatile uint8_t adcstatusreg;
volatile uint8_t adc_active_channel;

void init_adc_sc(void)
{
	ADMUX = 0;						/* clear the register before Writing to it */
	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
	ADMUX |= (1 << ADLAR);					/*left adjust result*/
	ADCSRA = 0;						/* clear the register before Writing to it */
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0);	/* ADC clock prescaler 128 */
	ADCSRA |= (1 << ADEN);					/* enable ADC */
}

void init_adc_fr(uint8_t reference, uint8_t clocksetting, uint8_t setchannels)
{
	uint8_t j=0;
	uint8_t i=0;
	ADC_DDR &= ~(setchannels & ADC_INPUTPIN_MASK)

	// This Code translates the set bits into the Channelstatus bytes so that the Library can work with it.
	while(j<ADC_CHANNELAMOUNT){
		if(setchannels != 0){
			while(!(setchannels & (1<<i))){		/* go through the setchannels byte indicatig that a channel is selected*/
				i++;
			}
			setchannels &= ~(1<<i);
			channelstatus[j] |= (1<<ADC_CHANNELSELECT);	/* set the Channelselect bit */
			channelstatus[j] |= (i & ADC_CHANNEL_SELECTION_MASK);	/* set the selected Channel */
			i=0;
		}
		else{
			channelstatus[j] &= 0xe0 ;		/* clear the channel from the list of sampled Channels*/
		}
		j++;
	}

	// TODO write atomatic frequency scailing code
	ADMUX |= (1 << REFS0);					/* reference voltage on AVCC */
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2) | (1 << ADPS0);	/* ADC clock prescaler 128 */
	ADCSRA |= (1 << ADEN);					/* enable ADC */
	ADCSRA |= (1 << ADIE);					/* set the interrupt enable Flag */
	ADMUX &= ~(1 << ADLAR);					/* set the reult right adjusted */

	if(setchannels==0){
		ADCSRA &= ~(1 << ADATE);				/* disable the auto triggering to free running mode for there a no channels to sample*/
		ADCSRA &= ~(1 << ADIE);					/* disable the interrupt enable Flag */
	}
	else{
		ADCSRA &= ~(1 << ADATE);				/* disable the auto triggering, we are emulating it in software */
		ADMUX &= 0b11110000;					/* clear the admux bits */
		ADMUX |= (channelstatus[0] & 0b00001111);		/* set the first channel to sample */
		adcstatusreg |= (j & ADC_CHANNEL_SELECTION_MASK);	/* set the amout of channels to be sampled*/
		adc_active_channel = 1;
	}
}

void set_adc_reference( uint8_t reference)
{
	ADMUX &= ~((1<<REFS1) | (1<<REFS0));		/* clear the old reference Selection */
	ADMUX |= (reference & 0b11000000);		/* Set the new one */
}

uint8_t adc_read_sc(uint8_t channel)
{
	ADMUX = ( (!ADC_CHANNEL_SELECTION_MASK) & ADMUX ) | (channel & ADC_CHANNEL_SELECTION_MASK);	/* select the channel */
	ADCSRA |= (1 << ADSC);				/* start the conversion */
	loop_until_bit_is_clear(ADCSRA, ADSC);		/* wait for the conversion to finish */
	ADCSRA |= (1<< ADIF);				/* clear the Interrupt Flag */
	return ADCH;					/* return the high byte of the result making it 8 bit resolution */
}

uint16_t adc_read_fr(uint8_t channel)
{
	uint16_t result=0;
	uint8_t i;
	uint8_t bufferwidth ADC_CHANNELDEPTH(ADC_CURRENT_CHANNELDEPTH)
	for( i=0; i<bufferwidth; i++ ){
		result |= (adcresults[(channel*bufferwidth)+i]<<(i*8))
	}
	return result;
}

ISR(ADC_vect)
{
	// TODO add the copy and set code in the ISR
	bufferpointer=adc_active_channel*ADC_CHANNELDEPTH(ADC_CURRENT_CHANNELDEPTH)
	adcresults[bufferpointer]=ADCL;
	adcresults[bufferpointer+1]=ADCH;
	// Select the next channel to sample
	do{
		adc_active_channel ++;
		if(adc_active_channel>=(adcstatusreg & ADC_CHANNEL_SELECTION_MASK)){
			adc_active_channel = 0 ;
			break;
		}
	}while(!(channelstatus[adc_active_channel]&(1<<ADC_CHANNELSELECT)))
	ADMUX |= channelstatus[adc_active_channel & ADC_CHANNEL_SELECTION_MASK];	/* Set the new channel to be sampled */
	ADCSRA |= (1<<ADSC);								/* Start the next conversion */
}
