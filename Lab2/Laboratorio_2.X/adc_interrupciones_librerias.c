/*
 * File:   juego_de_carreras.c
 * Author: JONATAN CHOCÓN
 *
 * Created on 22 de enero de 2021, 08:38 AM
 */

//******************************************************************************
// Importación de librerías
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include "tabla_7seg.h"

//******************************************************************************
// Palabra de configuración
//******************************************************************************

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

#define _XTAL_FREQ 8000000

//******************************************************************************
// Variables
//******************************************************************************

int i;
int cont = 0;
uint8_t conversion = 0;
int MSB;
int LSB;
int transistor = 1;

//******************************************************************************
// Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.RBIF == 1){
        __delay_ms(2);
        if (PORTBbits.RB0 == 1){
            PORTC++;
        }
        if (PORTBbits.RB1 == 1){
            PORTC--;
        }
        INTCONbits.RBIF = 0;
    }
    
    if (PIR1bits.ADIF == 1){
        conversion = ADRESH;
        PIR1bits.ADIF = 0;
    }
    
    if (INTCONbits.T0IF == 1){
        if (transistor == 1){
            transistor = 0;
        }
        else{
            transistor = 1;
        }
        TMR0 = 254;
        INTCONbits.T0IF = 0;
    }
}

//******************************************************************************
// Prototipo de funciones
//******************************************************************************

void setup(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void){
    setup();

    //**************************************************************************
    // Loop principal
    //**************************************************************************
    
    while (1){ 
        __delay_ms(1);
        ADCON0bits.GO_nDONE = 1;
        if (ADCON0bits.GO_nDONE == 0){
            ADCON0bits.GO_nDONE = 1;
        }
        
        if (transistor == 1){
            LSB = conversion & 0x0F;
            tabla(LSB);
            PORTEbits.RE0 = 1;
            PORTEbits.RE1 = 0; 
        }
        else{
            MSB = conversion>>4;
            tabla(MSB);
            PORTEbits.RE0 = 0;
            PORTEbits.RE1 = 1;  
        }  
        
        if (conversion > PORTC){
            PORTEbits.RE2 = 1;
        }
        else{
            PORTEbits.RE2 = 0;
        }
    }
}
//******************************************************************************
// Configuración
//******************************************************************************
void setup(void){
    TRISA = 0x01;
    PORTA = 0;
    TRISB = 0x03;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    TRISE = 0;
    PORTE = 0x01;
    ANSEL = 0x01;
    ANSELH = 0;
    IOCB = 0x03;
    INTCON = 0b11101000;
    ADCON0 = 0b00000011;
    ADCON1 = 0b00000000;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    OPTION_REG = 0b01010111;
    TMR0 = 254;
}

//******************************************************************************
// Funciones
//******************************************************************************
