/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 27, 2022, 3:09 PM
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000
//---------------- SPI_INIT -----------
void SPI_Init()
{
    // Setting I/O
    TRISBbits.TRISB0 = 0;       // Slave 1
    TRISBbits.TRISB1 = 0;       // Slave 2
    PORTBbits.RB0 = 1;       
    PORTBbits.RB1 = 1;       
    
    TRISCbits.TRISC2 = 1;       // Response (-)
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
//---------------- SPI_READ -----------
void SPI_Read(unsigned char SLAVE, unsigned char *Rvalue)
{
    switch(SLAVE)
    {
        case 1: PORTBbits.RB0 = 0;     // Select Slave1
        break;
        case 2: PORTBbits.RB1 = 0;     // Select Slave2
        break;
    }
    
    while(PORTCbits.RC2 == 1);         // Wait response from Slave1
    SSPBUF = 0x00;
    while(!PIR1bits.SSPIF);
    while(!SSPSTATbits.BF);
    *Rvalue = SSPBUF;
    while(PORTCbits.RC2 == 0);         // Wait response from Slave1
    
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
}
unsigned char SPI_Read_Return(unsigned char SLAVE)
{
    unsigned char Rvalue;
    switch(SLAVE)
    {
        case 1: PORTBbits.RB0 = 0;     // Select Slave1
        break;
        case 2: PORTBbits.RB1 = 0;     // Select Slave2
        break;
    }
    
    while(PORTCbits.RC2 == 1);         // Wait response from Slave1
    SSPBUF = 0x00;
    while(!PIR1bits.SSPIF);
    while(!SSPSTATbits.BF);
    Rvalue = SSPBUF;
    while(PORTCbits.RC2 == 0);         // Wait response from Slave1
    
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
    return Rvalue;
}
//---------------- SPI_READ -----------
void SPI_Write(unsigned char SLAVE, unsigned char Tvalue)
{
    unsigned char xoaBF;
    switch(SLAVE)
    {
        case 1: PORTBbits.RB0 = 0;     // Select Slave1
        break;
        case 2: PORTBbits.RB1 = 0;     // Select Slave2
        break;
    }
    
    while(PORTCbits.RC2 == 1);         // Wait response from Slave1
    if(SSPSTATbits.BF == 1)
        xoaBF = SSPBUF;
    SSPBUF = Tvalue;
    while(!PIR1bits.SSPIF);
    while(PORTCbits.RC2 == 0);         // Wait response from Slave1
    
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
}
//---------------- SPI_DUAL -----------
void SPI_Dual(unsigned char SLAVE, unsigned char Tvalue, unsigned char Rvalue)
{
    switch(SLAVE)
    {
        case 1: PORTBbits.RB0 = 0;     // Select Slave1
        break;
        case 2: PORTBbits.RB1 = 0;     // Select Slave2
        break;
    }
    
    while(PORTCbits.RC2 == 1);         // Wait response from Slave1
    SSPBUF = Tvalue;
    while(!PIR1bits.SSPIF);
    Rvalue = SSPBUF;
    while(PORTCbits.RC2 == 0);         // Wait response from Slave1
    
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
}

void main(void) 
{
    ADCON1 = 0x07;
    TRISA = 0;
    PORTA = 0;
    TRISD = 0xFF;
    SPI_Init();
    while(1)
    {
        if(PORTDbits.RD0 == 0)
        {
            __delay_ms(20);
            SPI_Write(1, 0xAA);
            SPI_Write(2, 0xFF);
        }
        if(PORTDbits.RD1 == 0)
        {
            __delay_ms(20);
            PORTA = SPI_Read_Return(1);
        } 
        if(PORTDbits.RD2 == 0)
        {
            __delay_ms(20);
            PORTA = SPI_Read_Return(2);
        }
    }
    return;
}
