/*
 * File: I2C_Library.h
 * Se utilizó y se adaptaron las librerías de Ligo George
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Revision history:
 * Created on 26 de febrero de 2021, 10:56 AM
 */

#include <xc.h>
#include <stdint.h>
#include "I2C_Library.h"

#define _XTAL_FREQ 8000000

void I2C_Master_Init(const unsigned long c){
    SSPCON = 0b00101000;
    SSPCON2 = 0b00000000;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0b00000000;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void I2C_Master_Wait(){
    while((SSPSTAT & 0b00000100) || (SSPCON2 & 0b00011111)){};
}

void I2C_Master_Start(void){
    I2C_Master_Wait();
    SSPCON2bits.SEN = 1;            // Start Condition enable bit
}

void I2C_RepeatedStart(void){
    I2C_Master_Wait();
    SSPCON2bits.RSEN = 1;           // Repeated Start Condition enable bit
}

void I2C_Master_Stop(){
    I2C_Master_Wait();
    SSPCON2bits.PEN = 1;            // Stop Condition enable bit
}

void I2C_Master_Write(unsigned data){
    I2C_Master_Wait();
    SSPBUF = data;
}

unsigned short I2C_Master_Read(unsigned short a){
    unsigned short temp;
    I2C_Master_Wait();
    SSPCON2bits.RCEN = 1;           // Receive enable bit
    I2C_Master_Wait();
    temp = SSPBUF;
    I2C_Master_Wait();
    SSPCON2bits.ACKDT = (a)?0:1;    // Acknowledge Data bit
    SSPCON2bits.ACKEN = 1;          // Acknowledge Sequence Enable bit
    return temp;
}