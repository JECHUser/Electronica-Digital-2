/*
 * File:   mcu2_slain.c
 * Author: HP
 *
 * Created on 12 de febrero de 2021, 02:06 PM
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
#include "adc_library.h"
#include "spi.h"

#define _XTAL_FREQ 8000000
//******************************************************************************
// Variables
//******************************************************************************
uint8_t conv_adc;
uint8_t temp;

//******************************************************************************
// Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    
    // comprobando transmision/recepcion
    if(PIR1bits.SSPIF == 1){
        PIR1bits.SSPIF = 0;
        temp = spiRead();
        spiWrite(conv_adc);
    }
    
    // comprobando si terminó la conversión
    if (PIR1bits.ADIF == 1){
        conv_adc = ADRESH;  // lectura de la conversion
        PIR1bits.ADIF = 0;  // off bandera conversión ADC
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
    setup();        // llamada configuraciones generales
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    adc_setup();    // llamada configuraciones ADC
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1){
        adc_convert();  // conversion
    }
}

//******************************************************************************
// Configuración
//******************************************************************************
void setup(void){
    TRISA = 0x21;
    PORTA = 0x00;
    TRISB = 0x00;
    PORTB = 0x00;
    ANSEL = 0x01;
    ANSELH = 0;
    ADCON0 = 0b000000011;
    ADCON1 = 0b000000000;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
}
//******************************************************************************
// Funciones
//******************************************************************************
