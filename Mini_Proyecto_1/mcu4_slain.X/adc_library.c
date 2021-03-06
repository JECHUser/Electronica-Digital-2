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
    if(ADCON0bits.GO == 0){
        __delay_ms(5);
        ADCON0bits.GO = 1;
        __delay_ms(5);
    }
}

void adc_setup(void){
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    ADCON0 = 0b00000011;
    ADCON1 = 0b00010000;
}
