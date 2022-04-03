/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 29, 2022, 1:10 PM
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic.h>
#include <pic16f877a.h>
#define _XTAL_FREQ 20000000
#define TMR2PRESCALE 4
long PWM_Frequency = 5000;

void PWM1_Init()
{
    PR2 = _XTAL_FREQ/(4*PWM_Frequency*TMR2PRESCALE) - 1; 
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    if(TMR2PRESCALE == 1)
    {
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 0;
    }
    if(TMR2PRESCALE == 4)
    {
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 1;
    }
    if(TMR2PRESCALE == 16)
    {
        T2CONbits.T2CKPS1 = 1;
        T2CONbits.T2CKPS0 = 1;
    }
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;
    TRISCbits.TRISC2 = 0;
    
}
void PWM1_Set_Duty(unsigned int duty)
{
    if(duty < 1024)
    {
        duty = ((float)duty/1023)*(_XTAL_FREQ/(PWM_Frequency*TMR2PRESCALE));
        CCP1X = duty & 1;
        CCP1Y = duty & 2;
        CCPR1L = duty >> 2;
    }
}

void main(void) 
{
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    PWM1_Init();
    int i;
    PORTDbits.RD6 = 1;
    PORTDbits.RD7 = 0;
    PWM1_Set_Duty(1023);
    while(1)
    {
    }
    return;
}
