/*
 * File:   main.c
 * Author: levan
 *
 * Created on January 5, 2022, 10:21 PM
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

void main(void) {
    char i, PressedFlag1 = 0;
    char PressedFlag2 = 0;
    char PressedFlag3 = 0;
    
    ADCON1 |= 0x07;
    TRISA = 0x00;
    PORTA = 0x3F;
    __delay_ms(50);
    
    while(1)
    {
        if(PORTAbits.RA0 == 0)
        {
            __delay_ms(20);
            if(PressedFlag1 == 0)
            {
                for(i=0;i<10;i++)
                {
                    PORTA = 0x37;    
                    __delay_ms(50);
                    PORTA = 0x2F; 
                    __delay_ms(50);
                    PORTA = 0x1F; 
                    __delay_ms(50);
                    PORTA = 0x3F;
                    __delay_ms(50);
                }
                PressedFlag1 = 1;
            }
        }
        else
        {
            PressedFlag1 = 0;
        }
        
        if(PORTAbits.RA1 == 0)
        {
            __delay_ms(20);
            if(PressedFlag2 == 0)
            {
                for(i=0;i<10;i++)
                {
                    PORTA = 0x07;
                    __delay_ms(50);
                    PORTA = 0x3F;
                    __delay_ms(50);
                    PORTA = 0x07;
                    __delay_ms(50);
                    PORTA = 0x3F;
                    __delay_ms(50);
                }
                PressedFlag2 = 1;
            }
        }
        else
        {
            PressedFlag2 = 0;
        }
        
        if(PORTAbits.RA2 == 0)
        {
            __delay_ms(20);
            if(PressedFlag3 == 0)
            {
                for(i=0;i<10;i++)
                {
                    PORTA = 0x37;    
                    __delay_ms(100);
                    PORTA = 0x3F; 
                    __delay_ms(100);
                    PORTA = 0x0F; 
                    __delay_ms(100);
                    PORTA = 0x3F;
                    __delay_ms(100);
                    PORTA = 0x1F; 
                    __delay_ms(100);
                    PORTA = 0x3F;
                }
                PressedFlag3 = 1;
            }
        }
        else
        {
            PressedFlag3 = 0;
        }
    } 
    return;
}
