/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 4, 2022, 9:37 PM
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
#define _XTAL_FREQ 20000000
#define LCD_RS PORTBbits.RB0
#define LCD_EN PORTBbits.RB1

#define LCD_SET_DATA TRISB
#define LCD_DATA PORTB

#define SDA_IO TRISDbits.TRISD1

#define I2C_SCL PORTDbits.RD0
#define I2C_SDA PORTDbits.RD1
#define READ_SDA PORTDbits.RD1

//--------------------------------------------------
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
	N : Thiet lap so hang hien thi. Khi N=0 : hien thi 1 h�ng, N=1: hien thi 2 hang.
	F : Thiet lap kieu ki tu. Khi F=0: kieu k� tu 5x8 diem anh, F=1: kieu k� tu 5x10 diem anh.
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
	LCD_RS = 1;          // Gui du lieu
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
	while(*str)
	{
		LCD_Char(*str++);
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
//--------------------------------------------------

void I2C_Init()
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    I2C_SDA = 1;
    I2C_SCL = 1;
}
void I2C_Start()
{
    SDA_IO = 0;
    I2C_SDA = 1;
    I2C_SCL = 1;
    __delay_us(4);
    I2C_SDA = 0;
    __delay_us(4);
    I2C_SCL = 0; 
}
void I2C_Stop()
{
    SDA_IO = 0;
    I2C_SDA = 0;
    I2C_SCL = 0;
    __delay_us(4);
    I2C_SCL = 1;
    __delay_us(4);
    I2C_SDA = 1;
}
void I2C_Write(unsigned char Tvalue)
{
    unsigned char i;
    SDA_IO = 0;
    I2C_SCL = 0;
    for(i=0;i<8;i++){
		I2C_SDA = (Tvalue&0x80) >> 7;
		Tvalue <<= 1;
		I2C_SCL = 1;
		__delay_us(4);
		I2C_SCL = 0;
	}  
}
unsigned char I2C_WaitACK()
{
    unsigned char time = 0;
    SDA_IO = 1;
    I2C_SDA = 1;
    __delay_us(1);
	I2C_SCL = 1;
    __delay_us(1);
	while(READ_SDA){
		time++;
		if(time>250) {I2C_Stop(); return 1;}
	}
	I2C_SCL = 0;
	return 0; 
}
void I2C_ACK()
{
    I2C_SCL = 0;
    SDA_IO = 0;
	I2C_SDA = 0;
	__delay_us(2);
	I2C_SCL = 1;
	__delay_us(2);
	I2C_SCL = 0;
}
void I2C_NACK()
{
    I2C_SCL = 0;
    SDA_IO = 0;
	I2C_SDA = 1;
	__delay_us(2);
	I2C_SCL = 1;
	__delay_us(2);
	I2C_SCL = 0;
}
unsigned char I2C_Read(unsigned char ACK)
{
    unsigned char i = 0, Rvalue = 0;
	SDA_IO = 1;
	for(i=0;i<8;i++)
    {
		I2C_SCL=0;
		__delay_us(2);
		I2C_SCL=1;
		__delay_us(2);
		Rvalue<<=1;
		if(READ_SDA) Rvalue++;
	}
	if(!ACK)
    {
		I2C_ACK();
	}
    else
    {
		I2C_NACK();	
	}
    return Rvalue;
}

void I2C_WriteMulti(unsigned char addr, unsigned char reg, unsigned char len, unsigned char buff[])
{
    unsigned char i;
    I2C_Start();
    
    I2C_Write(addr|0);
    if(I2C_WaitACK()) I2C_Stop();

    I2C_Write(reg);
    if(I2C_WaitACK()) I2C_Stop();
    
    for(i = 0; i < len; i++)
    {
        I2C_Write(buff[i]);
        if(I2C_WaitACK()) I2C_Stop();
    }
    
    I2C_Stop();
    __delay_ms(2);
}

void I2C_ReadMulti(unsigned char addr, unsigned char reg, unsigned char len, unsigned char Rvalue[])
{
    unsigned char i;
    I2C_Start();
    I2C_Write(addr);
    if(I2C_WaitACK()) I2C_Stop();
    I2C_Write(reg);
    if(I2C_WaitACK()) I2C_Stop();
    
    I2C_Start();
    I2C_Write(addr|1);
    if(I2C_WaitACK()) I2C_Stop();
    
    while(i!=len)
    {
        if(i == (len-1)) Rvalue[i] = I2C_Read(1);
        else Rvalue[i] = I2C_Read(0);
        i++;
    }      
    I2C_Stop();
    __delay_ms(2);
}

void I2C_ReadMultiPointer(unsigned char addr, unsigned char reg, unsigned char len, unsigned char* Rvalue)
{
    I2C_Start();
    I2C_Write(addr);
    if(I2C_WaitACK()) I2C_Stop();
    I2C_Write(reg);
    if(I2C_WaitACK()) I2C_Stop();
    
    I2C_Start();
    I2C_Write(addr|1);
    if(I2C_WaitACK()) I2C_Stop();
    
    while(1)
    {
        if(len == 1) {*Rvalue = I2C_Read(1); break;}
        else *Rvalue = I2C_Read(0);
        Rvalue++;
        len--;
    }      
    I2C_Stop();
    __delay_ms(2);
}

void main(void) 
{
    unsigned char Data[5] = "HELLO";
    unsigned char Rvalue[5],i;

    LCD_Init();
    I2C_Init();
    
    I2C_WriteMulti(0xA0, 0x00, 5, Data);
    
    __delay_ms(1000);
    
    I2C_ReadMultiPointer(0xA0, 0x00, 5, Rvalue);
    
    for(i=0;i<5;i++)
    {
        LCD_Char(Rvalue[i]);
    }
    
    while(1)
    {
        
    }
    return;
}
