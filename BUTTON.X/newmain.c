/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 21, 2022, 4:51 PM
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
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    
    PORTCbits.RC0 = 0; 
    PORTCbits.RC1 = 0; 
            
    while(1)
    {
        if(PORTBbits.RB0 == 0)
        {
            __delay_ms(20);
            PORTCbits.RC0 = !PORTCbits.RC0; 
            while(!PORTBbits.RB0);
        }
        if(PORTBbits.RB1 == 0)
        {
            __delay_ms(20);
            PORTCbits.RC1 = !PORTCbits.RC1; 
            while(!PORTBbits.RB1);
        }
        if(PORTBbits.RB2 == 0)
        {
            __delay_ms(20);
            PORTCbits.RC1 = 1; 
            PORTCbits.RC0 = 1; 
            while(!PORTBbits.RB2);
        }
//        if(PORTBbits.RB0 == 1 && PORTBbits.RB1 == 1 && PORTBbits.RB2 == 1)
//        {
//            PORTCbits.RC1 = 0; 
//            PORTCbits.RC0 = 0;
//        }
//        if(PORTBbits.RB3 == 0)
//        {
//            __delay_ms(20);
//            PORTCbits.RC1 = 1; 
//            PORTCbits.RC0 = 1;
//            __delay_ms(200);
//            PORTCbits.RC1 = 0; 
//            PORTCbits.RC0 = 0;
//            __delay_ms(200);
//            PORTCbits.RC1 = 1; 
//            PORTCbits.RC0 = 1;
//            __delay_ms(200);
//            PORTCbits.RC1 = 0; 
//            PORTCbits.RC0 = 0;
//            while(!PORTBbits.RB3);
//        }
//        if(PORTBbits.RB4 == 0)
//        {
//            __delay_ms(20);
//            PORTCbits.RC1 = 0; 
//            PORTCbits.RC0 = 0; 
//            while(!PORTBbits.RB4);
//        }
    }              
    return;
}
