/*
 * File:   main.c
 * Author: levan
 *
 * Created on January 7, 2022, 6:56 AM
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

void main(void) {
    // 1. Set the PWM period by writing to the PR2 register
    // Formula to calculate the value of the period register.
    // PR2 = ((Fosc)/(4*TMR2Prescale*PWMFrequency))-1         
    // we use a 20MHz clock and the o/p frequency is 5KHz and TMR2 Prescale is 4.
    // PR2 = ((20000000)/(4*4*5000))-1 = 249 = 0xF9
    PR2 = 0xF9;
    // 2. Set the PWM duty cycle by writing to the CCPR1L register and CCP1CON<5:4> bits.
    // Calculate the value of registers 
    // CCPR1L and CCP1CON<5:4> for the given duty cycle.
    // (CCPR1L:CCP1CON<5:4>) = PWM Duty Cycle / (TOSC * (TMR2 Prescale Value));
    // Value to be loaded in Duty cycle register (CCPR1L) = %Duty Cycle x PR2 Value
    CCPR1L = 0xBA;
    // Value to be loaded in CCP1CON = 0x1C // 0.25 value after decimal point. PWM mode of operation of CCP module
    // Value to be loaded in CCP1CON = 0x2C // 0.5 value after decimal point. PWM mode of operation of CCP module
    // Value to be loaded in CCP1CON = 0x3C // 0.75 value after decimal point. PWM mode of operation of CCP module
    // Value to be loaded in CCP1CON = 0x0C // 0.00 value after decimal point. PWM mode of operation of CCP module
    CCP1CON = 0x3C;
    // 3. Make the CCP1 pin an output by clearing the TRISC<2> bit.
    TRISCbits.TRISC2 = 0; //Pwm
    // 4. Set the TMR2 prescale value and enable Timer2 by writing to T2CON.
    TMR2 = 0x00;   // Load TMR2=00H
    T2CON = 0x00;  // Prescaler is 1
    T2CON |= 0x04; // Enable the Timer2, hence enable the PWM.
    // 5. Configure the CCP1 module for PWM operation in step 2.
    
    while(1)
    {
        
    }
    return;
}
