/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef _SPI_
#define	_SPI_

#include <xc.h> // include processor files - each processor file is guarded.  

void SPI_Slave_Init(void);
void SPI_Master_Init(void);
char SPI_read(void);
void SPI_write(char data);
static void wait();

#endif	/* XC_HEADER_TEMPLATE_H */

