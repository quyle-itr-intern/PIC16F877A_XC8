/* 
 * File:   spi.h
 * Author: levan
 *
 * Created on February 28, 2022, 10:24 AM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* SPI interfaces */
typedef enum { 
    SPI_DEFAULT
} spi_modes_t;

void SPI_Initialize(void);
bool SPI_Open(spi_modes_t spi1UniqueConfiguration);
void SPI_Close(void);
uint8_t SPI_ExchangeByte(uint8_t data);
void SPI_ExchangeBlock(void *block, size_t blockSize);
void SPI_WriteBlock(void *block, size_t blockSize);
void SPI_ReadBlock(void *block, size_t blockSize);
void SPI_WriteByte(uint8_t byte);
uint8_t SPI_ReadByte(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

