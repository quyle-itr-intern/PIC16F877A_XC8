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

# define _XTAL_FREQ 20000000

#include <xc.h>

int16_t count = 0;

void Delay_1s(void)
{
    count = 0;
    while(count < 1000);
}

void main() 
{
    TRISB = 0X00;
    PORTB = 0X00;
    // Cau hinh timer0
    OPTION_REGbits.T0CS = 0 ; //XUNG NOI 54/234
    OPTION_REGbits.PSA = 0; // Timer0 module
        // Chia ti le
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
        
        // INTERUPT  153/234
    TMR0 = 100 ; // Nap truoc 100 de delay 1ms 100-255
    INTCONbits.GIE = 1; // INTERUPT TOAN CUC
    INTCONbits.TMR0IE = 1; // INTERUPT TIMER0
    INTCONbits.TMR0IF = 0; // xoa co tran timer0
    
    while(1)
    {
        PORTBbits.RB0 = !PORTBbits.RB0;
        Delay_1s();
    }
        
}

void __interrupt() Timer_0(void) // 1ms
{
    if(INTCONbits.TMR0IF)
    {
        TMR0 = 100; //  Nap truoc 100
        count++;
        INTCONbits.TMR0IF = 0; // Xoa co bao ngat bang phan mem
    }
}