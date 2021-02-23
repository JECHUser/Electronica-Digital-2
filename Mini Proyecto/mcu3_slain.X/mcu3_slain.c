/*
 * File:   mcu3_slain.c
 * Author: HP
 *
 * Created on 12 de febrero de 2021, 02:07 PM
 */

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
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

#define _XTAL_FREQ 8000000

//******************************************************************************
// Variables
//******************************************************************************
int var = 0;
uint8_t cont = 0;
uint8_t var_temp;
//******************************************************************************
// Interrupciones
//******************************************************************************

void __interrupt() isr(void) {

    if (INTCONbits.RBIF == 1) {
        if (PORTBbits.RB0 == 1) {
            cont++;
        }
        if (PORTBbits.RB1 == 1) {
            cont--;
        }
        INTCONbits.RBIF = 0;
    }
}
//******************************************************************************
// Prototipo de funciones
//******************************************************************************
void setup(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {
    setup();

    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1) {
        PORTD = cont;
        SSPBUF = cont;
        while(!SSPSTATbits.BF){}
        var_temp = SSPBUF;
    }
}

//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    TRISA = 0b00100000;
    PORTA = 0x00;
    TRISC = 0b00011000;
    PORTC = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;
    TRISB = 0x03;
    PORTB = 0;
    ANSEL = 0;
    ANSELH = 0;
    INTCON = 0b11001000;
    IOCB = 0x03;
    SSPSTAT = 0b00000000;
    SSPCON = 0b00110100;
}
//******************************************************************************
// Funciones
//******************************************************************************
