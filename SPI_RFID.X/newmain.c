/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 7, 2022, 7:52 PM
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
#include <pic.h>
#include <stdio.h>
#include <string.h>
#include "rfid.h"
#define _XTAL_FREQ 20000000

#define SET_TX TRISCbits.TRISC6
#define SET_RX TRISCbits.TRISC7
#define SizeData 50

char RX_Buffer[SizeData];
char rx_index = 0;

unsigned char status;
unsigned char str[16]; // Max_LEN = 16
unsigned char serNum[5];

void IO_Init();
void UART_Init();
void UART_Transmit(char c);
void UART_Transmit_Text(char *str);

void IO_Init()
{
    SET_TX = 0;
    SET_RX = 1;
}
void UART_Init()
{
    // TX Transmit
    SPBRG = 32;  // Set (Asynchronous) Baud Rate 9600baud
    SPEN = 1;    // Serial port enabled (configures RC7/RX/DT and RC6/TX/CK pins as serial port pins)
    TXEN = 1;    // Transmit enabled
    CREN = 1;    // Enables continuous receive
    RCIE = 1;    // Enables interrupts
    INTCONbits.GIE = 1;      // Enables interrupts global
    INTCONbits.PEIE = 1;     // Enables interrupts peripheral
}
void UART_Transmit(char c)
{
    while(!TRMT); // Wait until TSR Register empty
    TXREG = c;
}
void UART_Transmit_Text(char *str)
{
    unsigned char i=0;
    while(str[i] != 0)
    {
        UART_Transmit(str[i]);
        i++;
    }
}

void main(void) 
{
    char tx[50];
    IO_Init();
    UART_Init();
    SPI_Init();
    MFRC522_Init();
    while(1)
    {
        UART_Transmit_Text("Waiting your Card! \r\n");
				
		status = MFRC522_Request(PICC_REQIDL, str);	
		if (status == MI_OK)
		{
			sprintf(tx, "Find out a card: %x, %x\r\n",str[0],str[1]);
			UART_Transmit_Text(tx);
		}

		status = MFRC522_AntiColl(str);
		memcpy(serNum, str, 5);
		if (status == MI_OK)
		{
			sprintf(tx, "Your card's number are: %x, %x, %x, %x, %x \r\n",serNum[0], serNum[1], serNum[2], serNum[3],serNum[4]);
		    UART_Transmit_Text(tx);
		}
        __delay_ms(1000);
    }
    return;
}
