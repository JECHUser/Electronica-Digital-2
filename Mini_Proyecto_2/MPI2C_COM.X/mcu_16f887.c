/*
 * File:   mcu_16f887.c
 * Author: Jonatan Choc�n
 *
 * Created on 26 de febrero de 2021, 10:38 AM
 */

//******************************************************************************
// Importaci�n de librer�as
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include "I2C_Library.h"
#include "UART_Library.h"

//******************************************************************************
// Palabra de configuraci�n
//******************************************************************************

// PIC16F887 Configuration Bit Settings
// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

#define _XTAL_FREQ 4000000

//******************************************************************************
// Variables
//******************************************************************************

char sensor = 1;
char temp;
char address = 20;

//******************************************************************************
// Interrupciones
//******************************************************************************

//******************************************************************************
// Prototipo de funciones
//******************************************************************************
void setup(void);
char Read_sensor(void);
char Read_esp32(void);
void Write_esp32(void);

//******************************************************************************
// Ciclo principal
//******************************************************************************

void main(void) {

    setup();
    I2C_Master_Init(100000);
    UART_Init(9600);

    //**************************************************************************
    // Loop principal
    //**************************************************************************

    while (1) {
        //sensor = Read_sensor();
        Write_esp32();
    }
}

//******************************************************************************
// Configuraci�n
//******************************************************************************

void setup(void) {
    TRISD = 0b00000000;
    PORTD = 0;
    TRISC = 0b11011000;
    PORTC = 0;
}

//******************************************************************************
// Funciones
//******************************************************************************

char Read_esp32(void){
    if(UART_DATA_Ready()){
        PORTD = UART_DATA_Read();
        __delay_ms(100);
    }
}

void Write_esp32(void){
    UART_Write_Text("Encender la led");
    //UART_Write_Text(sensor);
    __delay_ms(100);
}

char Read_sensor(void) {
    I2C_Master_Start();
    I2C_Master_Write((address * 2) + 1);
    sensor = I2C_Master_Read(1);
    I2C_Master_Stop();
    __delay_ms(100);
    I2C_Master_Start();
    I2C_Master_Write((address * 2) + 0);
    I2C_Master_Write(0x01);
    I2C_Master_Stop();
    __delay_ms(100);
}