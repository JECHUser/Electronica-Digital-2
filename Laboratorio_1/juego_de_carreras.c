/*
 * File:   juego_de_carreras.c
 * Author: HP
 *
 * Created on 22 de enero de 2021, 08:38 AM
 */

//******************************************************************************
// Importación de librerías
//******************************************************************************

#include <xc.h>

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

//******************************************************************************
// Prototipo de funciones
//******************************************************************************

void setup(void);
void secuencia_semaforo(void);
void secuencia_aumento_j1(void);
void secuencia_aumento_j2(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {
    setup();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1){
        if (PORTBbits.RB0 == 1){
            secuencia_semaforo();
        }
        if (PORTBbits.RB1 == 1){
            while (PORTBbits.RB1 == 1){
                __delay_ms(5);
                if (PORTBbits.RB1 == 0){
                    secuencia_aumento_j1();
                }
            }
        }
        if (PORTBbits.RB2 == 1){
            while (PORTBbits.RB2 == 1){
                __delay_ms(5);
                if (PORTBbits.RB2 == 0){
                    secuencia_aumento_j2();
                }
            }
        }
    }
}

//******************************************************************************
// Configuración
//******************************************************************************
void setup(void){
    TRISB = 0x03;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISE = 0;
    PORTE = 0;
    TRISD = 0;
    PORTD = 0;
    ANSEL = 0;
    ANSELH = 0;
}

//******************************************************************************
// Funciones
//******************************************************************************

void secuencia_semaforo(void){
    PORTEbits.RE0 = 1;
    __delay_ms(500);
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 1;
    __delay_ms(500);
    PORTEbits.RE1 = 0;
    PORTEbits.RE2 = 1;
    __delay_ms(500);
    PORTEbits.RE2 = 0;
}

void secuencia_aumento_j1(void){
    PORTC = PORTC + 2;
}

void secuencia_aumento_j2(void){
    PORTD = PORTD + 2;
}

void gana_j1(void){
    
}
