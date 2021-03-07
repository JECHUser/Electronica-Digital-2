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

char UART_Init(const long int baudrate) {
    unsigned int x;
    x = (_XTAL_FREQ - baudrate * 64) / (baudrate * 64);
    if (x > 255) {
        x = (_XTAL_FREQ - baudrate * 16) / (baudrate * 16);
        BRGH = 1;
    }
    if (x < 256) {
        SPBRG = x;
        SYNC = 0; // Enable the Asynchronous serial port
        SPEN = 1; // clearing SYNC and setting SPEN
        TX9 = 0;
        TRISC7 = 1;
        TRISC6 = 0;
        CREN = 1; // Receive enable
        TXEN = 1; // Transmit enable
        return 1;
    }
    return 0;
}

char UART_DATA_Read() {
    while (!PIR1bits.RCIF); // Is UART receive buffer full?
    return RCREG;
}

void UART_DATA_Write(char data) {
    TXREG = data;
    while (!TXSTAbits.TRMT); // is TSR full?
}

char UART_DATA_Ready() {
    return RCIF;
}

char UART_TX_EMPTY() {
    return TRMT; // Is TSR empty or full?
}

void UART_Write_Text(unsigned char *text) {
    while(*text !=0x00){
        UART_DATA_Write(*text);
        text++;
    }
}