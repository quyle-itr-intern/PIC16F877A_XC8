/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 28, 2022, 11:03 AM
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
//------------------- I2C_INIT ---------------------
void I2C_Init()
{
    // Setting interrupt, Master no care
    // Setting port I/O
    TRISCbits.TRISC3 = 1;                // SCL : RC3
    TRISCbits.TRISC4 = 1;                // SDA : RC4
    // Setting register control          // SSPCON : 00101000
    SSPCON = 0x28;                       // SSPM<3:0> = 1000 - Master mode 
    SSPCON2 = 0x00;                      
    SSPSTATbits.SMP = 1;                 // SMP = 1 Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)
    // Setting baudrate
    SSPADD = 0x0D;                       // Baudrate = 308kHz
}
//------------------- I2C_IDLE ---------------------
void I2C_Idle()
{
    while(SSPSTATbits.R_W | SSPCON2bits.SEN);
    while(SSPSTATbits.R_W | SSPCON2bits.RSEN);
    while(SSPSTATbits.R_W | SSPCON2bits.PEN);
    while(SSPSTATbits.R_W | SSPCON2bits.RCEN);
    while(SSPSTATbits.R_W | SSPCON2bits.ACKEN);
}
//------------------- I2C_START ---------------------
void I2C_Start()
{
    I2C_Idle();
    SSPCON2bits.SEN = 1;                  // Create condition start
    while(SSPCON2bits.SEN);          // Wait end start
}
//------------------- I2C_RESTART ---------------------
void I2C_Restart()
{
    I2C_Idle();
    SSPCON2bits.RSEN = 1;                 // Create condition restart
    while(SSPCON2bits.RSEN);         // Wait end restart
}
//------------------- I2C_STOP ---------------------
void I2C_Stop()
{
    I2C_Idle(); 
    SSPCON2bits.PEN = 1;                  // Create condition stop
    while(SSPCON2bits.PEN);          // Wait end stop
}
//------------------- I2C_READ ---------------------
void I2C_Read(unsigned char *Rvalue, unsigned char ACK)
{
    // Check idle
    I2C_Idle();
    // Clear Flag overflow + SSPIF
    SSPCONbits.SSPOV = 0;
    PIR1bits.SSPIF = 0;
    // Start read
    SSPCON2bits.RCEN = 1;
    // Wait bit 8
    while(SSPCON2bits.RCEN);
    // Check ACK
    SSPCON2bits.ACKDT = ACK;
    // Transfer ACK
    SSPCON2bits.ACKEN = 1;
    // Clear SSPIF
    PIR1bits.SSPIF = 0;
    // Wait ACK end check SSPIF
    while(PIR1bits.SSPIF == 0);
    // Read SSPBUF
    *Rvalue = SSPBUF;
    // if ACK == 1 -> clear SSPIF
    if(ACK == 1)
    {
        PIR1bits.SSPIF = 0;
    }
}
//------------------- I2C_WRITE ---------------------
void I2C_Write(unsigned char Tvalue, unsigned char *status)
{
    // Check idle
    I2C_Idle();
    // Clear Flag xung dot
    SSPCONbits.WCOL = 0;
    // Write SSPBUF
    SSPBUF = Tvalue;
    // Clear SSPIF
    PIR1bits.SSPIF = 0;
    // Wait until transfer complete check bit RW or SSPIF
    while(PIR1bits.SSPIF == 0);
    // if ACK == 1 clear SSPIF
    if(SSPCON2bits.ACKSTAT == 1)
    {
        PIR1bits.SSPIF = 0;
    }
    *status = !SSPCON2bits.ACKSTAT;
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
    unsigned char k = 0, Status = 0;
    I2C_Init();
    TRISB = 0;
    PORTB = 0;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD7 = 1;
    
    while(1)
    {
        Show(Rvalue);
        if(PORTDbits.RD7 == 0)
        {
            __delay_ms(20);
            if(PORTDbits.RD7 == 0)
            {
                k = 1;
            }
        }
        if(PORTDbits.RD7 == 1 && k == 1)
        {
            I2C_Start();
            I2C_Write(0x10, &Status);
            I2C_Write(Tvalue, &Status);
            __delay_ms(1);
            I2C_Restart();
            I2C_Write(0x11, &Status);
            I2C_Read(&Rvalue, 1);
            I2C_Stop();
            Tvalue++;
            k = 0;
        }
        __delay_ms(10);
    }
    return;
}
