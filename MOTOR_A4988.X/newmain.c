/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on April 10, 2022, 7:43 AM
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

#define EN_SET    TRISBbits.TRISB0
#define DIR_SET   TRISBbits.TRISB1
#define STEP_SET  TRISBbits.TRISB2

#define MS1_SET   TRISBbits.TRISB3
#define MS2_SET   TRISBbits.TRISB4
#define MS3_SET   TRISBbits.TRISB5

#define MS1       PORTBbits.RB3
#define MS2       PORTBbits.RB4
#define MS3       PORTBbits.RB5

#define EN        PORTBbits.RB0
#define DIR       PORTBbits.RB1
#define STEP      PORTBbits.RB2

#define BTN_DIR   TRISDbits.TRISD0
#define BTN       PORTDbits.RD0
                 
uint16_t u16Count_TOP, u16Count_BOT, u16Count_timer0;  

void Speed_M(int16_t x) 
{
  if (x < 0) 
  {
    DIR = 0;
    u16Count_BOT = -x;
    u16Count_TOP = -x/2;
  }
  else if (x > 0) 
  {
    DIR = 1;
    u16Count_BOT = x;
    u16Count_TOP = (uint16_t)x/2;
  }
  else 
  {
    u16Count_BOT = 0;
    u16Count_TOP = 0;
  }
}
void Reverse_Motor(int i)
{
    if(i%2 == 0)
    {
        DIR = 0;
    }
    else DIR = 1;
}

void main(void) 
{
    uint16_t count = 0;
    
    EN_SET = 0;
    DIR_SET = 0;
    STEP_SET = 0;

    MS1_SET = 0;
    MS2_SET = 0;
    MS3_SET = 0;
    
    MS1 = 1;
    MS2 = 1;
    MS3 = 1;
    
    EN = 0;   // On module

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;

    TMR0 = 217;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1;
    
    Speed_M(400);

    while(1)
    {
        if(0 == BTN)
        {
            count++;
            __delay_ms(20);
            while(!BTN);
            Reverse_Motor(count);
        }
    }
    return;
}

void __interrupt() ISR() 
{
    if(INTCONbits.TMR0IF)
    {     
        TMR0 = 217;
        INTCONbits.TMR0IF = 0;
        
        u16Count_timer0++;
        if (u16Count_timer0 <= u16Count_TOP) STEP = 1;  
        else STEP = 0;
        if (u16Count_timer0 >= u16Count_BOT) 
        {
            u16Count_timer0 = 0;
        }
    } 
}

