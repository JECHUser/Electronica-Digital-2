/*
 * File:   mcu1_master.c
 * Author: JONATAN CHOCÓN
 *
 * Created on 12 de febrero de 2021, 10:26 AM
 */


#define _XTAL_FREQ 8000000

#define RS RE0
#define EN RE1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//******************************************************************************
// Importación de librerías
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include "lcd_library.h"

//******************************************************************************
// Palabra de configuración
//******************************************************************************

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 8000000

//******************************************************************************
// Variables
//******************************************************************************
unsigned int a;
uint8_t S1 = 1;
uint8_t S2;
uint8_t S3;
char s[20];
//******************************************************************************
// Interrupciones
//******************************************************************************

void __interrupt() isr(void) {
}

//******************************************************************************
// Prototipo de funciones
//******************************************************************************

void setup(void);
void mostrar(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init();
    Lcd_Clear();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1) {
        mostrar();
        PORTBbits.RB0 = 0; // selec slave 1
        __delay_ms(1);
        if (SSPSTATbits.BF == 1) {
            while (!SSPSTATbits.BF) {
            }
            S1 = SSPBUF;
        }
        __delay_ms(1);
        PORTBbits.RB0 = 1;
    }
}

//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    TRISA = 0x00;
    PORTA = 0x00;
    TRISB = 0x07;
    PORTB = 0x00; 
    TRISC = 0b00010000;
    PORTC = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;
    ANSEL = 0;
    ANSELH = 0;
    SSPSTAT = 0b00000000;
    SSPCON = 0b00110010;
}
//******************************************************************************
// Funciones
//******************************************************************************

void mostrar(void) {
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("S1:");
    Lcd_Set_Cursor(1, 7);
    Lcd_Write_String("S2:");
    Lcd_Set_Cursor(1, 13);
    Lcd_Write_String("S3:");
    sprintf(s, "%d", S1);
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String(s);
    sprintf(s, "%d", S1);
    Lcd_Set_Cursor(2, 7);
    Lcd_Write_String(s);
    sprintf(s, "%d", S1);
    Lcd_Set_Cursor(2, 13);
    Lcd_Write_String(s);
}