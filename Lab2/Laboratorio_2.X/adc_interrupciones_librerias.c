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
// definición de variables
uint8_t conversion = 0;
uint8_t MSB;
uint8_t LSB;
int transistor = 1;

//******************************************************************************
// Interrupciones
//******************************************************************************
// inicio de módulo de interrupciones
void __interrupt() isr(void){
    
    // interrupciones debido a las interrupt-on-change del puerto B
    if (INTCONbits.RBIF == 1){
        __delay_ms(2);
        
        // evalua si se presiona el botón de incremento
        if (PORTBbits.RB0 == 1){
            PORTC++;
        }
        // evalua si se presiona el botón de decremento
        if (PORTBbits.RB1 == 1){
            PORTC--;
        }
        // apaga bandera de interrupt-on-change
        INTCONbits.RBIF = 0;
    }
    
    // interrupciones del adc (cuando se carga el valor)
    if (PIR1bits.ADIF == 1){
        // se mueve el valor de la conversion a una variable
        conversion = ADRESH;
        // apaga bandera de que se completó la conversión
        PIR1bits.ADIF = 0;
    }
    
    // interrupcion del timer0
    if (INTCONbits.T0IF == 1){
        // si el timer0 hacer overflow
        // hacer que el valor de transistor oscile entre 0 y 1
        if (transistor == 1){
            transistor = 0;
        }
        else{
            transistor = 1;
        }
        
        // apaga bandera de overflow del timer0
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
        // tiempo de captura de conversión
        __delay_ms(2);
        // enciende inicio de conversion A/D
        ADCON0bits.GO_nDONE = 1;
        // evalua si se completó y la enciende nuevamente para que se repita la conversion
        if (ADCON0bits.GO_nDONE == 0){
            ADCON0bits.GO_nDONE = 1;
        }
        
        // esta parte hace la multiplexacion de los displays
        // cuando el timer0 hace overflow el valor de transistor cambia de 0 a 1 y visceversa
        if (transistor == 1){
            // se toma el nibble (LSB) de la conversion
            LSB = conversion & 0x0F;
            // llamada de libreria para tomar el valor del display
            tabla(LSB);
            // alternancia de los transitores de los display
            PORTEbits.RE0 = 0;
            PORTEbits.RE1 = 1; 
        }
        else{
            // se toma el nibble (MSB) de la conversion
            MSB = (conversion & 0xF0)>>5;
            // llamada de libreria para tomar el valor del display
            tabla(MSB);
            // alternancia de los transitores de los display
            PORTEbits.RE0 = 1;
            
            PORTEbits.RE1 = 0; 
        }  
        
        // si se cumple la condicion enciende la alarma
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
    OPTION_REG = 0b01000111;
    TMR0 = 254;
}

//******************************************************************************
// Funciones
//******************************************************************************
