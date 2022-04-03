/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 29, 2022, 2:32 PM
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000

unsigned int T = 12500, Ton, Toff;
unsigned char Ton_H, Ton_L, Toff_H, Toff_L;

void PWM_50Hz_Init()
{
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 1; 
    // Internal clock
    T1CONbits.TMR1CS = 0;
    // Prescale 1:8
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TMR1IE = 1;
    
    Ton = T/2;
    Toff = T - Ton;
    
    Ton_H = (65536 - Ton) >> 8;
    Ton_L = (65536 - Ton) & 0x00FF; 
    
    Toff_H = (65536 - Toff) >> 8;
    Toff_L = (65536 - Toff) & 0x00FF; 
    
    TMR1H = Ton_H;
    TMR1L = Ton_L; 
}

void PWM_Start()
{
    // Start timer1
    T1CONbits.TMR1ON = 1;
}
void PWM_Stop()
{
    // Start timer1
    T1CONbits.TMR1ON = 0;
}

void PWM_Set_Duty(unsigned char duty)
{
    if(duty == 0)
    {
        PORTBbits.RB0 = 0;
        INTCONbits.PEIE = 0;
    }
    else if(duty == 100)
    {
        PORTBbits.RB0 = 1;
        INTCONbits.PEIE = 0;
    }
    else
    {
        Ton = ((unsigned long)T)*duty/100;
        Toff = T - Ton;

        Ton_H = (65536 - Ton) >> 8;
        Ton_L = (65536 - Ton) & 0x00FF; 

        Toff_H = (65536 - Toff) >> 8;
        Toff_L = (65536 - Toff) & 0x00FF; 
        
        INTCONbits.PEIE = 1;
    }
}
void PWM_Servo(unsigned char angle)
{
    unsigned int duty = (angle/180.0)*625+625; // practice : (angle/180.0)*1275+300, theory : (angle/180.0)*625+625
   
    Ton = duty;
    Toff = T - Ton;

    Ton_H = (65536 - Ton) >> 8;
    Ton_L = (65536 - Ton) & 0x00FF; 

    Toff_H = (65536 - Toff) >> 8;
    Toff_L = (65536 - Toff) & 0x00FF; 

}
void main(void) 
{
    PWM_50Hz_Init();
    PWM_Servo(0);
    PWM_Start();
    //unsigned char i;
    while(1)
    {
        PWM_Servo(0);
        __delay_ms(500);
//        PWM_Servo(90);
//        __delay_ms(500);
//        PWM_Servo(180);
//        __delay_ms(500);
    }
    return;
}

void __interrupt() isr1()
{
    if(PIR1bits.TMR1IF)
    {
        PORTBbits.RB0 = !PORTBbits.RB0;
        if(PORTBbits.RB0 == 0)
        {
            TMR1H = Toff_H;
            TMR1L = Toff_L;  
        }
        else
        {
            TMR1H = Ton_H;
            TMR1L = Ton_L; 
        }
        PIR1bits.TMR1IF = 0;
    }
}
