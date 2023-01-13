/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on April 10, 2022, 7:11 AM
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
#include <pic16f877a.h>
#include <stdint.h>
#define _XTAL_FREQ 20000000
#define TMR2PRESCALE 4
long PWM_Frequency = 5000;

#define BTN_DIR TRISBbits.TRISB0
#define BTN     PORTBbits.RB0

#define IN3_DIR TRISDbits.TRISD0
#define IN4_DIR TRISDbits.TRISD1
#define IN3     PORTDbits.RD0
#define IN4     PORTDbits.RD1

#define Duty_Cycle    1023

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
void Reverse_Motor(int i)
{
    if(i%2 == 0)
    {
        IN3 = 0;
        IN4 = 1;
        //PWM1_Set_Duty(Duty_Cycle);
    }
    else
    {
        IN3 = 1;
        IN4 = 0;
        //PWM1_Set_Duty(Duty_Cycle);
    }
}
void ADC_Config(void)
{
    TRISA = 0xFF;
    ADCON1 = 0b11000000;
    ADCON0 = 0b00000001;
    ADRESH = 0;
    ADRESL = 0;
}
void ADC_Read(void)
{
    unsigned char  u8channel = 7;
    ADCON0 |= u8channel << 3;
    __delay_ms(1);
    GO_nDONE = 1; //Initializes A/D Conversion
    while(GO_nDONE); //Wait for A/D Conversion to complete
    PWM1_Set_Duty((ADRESH<< 8)+ADRESL); //Returns Result
}

void main(void) 
{
    PWM1_Init();
    ADC_Config();
    
    TRISBbits.TRISB0 = 1;
    
    
    BTN_DIR = 1;
    IN3_DIR = 0;
    IN4_DIR = 0;
  
    unsigned char u8count = 0;
    
    IN3 = 0;
    IN4 = 0;
    
    while(1)
    {
        ADC_Read();
        if(0 == BTN)
        {
            u8count++;
            __delay_ms(20);
            while(!BTN);
            Reverse_Motor(u8count);
        }
    }
    return;
}
