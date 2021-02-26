/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
  
#ifndef I2C_LIB
#define	I2C_LIB

#include <xc.h> // include processor files - each processor file is guarded.  

void I2C_Master_Init(const unsigned long c);
void I2C_Master_Wait();
void I2C_Master_Start(void);
void I2C_RepeatedStart(void);
void I2C_Master_Stop();
void I2C_Master_Write(unsigned data);
unsigned short I2C_Master_Read(unsigned short a);

#endif	/* I2C_LIB */

