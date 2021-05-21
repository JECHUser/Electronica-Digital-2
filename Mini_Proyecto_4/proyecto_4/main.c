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

//****************************************************************************************************************************
// Definition of Variables
//****************************************************************************************************************************

uint32_t Period;
bool avail_P1;
bool avail_P2;
bool avail_P3;
bool avail_P4;
uint8_t parking_space_avail;

//****************************************************************************************************************************
// Function prototypes
//****************************************************************************************************************************

void InitTimer(void);
void InitUART(void);

int main(void)
{

    //****************************************************************************************************************************
    // Oscillator  selection
    //****************************************************************************************************************************

    SysCtlClockSet(
    SYSCTL_SYSDIV_6 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //****************************************************************************************************************************
    // I/O configuration
    //****************************************************************************************************************************

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(
            GPIO_PORTB_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,
    GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //****************************************************************************************************************************
    // Timer configuration
    //****************************************************************************************************************************

    //InitTimer();

    //****************************************************************************************************************************
    // UART COM configuration
    //****************************************************************************************************************************

    InitUART();

    IntMasterEnable();
    // IntEnable(INT_TIMER0A);
    IntEnable(INT_UART0);
    IntEnable(INT_UART2);

    //****************************************************************************************************************************
    // Main Loop
    //****************************************************************************************************************************

    while (1)
    {

        // ***************** First parking *****************
        if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0))
        {
            GPIOPinWrite(GPIO_PORTD_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0x01);
            avail_P1 = 1;

        }
        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0x02);
            avail_P1 = 0;
        }
        // **************** Second parking *****************
        if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))
        {
            GPIOPinWrite(GPIO_PORTD_BASE, (GPIO_PIN_2 | GPIO_PIN_3), 0x04);
            avail_P2 = 1;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, (GPIO_PIN_2 | GPIO_PIN_3), 0x08);
            avail_P2 = 0;
        }
        // **************** Third parking *****************
        if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))
        {
            GPIOPinWrite(GPIO_PORTA_BASE, (GPIO_PIN_3 | GPIO_PIN_4), 0x08);
            avail_P3 = 1;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, (GPIO_PIN_3 | GPIO_PIN_4), 0x10);
            avail_P3 = 0;
        }
        // *************** Fourth parking *****************
        if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3))
        {
            GPIOPinWrite(GPIO_PORTA_BASE, (GPIO_PIN_5 | GPIO_PIN_6), 0x20);
            avail_P4 = 1;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, (GPIO_PIN_5 | GPIO_PIN_6), 0x40);
            avail_P4 = 0;
        }

        parking_space_avail = avail_P1 + avail_P2 + avail_P3 + avail_P4;

        switch(parking_space_avail){
        case 0:
            GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7), 0x3F);
            break;
        case 1:
            GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7), 0x06);
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7), 0x5B);
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7), 0x4F);
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7), 0x66);
            break;
        }

        UARTCharPutNonBlocking(UART0_BASE, 'a');
        //UARTCharPutNonBlocking(UART2_BASE, 'a');

    }
}

//****************************************************************************************************************************
// Interrupt Handler
//****************************************************************************************************************************
void UARTIntHandler(void){
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, ui32Status);
}
//****************************************************************************************************************************
// Functions
//****************************************************************************************************************************

void InitUART(void)
{
    //Enable the UART peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);          // Enable module UART0

    // Set the Rx/Tx pins as UART pins
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2)){};
    // Configure the UART baud rate, data configuration
    UARTConfigSetExpClk(
            UART2_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    // module UART0, clock, baud rate = 115200, (8 data bits | 1 stop bit | none parity)

    // Configure other UART features
    UARTFIFOEnable(UART2_BASE);
    UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);
    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_TX); // Enable interrupt to reception

    UARTEnable(UART2_BASE);

    //Enable the UART peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);          // Enable module UART0

        // Set the Rx/Tx pins as UART pins
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){};
        // Configure the UART baud rate, data configuration
        UARTConfigSetExpClk(
                UART0_BASE, SysCtlClockGet(), 115200,
                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
        // module UART0, clock, baud rate = 115200, (8 data bits | 1 stop bit | none parity)

        // Configure other UART features
        UARTFIFOEnable(UART0_BASE);
        UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);
        UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_TX); // Enable interrupt to reception

        UARTEnable(UART0_BASE);
}

void InitTimer(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Configuración del Timer 0 como temporizador períodico
    Period = (SysCtlClockGet()) / 2;            // Cálculo del período del Timer
    TimerLoadSet(TIMER0_BASE, TIMER_A, Period - 1); // Aplicación del período al temporizador
    IntEnable(INT_TIMER0A);        // Interrupciones habilitadas para el TIMER0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Interrupciones habilitadas por Timeout

    TimerEnable(TIMER0_BASE, TIMER_A);
}
