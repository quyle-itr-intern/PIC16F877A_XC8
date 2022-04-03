/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 20, 2022, 7:16 PM
 */
#include "main.h"
#include "MPU6050.h"
#include "UART.h"
#include "I2C.h"

#define EN1_SET TRISBbits.TRISB4
#define DIR1_SET TRISBbits.TRISB3
#define STEP1_SET TRISDbits.TRISD6

#define EN2_SET TRISBbits.TRISB1
#define DIR2_SET TRISBbits.TRISB0
#define STEP2_SET TRISDbits.TRISD7

#define MS1_SET TRISBbits.TRISB5
#define MS2_SET TRISBbits.TRISB6
#define MS3_SET TRISBbits.TRISB7

#define MS1 PORTBbits.RB5
#define MS2 PORTBbits.RB6
#define MS3 PORTBbits.RB7

#define EN1 PORTBbits.RB4
#define DIR1 PORTBbits.RB3
#define STEP1 PORTDbits.RD6

#define EN2 PORTBbits.RB1
#define DIR2 PORTBbits.RB0
#define STEP2 PORTDbits.RD7

int8_t Dir_M1, Dir_M2;
volatile int Count1_timer0 = 0, Count2_timer0 = 0;                 
int32_t Step1, Step2;
unsigned int Count_TOP1, Count_BOT1, Count_TOP2, Count_BOT2;  
 
void Speed_M1(int16_t x) 
{
  if (x < 0) 
  {
    Dir_M1 = -1;
    DIR1 = 0;
    Count_BOT1 = -x;
    Count_TOP1 = -x/2;
  }
  else if (x > 0) 
  {
    Dir_M1 = 1;
    DIR1 = 1;
    Count_BOT1 = x;
    Count_TOP1 = (unsigned int)x/2;
  }
  else 
  {
    Dir_M1 = 0;
    Count_BOT1 = 0;
    Count_TOP1 = 0;
  }
}

void Speed_M2(int16_t x) 
{
  if (x < 0) 
  {
    Dir_M2 = -1;
    DIR2 = 0;
    Count_BOT2 = -x;
    Count_TOP2 = -x/2;
  }
  else if (x > 0) 
  {
    Dir_M2 = 1;
    DIR2 = 1;
    Count_BOT2 = x;
    Count_TOP2 = x/2;
  }
  else 
  {
    Dir_M2 = 0;
    Count_BOT2 = 0;
    Count_TOP2 = 0;  
  }
}

void Step()
{
    if (Dir_M1 != 0) 
    {        
        Count1_timer0++;
        if (Count1_timer0 <= Count_TOP1) STEP1 = 1;  
        else STEP1 = 0;
        if (Count1_timer0 >= Count_BOT1) 
        {
            Count1_timer0 = 0;
        }
    }  
    if (Dir_M2 != 0) 
    {       
        Count2_timer0++;
        if (Count2_timer0 <= Count_TOP2) STEP2 = 1;
        else STEP2 = 0;
        if (Count2_timer0 >= Count_BOT2) 
        {
            Count2_timer0 = 0;
        }
    } 
}

void main(void) 
{
    MPU6050_Init();
    UART_Init();
//    OPTION_REGbits.T0CS = 0;
//    OPTION_REGbits.PSA = 0;
//    OPTION_REGbits.PS2 = 0;
//    OPTION_REGbits.PS1 = 0;
//    OPTION_REGbits.PS0 = 0;
//
//    TMR0 = 217;
//    INTCONbits.TMR0IF = 0;
//    INTCONbits.TMR0IE = 1;

    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 0;

    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PR2 = 9;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;

    EN1_SET = 0;
    DIR1_SET = 0;
    STEP1_SET = 0;
    
    EN2_SET = 0;
    DIR2_SET = 0;
    STEP2_SET = 0;
    
    MS1_SET = 0;
    MS2_SET = 0;
    MS3_SET = 0;
    
    MS1 = 1;
    MS2 = 1;
    MS3 = 1;
    
    EN1 = 0;   // On module
    EN2 = 0;
   
    
    
    
    while(1)
    {
        MPU6050_Readvalue();
        Speed_M1(50);
        Speed_M2(50);
    }
    return;
}

void __interrupt() ISR() 
{
    if(PIR1bits.TMR2IF)
    { 
        T2CONbits.TMR2ON = 0;
        Step();
        TMR2 = 0;
        T2CONbits.TMR2ON = 1;
        PIR1bits.TMR2IF = 0;
    }
}
