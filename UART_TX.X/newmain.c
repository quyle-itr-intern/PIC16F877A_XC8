/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 24, 2022, 9:54 PM
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
#include <stdlib.h>
#define _XTAL_FREQ 20000000
#define SET_TX TRISCbits.TRISC6
#define SET_RX TRISCbits.TRISC7
#define SizeData 50

char RX_Buffer[SizeData];
char rx_index = 0;

void IO_Init();
void UART_Init();
void UART_Transmit(char c);
void UART_Transmit_Text(char *str);
void UART_Receive();
void HandleData(char *RX);
void ClearRX_Buffer(void);

void IO_Init()
{
    SET_TX = 0;
    SET_RX = 1;
    TRISB = 0;
    PORTB = 0x00;
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
void UART_Receive()
{
    if(RCIF)   // Wait until RSR Register complete
    {
        char ReceivedData;
        ReceivedData = RCREG;
        if(ReceivedData != '\n')
        {
            RX_Buffer[rx_index++] = RCREG;
        }
        else
        {
            HandleData(RX_Buffer);
            ClearRX_Buffer();
        }
    }
}
void HandleData(char *RX)
{
    UART_Transmit_Text(RX);
}
void ClearRX_Buffer(void)
{
	char i;
	for(i = 0; i < SizeData; i++)
	{
		RX_Buffer[i] = 0;		
	}
	rx_index = 0;
}
void main(void) 
{   
    IO_Init();
    UART_Init();
    unsigned int i = 0;
    char text[20];
    while(1)
    {
        //UART_Receive();
        sprintf(text, "Hello World %d\r\n", i++);
        UART_Transmit_Text(text);
        __delay_ms(1000);
    }
}
void __interrupt() isr1() 
{
    if(RCIE)
    {
        if(RCIF)   // Wait until RSR Register complete
        {
            char ReceivedData;
            ReceivedData = RCREG;
            if(ReceivedData != '\n')
            {
                RX_Buffer[rx_index++] = RCREG;
            }
            else
            {
                HandleData(RX_Buffer);
                ClearRX_Buffer();
            }
        }
    }
}
