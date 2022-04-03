/*
 * File:   I2C.c
 * Author: levan
 *
 * Created on February 10, 2022, 9:24 PM
 */
#include "I2C.h"
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
    // Setting baudrate                  // SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;                       // Baudrate = 308kHz
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
unsigned char I2C_Read(unsigned char ACK)
{
    unsigned char Rvalue;
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
    Rvalue = SSPBUF;
    // if ACK == 1 -> clear SSPIF
    if(ACK == 1)
    {
        PIR1bits.SSPIF = 0;
    }
    return Rvalue;
}
//------------------- I2C_WRITE ---------------------
void I2C_Write(unsigned char Tvalue)
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
}
