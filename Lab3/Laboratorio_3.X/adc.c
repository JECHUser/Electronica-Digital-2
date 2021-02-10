/*
 * File:   adc.c
 * Author: HP
 *
 * Created on 6 de febrero de 2021, 10:20 AM
 */


#include <xc.h>
#include "adc.h"

#define _XTAL_FREQ 8000000

void adc_convert(uint8_t channel){
    ADCON0bits.CHS = channel;
    __delay_ms(2);
    ADCON0bits.GO_nDONE = 1;
    if (ADCON0bits.GO_nDONE == 0){
        ADCON0bits.GO_nDONE = 1;
    }
}

void adc_setup(uint8_t result_format){
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    ADCON0 = 0b10000011;
    ADCON1bits.ADFM = result_format;
}


