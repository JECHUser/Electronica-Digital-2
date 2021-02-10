
#ifndef _adc_
#define	_adc_

#include <xc.h>
#include <stdint.h>
void adc_convert(uint8_t channel);
void adc_setup(uint8_t format_result);
#endif

