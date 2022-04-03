/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 13, 2022, 8:10 AM
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

void Timer0_Config(void)
{
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
}

void Delay1ms(void)
{
    TMR0 = 100;
    while(!INTCONbits.TMR0IF);
    INTCONbits.TMR0IF = 0;
}
void Delayms(unsigned int delay)
{
    while(delay)
    {
        Delay1ms();
        delay--;
    }
}

void main(void) 
{
    Timer0_Config();
    TRISC = 0;
    PORTC = 0;
    while(1)
    {
        PORTC = ~PORTC;
        Delayms(10);
    }
    return;
}
