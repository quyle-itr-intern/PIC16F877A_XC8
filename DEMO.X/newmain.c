/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on April 9, 2022, 7:51 AM
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
#include <pic.h>
#define _XTAL_FREQ 20000000
#define Delay 50

void Tuantu(void)
{
    for(int i = 0; i < 3; i++)
    {
        PORTB = (0x01 << i);
        __delay_ms(Delay);
    }
    PORTB = 0;
    for(int i = 0; i < 3; i++)
    {
        PORTC = (0x01 << i);
        __delay_ms(Delay);
    }
    PORTC = 0;
    for(int i = 0; i < 2; i++)
    {
        PORTD = (0x01 << i);
        __delay_ms(Delay);
    }
    PORTD = 0;
}

void ADC_Config(void)
{
    TRISA = 0xFF;
    ADCON1 = 0b11000000;
    ADCON0 = 0b00000001;
    ADRESH = 0;
    ADRESL = 0;
}

void Show_Led(int adc)
{
    PORTB = adc;
    PORTC = adc >> 3;// ob00 111 000
    PORTD = adc >> 6; 
}

void ADC_Show(void)
{
        int adc = 0;
        int channel = 7;
        ADCON0 |= channel << 3;
        __delay_ms(20);
        GO_nDONE = 1; //Initializes A/D Conversion
        while(GO_nDONE); //Wait for A/D Conversion to complete
        adc = ((ADRESH<< 8)+ADRESL); //Returns Result
        Show_Led((adc*1.0/1023)*255);
}

void main(void) 
{
    TRISB = 0;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    
    ADC_Config();
    
    while(1)
    {
        ADC_Show();
    }
    return;
}
