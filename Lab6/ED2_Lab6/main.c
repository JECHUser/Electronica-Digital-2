#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

int var = 0;

int main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);       // configuración del reloj
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                            // selección de puerto F para reloj
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);                // configuración de salidas
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);                                       // configuración de entradas
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   // configuración de input pull-up


    while(1){
        // ciclo infinito
        var = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
        if (var == 0) {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x08); // inicia en verde
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x00); // inicia ciclo de verde parpadeante
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x08); // ...
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x00); // ...
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x08); // ...
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x00); // ...
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x08); // ...
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x00); // fin
            SysCtlDelay(10000000);


            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0x02); // inicia color amarillo
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x08);
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0x02); // inicia color rojo
            SysCtlDelay(10000000);
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0x00);
        }
    }
}
