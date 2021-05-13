#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"

uint32_t Period;

int led_select = 0;
uint8_t character = 0;


//**************************************************************************************************************
// Prototipos de Función
//**************************************************************************************************************
void InitUART(void);
void InitTimer(void);

int main(void){

    // Selección de oscilador, usando PLL configurado a 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_6|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //****************************************************************************************************************************
    // Configuración I/O
    //****************************************************************************************************************************

    // Configuración de salidas del puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Configuración de salidas del puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);

    // interrupciones globales
    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    IntEnable(INT_UART0);

    // funciones de inicialización de timer y comunicación uart
    InitTimer();
    InitUART();

    //****************************************************************************************************************************
    // Loop Principal
    //****************************************************************************************************************************
    while(1){}
}


//****************************************************************************************************************************
// Handler de las interrupciones
//****************************************************************************************************************************

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A);     // Se limpia la interrupción del Timer
    if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3)){
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
    }
    else{
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x08);
    }
    // ingresaron caracter 'r' - led rojo
    if(led_select == 1){
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);
        }
    }

    // ingresaron caracter 'g'- led verde
    if (led_select == 2){
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08);
        }
    }


    // ingresaron caracter 'b' - led azul
    if (led_select == 3){
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);
        }
    }
}

void UARTIntHandler(void){
    uint32_t ui32Status;
    int16_t data;
    ui32Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ui32Status);

    while(UARTCharsAvail(UART0_BASE)){
        data = UARTCharGet(UART0_BASE);
        character = data;
        if (character == 114){
            led_select = 1; // ingresaron caracter 'r' - led rojo
        }
        if (character == 103){
            led_select = 2; // ingresaron caracter 'g' - led verde
        }
        if (character == 98){
            led_select = 3; // ingresaron caracter 'b' - led azul
        }
    }
}

//****************************************************************************************************************************
// Funciones
//****************************************************************************************************************************

void InitUART(void){
    //Enable the UART peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                // Enable module UART0

    // Set the Rx/Tx pins as UART pins
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    // Configure the UART baud rate, data configuration
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
                    // module UART0, clock, baud rate = 115200, (8 data bits | 1 stop bit | none parity)

    // Configure other UART features
    UARTFIFOEnable(UART0_BASE);
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);
    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_TX);                 // Enable interrupt to reception

    UARTEnable(UART0_BASE);


}

void InitTimer(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    // Configuración del Timer 0 como temporizador períodico
    Period = (SysCtlClockGet()) / 2;                    // Cálculo del período del Timer
    TimerLoadSet(TIMER0_BASE, TIMER_A, Period - 1);     // Aplicación del período al temporizador
    IntEnable(INT_TIMER0A);                             // Interrupciones habilitadas para el TIMER0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);    // Interrupciones habilitadas por Timeout

    TimerEnable(TIMER0_BASE, TIMER_A);
}






