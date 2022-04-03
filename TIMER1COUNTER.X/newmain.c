/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 22, 2022, 11:59 AM
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
const unsigned char Number[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void main(void) {
    ADCON1 |= 0x07;
    unsigned int counter;
    //TRISCbits.TRISC0 = 1;
    TRISD = 0x00;
    PORTD = 0x00;
    
    TRISB = 0x00;
    PORTB = 0x00;
    
    PIE1bits.TMR1IE = 0;
    // Prescale 1:1
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 0;
    // External clock
    T1CONbits.TMR1CS = 1;
    // Clear counter timer1
    TMR1H = 0;
    TMR1L = 0;
    // Enable timer1
    T1CONbits.TMR1ON = 1;
    
    while(1)
    {
        counter = TMR1H;
        counter <<= 8;
        counter |= TMR1L;
        if(counter >= 10000)
        {
            counter = 0;
            T1CONbits.TMR1ON = 0;
            TMR1H = TMR1L = 0;
            T1CONbits.TMR1ON = 1;
        }
        
        // counter = 2001
        PORTB = Number[counter/1000];
        PORTDbits.RD0 = 0;
        __delay_ms(1);
        PORTDbits.RD0 = 1;
        __delay_ms(1);
        
        PORTB = Number[counter/100%10];
        PORTDbits.RD1 = 0;
        __delay_ms(1);
        PORTDbits.RD1 = 1;
        __delay_ms(1);
        
        PORTB = Number[counter/10%10];
        PORTDbits.RD2 = 0;
        __delay_ms(1);
        PORTDbits.RD2 = 1;
        __delay_ms(1);
        
        PORTB = Number[counter%10];
        PORTDbits.RD3 = 0;
        __delay_ms(1);
        PORTDbits.RD3 = 1;
        __delay_ms(1);
    }

    return;
}
