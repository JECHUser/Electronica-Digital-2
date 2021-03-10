/*
 * File: I2C_Library.h
 * Se utiliz� y se adaptaron las librer�as de Ligo George
 * de la p�gina www.electrosome.com
 * Enlace: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Revision history:
 * Created on 26 de febrero de 2021, 10:56 AM
 */

#include <xc.h>
#include <stdint.h>
#include "I2C_Library.h"

#define _XTAL_FREQ 8000000

void I2C_Master_Init(const unsigned long c){
    SSPADD = (_XTAL_FREQ/(4*c))-1;  // velocidad de transmisi�n en baudios
    SSPSTAT = 0b00000000;           // velocidad de rotaci�n
    SSPCON = 0b00101000;            // modo maestro y habilitaci�n serial port
    
    SSPCON2 = 0b00000000;
    
    // configuraci�n de pines de transmisi�n/recepci�n
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void I2C_Master_Wait(){
    // evalua que ning�n proceso se realice en el fondo
    while((SSPSTAT & 0b00000100) || (SSPCON2 & 0b00011111)){}; 
}

void I2C_Master_Start(void){
    I2C_Master_Wait();              // espera que terminen procesos
    SSPCON2bits.SEN = 1;            // habilita proceso de Start Condition 
}

void I2C_Master_Stop(){
    I2C_Master_Wait();              // espera que terminen procesos
    SSPCON2bits.PEN = 1;            // habilita proceso de Stop Condition
}

void I2C_Master_Write(unsigned data){
    I2C_Master_Wait();              // espera que terminen procesos
    SSPBUF = data;                  // escribe al SSPBUF para enviar el dato
}

unsigned short I2C_Master_Read(unsigned short a){
    unsigned short temp;
    I2C_Master_Wait();
    SSPCON2bits.RCEN = 1;           // habilita la recepci�n de datos
    I2C_Master_Wait();
    temp = SSPBUF;                  // escribe a SSPBUF para transmisi�n
    I2C_Master_Wait();
    SSPCON2bits.ACKDT = (a)?0:1;    // Determina el valor del acknowledge
    SSPCON2bits.ACKEN = 1;          // habilita la secuencia del Acknowledge
    return temp;
}