
#ifndef adc
#define	adc

#include <xc.h>
#include <stdint.h>

void adc_convert(int channel);
void adc_setup(int format_result);
#endif

