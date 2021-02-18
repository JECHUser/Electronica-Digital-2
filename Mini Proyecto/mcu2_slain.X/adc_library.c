/*
 * File:   adc_library.c
 * Author: HP
 *
 * Created on 18 de febrero de 2021, 11:18 AM
 */


#include <xc.h>
#include "adc_library.h"
#define _XTAL_FREQ 8000000

void adc_convert(void){
    __delay_ms(2);
    ADCON0bits.GO_nDONE = 1;
    if (ADCON0bits.GO_nDONE == 0){
        ADCON0bits.GO_nDONE = 1;
    }
}

void adc_setup(void){
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    ADCON0 = 0b00000011;
    ADCON1 = 0b00000000;
}
