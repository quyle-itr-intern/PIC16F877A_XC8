/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 27, 2022, 3:10 PM
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
#include <pic16f877a.h>
#define _XTAL_FREQ 20000000
unsigned char Tvalue = 0;
unsigned char Rvalue = 0;

void SPI_Init()
{
    // Setting I/O
    TRISBbits.TRISB1 = 0;
    PORTBbits.RB1 = 1;
    TRISCbits.TRISC2 = 0;       // Hoi tiep
    
    TRISCbits.TRISC3 = 1;       // Xung clock
    TRISCbits.TRISC4 = 1;       // Receive data
    TRISCbits.TRISC5 = 0;       // TRansfer data
    ADCON1 = 0x07;
    TRISAbits.TRISA5 = 1;       // SS input
    TRISBbits.TRISB0 = 1;       // Interrupt RB0
    
    SSPSTATbits.SMP = 0;        //  Sampled at middle of data output time
    SSPSTATbits.CKE = 0;        // Transmit occurs on transition from Idle to active clock state
    
    SSPCONbits.WCOL = 0;        // No collision
    SSPCONbits.SSPOV = 0;       // No overflow
    SSPCONbits.SSPEN = 1;       // Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
    SSPCONbits.CKP = 0;         // Idle state for clock is a low level
    SSPCONbits.SSPM3 = 0;       // SPI Slave NOT_SS pin control enabled
    SSPCONbits.SSPM2 = 1;
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM0 = 0;
    
    INTCONbits.GIE = 1;          // Enable Interrupt global
    INTCONbits.INTE = 1;         // Interrupt RB0
    INTCONbits.PEIE = 1;         // Interrupt peripheral 
    PIE1bits.SSPIE = 1;          // Interrupt SSP
    OPTION_REGbits.INTEDG = 0;
    PIR1bits.SSPIF = 0;
}

void main(void) 
{
    SPI_Init();
    PORTCbits.RC2 = 0;
    TRISD = 0x00;
    PORTD = 0x00;
    while(1)
    {
        //PORTD = Rvalue;
    }
    return;
}
void __interrupt() isr1()
{
    if(INTCONbits.INTF == 1)
    {
        SSPBUF = 0xAA;
        INTCONbits.INTF = 0;    // Clear interrupt
        PORTCbits.RC2 = 1;       // Response (+)
        PORTBbits.RB1 = 0;
    }
    if(PIR1bits.SSPIF == 1)
    {
        Rvalue = SSPBUF;
        PORTD = Rvalue;
        SSPCONbits.SSPOV = 0;
        SSPCONbits.WCOL = 0;
        
        SSPCONbits.SSPEN = 0;
        PORTCbits.RC5 = 0;
        SSPCONbits.SSPEN = 1;
        PORTCbits.RC2 = 0;       // Response (-)
        PIR1bits.SSPIF = 0;
        PORTBbits.RB1 = 1;
    }
}