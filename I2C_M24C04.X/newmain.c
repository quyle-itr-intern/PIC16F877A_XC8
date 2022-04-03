/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 3, 2022, 9:17 PM
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
#include <stdio.h>
#define _XTAL_FREQ 16000000

#define LCD_RS PORTBbits.RB0
#define LCD_EN PORTBbits.RB1

#define LCD_SET_DATA TRISB
#define LCD_DATA PORTB

unsigned char Rvalue = 0;
unsigned char DATA = 10;

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

void LCD_Cmd(unsigned char cmd)
{
	LCD_RS = 0;       // Gui lenh
	LCD_DATA = cmd & 0xF0;
	LCD_EN = 1;
	LCD_EN = 0;
    LCD_DATA = cmd << 4;
	LCD_EN = 1;
	LCD_EN = 0;
	if( cmd <= 0x02)
	{
		__delay_ms(2);
	}
	else
	{
		__delay_ms(1);
	}
}
void LCD_Cmd_Init(unsigned char cmd)
{
	LCD_RS = 0;       // Gui lenh
	LCD_DATA = cmd & 0xF0 ;
	LCD_EN = 1;
	LCD_EN = 0;
    __delay_us(40);
}
void LCD_Init()
{
    LCD_SET_DATA = 0;
    
    /* Only need send 4 bit High */
	LCD_Cmd_Init(0x30);
	__delay_ms(5);
	LCD_Cmd_Init(0x30);
	__delay_ms(1);
	LCD_Cmd_Init(0x30);
    
    /* Only need send 4 bit High, Command first only need send 4 bit High */
    /* Command continue send 4 bit High anh 4 bit Low */
    LCD_Cmd_Init(0x28);
    
    /* Send 4 bit High and 4 bit Low */
	LCD_Cmd(0x28);  // Chon so dong hien thi la 2, font 5x8, 4 bit
	
	/*
	    DBx = DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
		DBx = 0   0   1  [DL] [N] [F] *   * 
	Khi DL=1, LCD 8 bit (bit DB7 -> DB0)
	Khi DL=0, LCD 4 bit (bit DB7 -> DB0), du lieu truyen nhan 2 lan, 4 bit cao gui/nhan truoc, 4 bit cao gui/nhan sau
	N : Thiet lap so hang hien thi. Khi N=0 : hien thi 1 hàng, N=1: hien thi 2 hang.
	F : Thiet lap kieu ki tu. Khi F=0: kieu kí tu 5x8 diem anh, F=1: kieu kí tu 5x10 diem anh.
	*/
	
	LCD_Cmd(0x01);    // Xoa toan bo noi dung LCD
	/*
	LCD_Cmd(0x0D);    // Bat hien thi va tat con tro nhap nhay
    LCD_Cmd(0x0C);    // Bat hien thi va tat con tro 
    LCD_Cmd(0x0F);    // Bat hien thi va bat con tro nhap nhay
    */
    LCD_Cmd(0x0E);    // Bat hien thi va bat con tro
}
void LCD_Char(unsigned char c)    // Current position
{
	LCD_RS = 1;      // Gui du lieu
	LCD_DATA = c | 0x01;
	LCD_EN = 1;
	LCD_EN = 0;
    LCD_DATA = (c << 4) | 0x01;
	LCD_EN = 1;
	LCD_EN = 0;
	__delay_ms(1);
}
void LCD_String(char *str)
{
	unsigned char i = 0;
	while(str[i] != 0)
	{
		LCD_Char(str[i]);
		i++;
	}
}
void LCD_GotoCursor(unsigned char row, unsigned char col)
{
	unsigned char cmd;
	cmd = (row==1?0x80:0xC0) + col - 1;
	LCD_Cmd(cmd);
}
void LCD_GotoCursor_Char(unsigned char row, unsigned char col, char c)  // Xuat ra man hinh LCD 1 ky tu, ngay tai vi tri dong row, cot col
{
	unsigned char cmd;
	cmd = (row==1?0x80:0xC0) + col - 1;
	LCD_Cmd(cmd);
	LCD_Char(c);
}
void LCD_GotoCursor_String(unsigned char row, unsigned char col, char *str)  // Xuat ra man hinh LCD 1 ky tu, ngay tai vi tri dong row, cot col
{
	unsigned char cmd;
	cmd = (row==1?0x80:0xC0) + col - 1;
	LCD_Cmd(cmd);
	LCD_String(str);
}

void I2C_Write24C04(unsigned char StartAddress, unsigned char Data)
{
    I2C_Start();
    I2C_Write(0xA0);
    I2C_Write(StartAddress);
    I2C_Write(Data);
    I2C_Stop();
}
void I2C_Read24C04(unsigned char ReadAddress)
{
    I2C_Start();
    I2C_Write(0xA0);
    I2C_Write(ReadAddress);
    __delay_ms(1);
    
    I2C_Restart();
    I2C_Write(0xA1);
    I2C_Read(&Rvalue, 1);
    I2C_Stop();
}


void main(void) 
{
    TRISDbits.TRISD0 = 1;
    I2C_Init();
    LCD_Init();
    char EE[5];
    I2C_Write24C04(0x00, DATA);
    while(1)
    {
        if(PORTDbits.RD0 == 0)
        {
            DATA++;
            __delay_ms(10);
            I2C_Write24C04(0x00+1, DATA);
            while(!PORTDbits.RD0);
        }
        LCD_GotoCursor(1,1);
        I2C_Read24C04(0x00);
        LCD_String("EEPROM : ");
        sprintf(EE, "%d       ", Rvalue);
        LCD_String(EE);
        __delay_ms(50);
    }
    return;
}
