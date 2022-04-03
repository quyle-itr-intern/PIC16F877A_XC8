/*
 * File:   spi.c
 * Author: levan
 *
 * Created on February 28, 2022, 10:24 AM
 */
#include "spi.h"

typedef struct { 
    uint8_t CON; 
    uint8_t STAT;
    uint8_t OPERATION;
} spi_configuration_t;

//con1 == SSPxCON1, stat == SSPxSTAT, add == SSPxADD, operation == Master/Slave
static const spi_configuration_t spi_configuration[] = {   
    {0x20, 0x00, 0}
};

void SPI_Initialize(void)
{
    TRISCbits.TRISC3 = 0;       // Xung clock
    TRISCbits.TRISC4 = 1;       // Receive data
    TRISCbits.TRISC5 = 0;       // TRansfer data
    
    SSPSTATbits.SMP = 0;        // Sampled at middle of data output time
    SSPSTATbits.CKE = 0;        // Transmit occurs on transition from Idle to active clock state
    
    SSPCONbits.WCOL = 0;        // No collision
    SSPCONbits.SSPOV = 0;       // No overflow
    SSPCONbits.SSPEN = 1;       // Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
    SSPCONbits.CKP = 0;         // Idle state for clock is a low level
    SSPCONbits.SSPM3 = 0;       // SPI Master Clock = FOSC/4
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM1 = 1;
    SSPCONbits.SSPM0 = 0;
}

bool SPI_Open(spi_modes_t spi1UniqueConfiguration)
{
    if(!SSPCONbits.SSPEN)
    {
        SSPSTAT = spi_configuration[spi1UniqueConfiguration].STAT;
        SSPCON = spi_configuration[spi1UniqueConfiguration].CON;
        SSPCON2 = 0x00;
        TRISAbits.TRISA5 = spi_configuration[spi1UniqueConfiguration].OPERATION;
        SSPCONbits.SSPEN = 1;
        return true;
    }
    return false;
}

void SPI_Close(void)
{
    SSPCONbits.SSPEN = 0;
}

uint8_t SPI_ExchangeByte(uint8_t data)
{
    SSPBUF = data;
    while(BF == 0);
    return SSPBUF;
}

void SPI_ExchangeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SSPBUF = *data;
        while(BF == 0);
        *data++ = SSPBUF;
    }
}

void SPI_WriteBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SPI_ExchangeByte(*data++);
    }
}

void SPI_ReadBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data++ = SPI_ExchangeByte(0);
    }
}

void SPI_WriteByte(uint8_t byte)
{
    unsigned char xoaBF;
    if(SSPSTATbits.BF == 1)
        xoaBF = SSPBUF;
    SSPBUF = byte;
    while(BF == 0);
}

uint8_t SPI_ReadByte(void)
{
    SSPBUF = 0x00;
    while(BF == 0);
    return SSPBUF;
}
