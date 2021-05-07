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

uint32_t Period;

int main(void){

    // Selección de oscilador, usando PLL configurado a 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN);

    // Habilitación del reloj para el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Habilitación del reloj para el temporizador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //****************************************************************************************************************************
    // Configuración I/O
    //****************************************************************************************************************************

    // Configuración de salidas del puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Configuración de entradas del puerto F
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

    //****************************************************************************************************************************
    // Configuración del temporizador
    //****************************************************************************************************************************

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);    // Configuración del Timer 0 como temporizador períodico
    Period = (SysCtlClockGet()) / 2;                    // Cálculo del período del Timer
    TimerLoadSet(TIMER0_BASE, TIMER_A, Period - 1);     // Aplicación del período al temporizador
    IntEnable(INT_TIMER0A);                             // Interrupciones habilitadas para el TIMER0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);    // Interrupciones habilitadas por Timeout
    TimerEnable(TIMER0_BASE, TIMER_A);                  // Timer habilitado

    IntMasterEnable();                                  // Interrupciones Globales habilitadas

    //****************************************************************************************************************************
    // Loop Principal
    //****************************************************************************************************************************
    while(1){

    }
}


//****************************************************************************************************************************
// Handler de las interrupciones
//****************************************************************************************************************************

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);     // Se limpia la interrupción del Timer

    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
    }
    else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);
    }

}
























