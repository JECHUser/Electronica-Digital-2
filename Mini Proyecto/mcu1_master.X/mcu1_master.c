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
#include "spi_library.h"
#include "spi.h"

//******************************************************************************
// Palabra de configuración
//******************************************************************************

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
uint8_t S1;
uint8_t S2;
uint8_t S3;
char s[16];

int entero;
int decima;

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
void adc_to_5V(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);


    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1) {

        PORTBbits.RB0 = 0;  // selec slave 1
        spiWrite(0);    // escritura de dato fantasma
        S1 = spiRead(); // lectura de valor (potenciometro))
        PORTBbits.RB0 = 1;  // des-selecciona el slave 1
        adc_to_5V();        // mapeo adc --> 5V
        mostrar();

        PORTBbits.RB1 = 0;  // selec slave 2
        spiWrite(0);    // escritura de dato fantasma
        S2 = spiRead(); // lectura de valor (contador 8-bits)
        PORTBbits.RB1 = 1;  // des-selecciona el slave 2
        mostrar();

        PORTBbits.RB2 = 0;  // selec slave 2
        spiWrite(0);    // escritura de dato fantasma
        S3 = spiRead(); // lectura de valor (sensor temp)
        PORTBbits.RB2 = 1;  // des-selecciona el slave 3
        mostrar();

        entero = 0;
        decima = 0;
    }
}

//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    TRISB = 0x00;
    PORTB = 0b00000111;
    TRISD = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    ANSEL = 0;
    ANSELH = 0;
    INTCON = 0b11000000;
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
    sprintf(s, "%1i.%1i", entero, decima);
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String(s);
    sprintf(s, "%3i", S2);
    Lcd_Set_Cursor(2, 7);
    Lcd_Write_String(s);
    sprintf(s, "%3i", S3);
    Lcd_Set_Cursor(2, 13);
    Lcd_Write_String(s);
}

void adc_to_5V(void) {
    
    // lectura de 5V exactos
    if (S1 > 250) {
        entero = 5;
        decima = 0;
    } else {
        // en caso contrario calcula entero y decimal
        while (S1 >= 50) {
            S1 = S1 - 50;   // iteracion para obtener entero
            entero++;
        }
        while (S1 >= 5) {
            S1 = S1 - 5;    // iteracion para obtener decimal
            decima++;
        }
    }
}
