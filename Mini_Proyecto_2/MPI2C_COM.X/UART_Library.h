/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
 
#ifndef UART_LIB
#define	UART_LIB

#include <xc.h> // include processor files - each processor file is guarded.  

char UART_Init(const long int baudrate);
char UART_DATA_Read();
void UART_DATA_Write(char data);
char UART_DATA_Ready();
char UART_TX_EMPTY();
void UART_Write_Text(char *text);

#endif	/* UART_LIB */

