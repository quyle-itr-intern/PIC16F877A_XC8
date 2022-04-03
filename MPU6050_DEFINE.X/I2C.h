/* 
 * File:   I2C.h
 * Author: levan
 *
 * Created on February 10, 2022, 9:25 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"
#define I2C_BaudRate      400000

void I2C_Init();
void I2C_Idle();
void I2C_Start();
void I2C_Restart();
void I2C_Stop();
unsigned char I2C_Read(unsigned char ACK);
void I2C_Write(unsigned char Tvalue);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

