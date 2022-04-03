/*
 * File:   main.c
 * Author: levan
 *
 * Created on January 6, 2022, 4:06 PM
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

unsigned char Number[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

void main(void) {
    
    unsigned int counter = 0;
    
    TRISB = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    
    while(1)
    {
        for(int i = 0; i < 50; i++)
        {
            PORTB = Number[counter/1000];
            PORTCbits.RC0 = 1;
            __delay_ms(1);
            PORTCbits.RC0 = 0;

            PORTB = Number[counter/100%10];
            PORTCbits.RC1 = 1;
            __delay_ms(1);
            PORTCbits.RC1 = 0;

            PORTB = Number[counter/10%10];
            PORTCbits.RC2 = 1;
            __delay_ms(1);
            PORTCbits.RC2 = 0;

            PORTB = Number[counter%10];
            PORTCbits.RC3 = 1;
            __delay_ms(1);
            PORTCbits.RC3 = 0;
        }
        counter++;
        counter %= 10000;
    }
    return;
}
