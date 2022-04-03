/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 4, 2022, 4:02 PM
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
#include <pic.h>
#include <stdio.h>
#define _XTAL_FREQ 16000000

unsigned char u8LCD_Buff[8];//bo nho dem luu lai toan bo
unsigned char u8LcdTmp;
#define I2C_LCD_ADDR1 0x4E
#define I2C_LCD_ADDR2 0x42
#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3
unsigned char LCD = 1;
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

void I2C_WriteLCD(unsigned char u8Address, unsigned char *pu8Data, unsigned char u8Length)
{
	unsigned char i;

	I2C_Start();
	I2C_Write(u8Address);
	for (i = 0; i < u8Length; ++i) {
		I2C_Write(pu8Data[i]);
	}
	I2C_Stop();
}

void I2C_LCD_FlushVal(void)
{
	unsigned char i;
	
	for (i = 0; i < 8; ++i) {
		u8LcdTmp >>= 1;
		if(u8LCD_Buff[i]) {
			u8LcdTmp |= 0x80;
		}
	}
    if(LCD == 1)
    {
        I2C_WriteLCD(I2C_LCD_ADDR1, &u8LcdTmp, 1);
    }
    else
    {
        I2C_WriteLCD(I2C_LCD_ADDR2, &u8LcdTmp, 1);
    } 
}

void I2C_LCD_WriteData(unsigned char u8Data)
{
    if(u8Data & 0x80) {
		u8LCD_Buff[LCD_D7] = 1;
	} else {
		u8LCD_Buff[LCD_D7] = 0;
	}
	if(u8Data & 0x40) {
		u8LCD_Buff[LCD_D6] = 1;
	} else {
		u8LCD_Buff[LCD_D6] = 0;
	}
	if(u8Data & 0x20) {
		u8LCD_Buff[LCD_D5] = 1;
	} else {
		u8LCD_Buff[LCD_D5] = 0;
	}
	if(u8Data & 0x10) {
		u8LCD_Buff[LCD_D4] = 1;
	} else {
		u8LCD_Buff[LCD_D4] = 0;
	}
    
    u8LCD_Buff[LCD_EN] = 1;
	I2C_LCD_FlushVal();
    
    u8LCD_Buff[LCD_EN] = 0;
	I2C_LCD_FlushVal();
    __delay_ms(1);
}

void I2C_LCD_WriteCmd(unsigned char u8Cmd)
{
    u8LCD_Buff[LCD_RS] = 0;
	I2C_LCD_FlushVal();
	
	u8LCD_Buff[LCD_RW] = 0;
	I2C_LCD_FlushVal();

    I2C_LCD_WriteData(u8Cmd);
	I2C_LCD_WriteData(u8Cmd << 4);
}

void I2C_LCD_Init(void)
{
    I2C_Init();
    __delay_ms(5);
    unsigned char i;
    
    for (i = 0; i < 8; ++i) {
		u8LCD_Buff[i] = 0;
	}
    I2C_LCD_FlushVal();
    
    u8LCD_Buff[LCD_RS] = 0;
	I2C_LCD_FlushVal();
	
	u8LCD_Buff[LCD_RW] = 0;
	I2C_LCD_FlushVal();
    
    I2C_LCD_WriteData(0x30);
    __delay_ms(5);
    I2C_LCD_WriteData(0x30);
    __delay_ms(1);
    I2C_LCD_WriteData(0x30);
    __delay_ms(1);
    
    I2C_LCD_WriteData(0x28);
    __delay_ms(1);
    
    I2C_LCD_WriteCmd(0x28);
    
    I2C_LCD_WriteCmd(0x01);
    
    I2C_LCD_WriteCmd(0x0E);
}

void I2C_LCD_Write_Char(char c)
{
	u8LCD_Buff[LCD_RS] = 1;
	I2C_LCD_FlushVal();
	u8LCD_Buff[LCD_RW] = 0;
	I2C_LCD_FlushVal();
    
	I2C_LCD_WriteData(c);
	I2C_LCD_WriteData(c << 4);
}

void I2C_LCD_Write_String(char *str)
{
	while(*str)
    {
        I2C_LCD_Write_Char(*str++);
    }
}

void I2C_LCD_GotoCursor(unsigned char u8row, unsigned char u8col)
{
    unsigned char cmd;
	cmd = (u8row==1?0x80:0xC0) + u8col - 1;
    I2C_LCD_WriteCmd(cmd);
}

void I2C_LCD_CursorChar(unsigned char u8row, unsigned char u8col, char c)
{
    unsigned char cmd;
	cmd = (u8row==1?0x80:0xC0) + u8col - 1;
    I2C_LCD_WriteCmd(cmd);
    
    I2C_LCD_Write_Char(c);
}

void I2C_LCD_CursorString(unsigned char u8row, unsigned char u8col, char *str)
{
    unsigned char cmd;
	cmd = (u8row==1?0x80:0xC0) + u8col - 1;
    I2C_LCD_WriteCmd(cmd);
    
    I2C_LCD_Write_String(str);
}

void I2C_LCD_BackLight(unsigned char u8BackLight)
{
	
	if(u8BackLight) {
		u8LCD_Buff[LCD_BL] = 1;
	} else {
		u8LCD_Buff[LCD_BL] = 0;
	}
	I2C_LCD_FlushVal();
}

void main(void) 
{
    unsigned int counter1 = 0, counter2 = 0;
    char dem1[5], dem2[3];
    
    TRISDbits.TRISD0 = 1;
    LCD = 1;
    I2C_LCD_Init();
    I2C_LCD_BackLight(1);
    
    
    
    LCD = 1;
    I2C_LCD_CursorString(1,1, "COUNTER1 : ");
    sprintf(dem1, "%d", counter1);
    I2C_LCD_CursorString(1,12,dem1);
    I2C_LCD_CursorString(2,1,"------(^.^)-----");
    
    while(1)
    {
        if(PORTDbits.RD0 == 0)
        {
            __delay_ms(10);
            counter1++;
            LCD = 1;
            sprintf(dem1, "%d", counter1);
            I2C_LCD_CursorString(1,12,dem1);
        }
        __delay_ms(25);
    }
    return;
}
