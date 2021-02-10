/*
 * File:   lab_3.c
 * Author: JONATAN CHOCÓN
 *
 * Created on 5 de febrero de 2021, 10:22 AM
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
#include "lcd.h"
#include "adc.h"

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

#define _XTAL_FREQ 8000000

//******************************************************************************
// Variables
//******************************************************************************

uint8_t conversion;
uint8_t pot1_value;
uint8_t pot2_value;
uint8_t channel_1;
uint8_t channel_2;
uint8_t format_result = 0;

//******************************************************************************
// Interrupciones
//******************************************************************************

void __interrupt() isr(void) {
    if (PIR1bits.ADIF == 1) {
        conversion = ADRESH;
        PIR1bits.ADIF = 0;
    }
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
    adc_setup(format_result);
    Lcd_Init();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while (1) {
        mostrar();
        channel_1 = 0;
        adc_convert(channel_1);
        pot1_value = conversion;
        channel_2 = 1;
        adc_convert(channel_2);
        pot2_value = conversion;
    }
}
//******************************************************************************
// Configuración
//******************************************************************************

void setup(void) {
    TRISA = 0x0F;
    PORTA = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;
    ANSEL = 0x0F;
    ANSELH = 0;
}

//******************************************************************************
// Funciones
//******************************************************************************

void mostrar(void){
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("T1");
    Lcd_Set_Cursor(1,5);
    Lcd_Write_String("T2");
    Lcd_Set_Cursor(1,10);
    Lcd_Write_String("P");
    __delay_ms(2000);
}