/*
 * File:   USART_Library.c
 * Se utilizó y se adaptaron las librerías de Ligo George
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Revision history:
 * Created on 26 de febrero de 2021, 02:03 PM
 */

#include <xc.h>
#include "UART_Library.h"

#define _XTAL_FREQ 8000000

char UART_Init(void) {
         // configuración de baud rate
        SPBRGH = 0;
        SPBRG = 25;
        BRGH = 1;
        SYNC = 0; 
        
        // configuración UART
        SPEN = 1; 
        TX9 = 0;
        CREN = 1;   // habilitar recepción
        TXEN = 1;   // habilitar transmisión
        
        // configuración pines de transmisión/recepción
        TRISC7 = 1;
        TRISC6 = 0;       
}

char UART_DATA_Read() {
    while (!PIR1bits.RCIF);     // Evalua la bandera de recepción
    return RCREG;               // devuelve el valor que llega al RCREG
}

void UART_DATA_Write(char data) {   
    TXREG = data;               // se escribe el dato a enviar 
    while (!TXSTAbits.TRMT);    // Evalua si el TSR se encuentra lleno
}

char UART_DATA_Ready() {
    return RCIF;    // evalua la bandera de recepción
}

char UART_TX_EMPTY() {
    return TRMT;    // Se evalua si el TSR se encuentra lleno
}