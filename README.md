# avrADC
## Design
The library supports both single Read and a free running mode.
It operates entirely ISR-based when set into free running mode minimizing CPU load. Also it behaves rather Arduino like when set to Single conversion mode.
### Single Conversion
<p>
In Single conversion Mode the User Initialises the ADC with the <b>init_adc_sc</b> Funktion and can then start a conversion
using the <b>adc_read_sc(channel)</b>. This funktion waits for the end of the coversion and blocks the CPU while doing so.
if multiple channels are to be sampled the use of the "free running" mode is recomended
</p>
<p>
When in single conversion mode it is possible to swich references using the <b>set_adc_reference</b> funktion.
### Free running Mode
<p>
in Free running mode the user has to give the init funktion a byte that sets the Channels to be read in by setting the appropriate bit the byte.
The number of the bit corresponds to the channelnumber and is also used to clear the appropriate bit in the DDR register of the chip. (This feature momentarily only applies to the 328 Chip from Atmel)
</p>
#### Configuration
<p>
If the Use of the Free running mode is desired, be aware, that for the correct funktion of the Library, two defines have to be set to guarantee an error free execution. The first is the ADC_CURRENT_CHANNELDEPTH in bit and the ADC_CHANNELAMOUNT. These Defines govern the Generation of the buffer to later store the ADC results. The Channeldepth should not exeed 16 bit
</p>
<p>
The Free running Mode uses an Interrupt-based buffering mechanism to aquire and store the ADC Results in a software implemented Buffer and grant the CPU random acces to it via the <b>adc_read_fr</b> Funktion that returns the buffered value of the respective Chanel.
The buffering mechanism rotates through the chanels set to be active and sets the appropriate ADMUX bits before starting a conversion and then stores the conversion, before repeating the cycle over. The funktion <b>adc_read_fr</b> only acesses the buffer and so does not have to wait for a result to be generated. This uses the available Hardware to its optimum reducing waittime and keeping the ADC buisy nearly all the time.
</p>
<p>
The Free running mode does not allow to change the channels being monitored without reinitialising the software, and does not allow for the use of Single conversons during its opreation (the init Funktions are not yet built to overwrite all settings in the respective Registers).
</p>
##TODO'S
* Edit Init funktions to overwrite old values
* Do actual Hardware testing

