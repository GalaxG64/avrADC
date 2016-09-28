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
The number of the bit corresponds to the channelnumber and is also used to clear the appropriate bit in the DDR register of the chip.
</p>
<p>
The Free running Mode uses an ISR-based buffering mechanism to aquire and store the ADC Results and grant the CPU random acces to them via the <b>adc_read_fr</b> Funktion that returns a uint16_t witout waiting for the ADC to finish a Conversion.
</p>
<p>
The Free running mode does not allow to change the channels being monitored without reinitialising the software, and does not allow for the use of Single conversons.
</p>
