 /*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 22, 2022, 6:37 PM
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

void Delay_1ms()
{
    // C1 : Read value after check
    // Load value TMR1H:TMR1L = 0
    // Wait until TMR1H:TMR1L = 5000
    // C2 : Check Flag bit
    // Load value TMR1H:TMR1L = 65535 - 5000 = 60535
    // Check Flag bit
    TMR1H = 60535 >> 8;     // 60564
    TMR1L = 60535 & 0x00FF;
    // Start timer1
    T1CONbits.TMR1ON = 1;
    // Wait Flag bit set
    while(!PIR1bits.TMR1IF);
    // Clear Flag bit 
    PIR1bits.TMR1IF = 0;
    // Stop timer1
    T1CONbits.TMR1ON = 0;
}

void main(void) {
    // Internal clock
    T1CONbits.TMR1CS = 0;
    // Prescale 1:1
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 0;
    
    TRISB = 0;
    PORTB = 0;
     
    while(1)
    {
        Delay_1ms();        // T = 2ms , f = 0.5kHz
        PORTB = ~PORTB;
    }
    return;
}
