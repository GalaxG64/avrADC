#ifndef ADC_H
#define ADC_H

// These system defines set the depth and the amount of the channels
#define ADC_CHANNELAMOUNT 8
#define ADC_CHANNELDEPTH(bit) ((((bit)-1)/8)+1)		// CAREFUL you are not allowed to enter a 0 into this funktion or it will produce massive buffers
#define ADC_CHANNEL_SELECTION_MASK 0x0f
#define ADC_INPUTPIN_MASK 0xbf
#define ADC_DDR DDRC

// These Defines are used as reference Selectors in the set_adc_reference
#define ADC_REF_AREF 0
#define ADC_REF_AVCC (1<<REFS0)
#define ADC_REF_INTERNAL (1<<REFS0)|(1<<REFS1)

// These defines are used for the clock setting of the ADC
#define ADC_SPEED_FAST 1
#define ADC_SPEED_ANDANTE 2
#define ADC_SPEED_SLOW 3

void init_adc_sc(void);
void init_adc_fr(void);
uint8_t adc_read_8bit(uint8_t channel);
#endif
