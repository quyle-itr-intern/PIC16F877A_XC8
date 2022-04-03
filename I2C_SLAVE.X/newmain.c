/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 28, 2022, 1:43 PM
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
unsigned char Code7Seg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0xFF, 0x6F};
unsigned char Rvalue = 0;
unsigned char Tvalue = 0;
//----------------- I2C_INIT --------------
void I2C_SInit()
{
    // Setting interrupt
    INTCONbits.GIE = 1;         // Interrupt global
    INTCONbits.PEIE = 1;        // Interrupt peripheral
    PIE1bits.SSPIE = 1;         // Interrupt SSPIE
    PIR1bits.SSPIF = 0;         // Clear SSPIF
    // Setting port I/O
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    // Setting register SSPCON
    SSPCON = 0x36;              // SSPCON : 00110110
    SSPSTAT = 0x40;             // CKE = 1
    // Setting address
    SSPADD = 0x10;
    // 
    SSPCON2bits.SEN = 1;
}
void I2C_SWrite(unsigned char Tvalue)
{
    // Write SSPBUF
    SSPBUF = Tvalue;
    // Set CKP
    SSPCONbits.CKP = 1;
    // Clear SSPIF
    PIR1bits.SSPIF = 0;
    while(PIR1bits.SSPIF == 0);
}
void I2C_SRead(unsigned char *Rvalue)
{
    if(SSPSTATbits.D_A == 0)           // Check byte previous
    {
        *Rvalue = SSPBUF;          // Clear SSPBUF
        *Rvalue = 0;
    }
    PIR1bits.SSPIF = 0;            // Clear Flag interrupt
    SSPCONbits.SSPOV = 0;          // Clear Flag Overflow
    while(PIR1bits.SSPIF == 0);    // Wait complete
    SSPCONbits.CKP = 0;
    *Rvalue = SSPBUF;
    SSPCONbits.CKP = 1; 
}
void Show(Rvalue)
{
  PORTDbits.RD0 = 1;
  PORTDbits.RD1 = 1;
  
  PORTDbits.RD3 = 0;
  PORTB = Code7Seg[Rvalue%10];
  __delay_ms(1);
  PORTDbits.RD3 = 1;
  
  PORTDbits.RD2 = 0;
  PORTB = Code7Seg[Rvalue/10];
  __delay_ms(1);
  PORTDbits.RD2 = 1;
  
}
void main(void) 
{
    I2C_SInit();
    ADCON1 = 0x07;   // Off ADC
    TRISA = 0xFF;
    TRISB = 0;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    
    while(1)
    {
        Tvalue = PORTA;
        Show(Rvalue);
        __delay_ms(30);
    }
    return;
}

void __interrupt() isr1()
{
    if(PIR1bits.SSPIF)
    {
        if(SSPSTATbits.R_W == 0)   // if RW = 0 is read
        {
            I2C_SRead(&Rvalue);
        }
        if(SSPSTATbits.R_W == 1)   // if RW = 1 is write
        {
            I2C_SWrite(Tvalue);
        }
        PIR1bits.SSPIF = 0;
    }
}