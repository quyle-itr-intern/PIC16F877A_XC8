/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on April 4, 2022, 6:48 PM
 */
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

# define _XTAL_FREQ 20000000

#include <xc.h>

const unsigned char Code7Seg[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};// anode chung
unsigned int count, nghin, tram, chuc, donvi;

void main() {
       ADCON1 |= 0X07 ; //Khong su dung ADC
       TRISCbits.TRISC0 = 1; // INPUT
       TRISB =0X00;
       PORTB = 0X00;
       TRISD = 0X00;
       PORTD = 0X00;
       
       // tiimer1 hoat dong che do counter, 1:1
       T1CONbits.TMR1CS = 1; // 57/234 COUTER
       T1CONbits.T1CKPS1 = T1CONbits.T1CKPS0 = 0; // TI LE CHIA
       
       TMR1H = TMR1L = 0 ;
       T1CONbits.TMR1ON = 1 ; // ENALBLE
       
       while(1)
       {
               count =  TMR1H;
               count <<=  8 ; 
               count |= TMR1L;
               
               // 1980
               nghin = count/1000;      // 1
               tram = (count%1000)/100;   // 980/100 =    9
               chuc = (count%100)/10;     // 80/ 10 = 8
               donvi = count%10 ;
               
                PORTB = Code7Seg[donvi];
                PORTDbits.RD3 = 0;
                __delay_ms(1);
                PORTDbits.RD3 = 1;
                
                PORTB = Code7Seg[chuc];
                PORTDbits.RD2 = 0;
                __delay_ms(1);
                PORTDbits.RD2 = 1;
                
                PORTB = Code7Seg[tram];
                PORTDbits.RD1 = 0;
                __delay_ms(1);
                PORTDbits.RD1 = 1;
                
                PORTB = Code7Seg[nghin];
                PORTDbits.RD0 = 0;
                __delay_ms(1);
                PORTDbits.RD0 = 1;
       }
}
