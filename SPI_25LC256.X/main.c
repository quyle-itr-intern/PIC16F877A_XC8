/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 7, 2022, 1:02 PM
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements
#include "main.h"
#include "spi.h"

#define CS_SET TRISCbits.TRISC6
#define CS PORTCbits.RC6
/*
//---------------- SPI_INIT -----------
void SPI_Init()
{
    // Setting I/O    
    CS_SET = 0;
    TRISCbits.TRISC3 = 0;       // Xung clock
    TRISCbits.TRISC4 = 1;       // Receive data
    TRISCbits.TRISC5 = 0;       // TRansfer data
    
    SSPSTATbits.SMP = 0;        //  Sampled at middle of data output time
    SSPSTATbits.CKE = 0;        // Transmit occurs on transition from Idle to active clock state
    
    SSPCONbits.WCOL = 0;        // No collision
    SSPCONbits.SSPOV = 0;       // No overflow
    SSPCONbits.SSPEN = 1;       // Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
    SSPCONbits.CKP = 0;         // Idle state for clock is a low level
    SSPCONbits.SSPM3 = 0;       // SPI Master Clock = FOSC/64
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM1 = 1;
    SSPCONbits.SSPM0 = 0;
}
//---------------- SPI_WRITE -----------
void SPI_Write(unsigned char Tvalue)
{
    unsigned char xoaBF;
    if(SSPSTATbits.BF == 1)
        xoaBF = SSPBUF;
    SSPBUF = Tvalue;
    while(BF == 0);
}
//---------------- SPI_READ -----------
unsigned char SPI_Read()
{
    SSPBUF = 0x00;
    while(BF == 0);
    return SSPBUF;
}
unsigned char EEPROM25LCXXX_CheckBusy()
{
    unsigned char SR;
    CS = 0;
    SPI_Write(0x05);
    SR = SPI_Read();
    CS = 1;
    return SR&0x01;
}
void EEPROM25LCXXX_Write(unsigned int addr, unsigned char wrdata)
{
    while(EEPROM25LCXXX_CheckBusy());
    CS = 0;
    SPI_Write(0x06);
    CS = 1;
    
    CS = 0;
    SPI_Write(0x02);
    SPI_Write(addr>>8);
    SPI_Write(addr&0x00FF);
    SPI_Write(wrdata);
    CS = 1;
}
unsigned char EEPROM25LCXXX_Read(unsigned int addr)
{
    unsigned char data;
    while(EEPROM25LCXXX_CheckBusy());
    CS = 0;
    SPI_Write(0x03);
    SPI_Write(addr>>8);
    SPI_Write(addr&0x00FF);
    data = SPI_Read();
    CS = 1;
    return data;
}
void EEPROM25LCXXX_ReadMulti(unsigned int addr, unsigned char* buff, unsigned int len)
{
    unsigned int i;
    while(EEPROM25LCXXX_CheckBusy());
    CS = 0;
    SPI_Write(0x03);
    SPI_Write(addr>>8);
    SPI_Write(addr&0x00FF);
    for(i=0;i<len;i++)
    {
        buff[i] = SPI_Read(); // *(buff+i)
    }
    CS = 1;
}
*/

unsigned char EEPROM25LCXXX_CheckBusy()
{
    unsigned char SR;
    CS = 0;
    SPI_WriteByte(0x05);
    SR = SPI_ReadByte();
    CS = 1;
    return SR&0x01;
}

void EEPROM25LCXXX_Write(unsigned int addr, unsigned char wrdata)
{
    while(EEPROM25LCXXX_CheckBusy());
    CS = 0;
    SPI_WriteByte(0x06);
    CS = 1;
    
    CS = 0;
    SPI_WriteByte(0x02);
    SPI_WriteByte(addr>>8);
    SPI_WriteByte(addr&0x00FF);
    SPI_WriteByte(wrdata);
    CS = 1;
}

void EEPROM25LCXXX_ReadMulti(unsigned int addr, unsigned char* buff, unsigned int len)
{
    while(EEPROM25LCXXX_CheckBusy());
    CS = 0;
    SPI_WriteByte(0x03);
    SPI_WriteByte(addr>>8);
    SPI_WriteByte(addr&0x00FF);
    SPI_ReadBlock(buff, len);
    CS = 1;
}

void main(void) 
{
    unsigned char i, array[10];
    TRISB = 0;
    PORTB = 0;
    CS_SET = 0;
    //SPI_Init();
    SPI_Initialize();
    
    for(i=0;i<100;i++)
    {
        EEPROM25LCXXX_Write(i,i);
    }
    
    EEPROM25LCXXX_ReadMulti(0,array,10);
   
    for(i=0;i<10;i++)
    {
        PORTB = array[i];
        __delay_ms(500);
    }
    
    while(1)
    {
        for(i=0;i<10;i++)
        {
            PORTB = array[i];
            __delay_ms(500);
        }
        
    }
    return;
}
