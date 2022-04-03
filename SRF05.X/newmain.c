/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 23, 2022, 4:49 PM
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
#include "stdlib.h"
#include "stdio.h"
#define _XTAL_FREQ 20000000

#define LCD_SET_RS TRISBbits.TRISB6
#define LCD_SET_EN TRISBbits.TRISB7
#define LCD_SET_DATA TRISC
#define LCD_RS PORTBbits.RB6
#define LCD_EN PORTBbits.RB7
#define LCD_DATA PORTC

#define SRF05_SET_TRIGGER TRISBbits.TRISB0
#define SRF05_SET_ECHO TRISBbits.TRISB1

#define SRF05_TRIGGER PORTBbits.RB0
#define SRF05_ECHO PORTBbits.RB1

void LCD_Cmd(unsigned char cmd)
{
	LCD_RS = 0;       // Gui lenh
	LCD_DATA = cmd;
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
	LCD_DATA = cmd;
	LCD_EN = 1;
	LCD_EN = 0;
    __delay_us(40);
}
void LCD_Init()
{
    LCD_SET_RS = 0;
    LCD_SET_EN = 0;
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
	LCD_DATA = c;
	LCD_EN = 1;
	LCD_EN = 0;
    LCD_DATA = c << 4;
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

void main(void) 
{
    LCD_Init();
    
    ADCON1 |= 0x07;
    unsigned int time = 0;
    unsigned int distance = 0;
    SRF05_SET_TRIGGER = 0;
    SRF05_SET_ECHO = 1;
 
    T1CONbits.TMR1CS = 0;
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 0;
    
    LCD_GotoCursor_String(1,1,"Distance: XXXcm");
    LCD_GotoCursor_String(2,1, "LVQ");
    
    while(1)
    {
        SRF05_TRIGGER = 1;
        __delay_ms(15);
        SRF05_TRIGGER = 0;
        
        while(SRF05_ECHO == 0);
        T1CONbits.TMR1ON = 1;
        while(SRF05_ECHO == 1);
        T1CONbits.TMR1ON = 0;
        
        time = TMR1H;
        time <<= 8;
        time |= TMR1L;
        
        if(time > 125 && time < 31250)
        {
            distance = (time*0.8/58);
            LCD_GotoCursor_Char(1,11, distance/100+0x30);
            LCD_Char(distance/10%10+0x30);
            LCD_Char(distance%10+0x30);
        }
        else
        {
            LCD_GotoCursor_String(1,11, "@_@");
        }
        TMR1H = TMR1L = 0;
        __delay_ms(500);
    }
    return;
}

