/*
 * File:   spi_library.c
 * Author: HP
 *
 * Created on 23 de febrero de 2021, 05:21 PM
 */


#include <xc.h>
#include "spi_library.h"

static void wait(){
    while(!SSPSTATbits.BF);
}
char SPI_read(void){
    wait();
    return(SSPBUF);
}
void SPI_write(char data){
    SSPBUF = data;
}
void SPI_Master_Init(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC3 = 0;   
    PIE1bits.SSPIE = 1;
    SSPSTAT = 0b00000000;
    SSPCON = 0b00100000;
}
void SPI_Slave_Init(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC3 = 1;
    PIE1bits.SSPIE = 1;
    SSPSTAT = 0b00000000;
    SSPCON = 0b00100100;
}

