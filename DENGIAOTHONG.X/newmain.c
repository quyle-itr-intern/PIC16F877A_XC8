/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on January 21, 2022, 4:06 PM
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
unsigned char Number[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0xFF, 0x6F};

void main(void) {
    
    signed char dem;
    unsigned char LED_CHUC, LED_DVI;
    unsigned int i;
    TRISC = 0;
    TRISB = 0;
    PORTC = 0;
    while(1)
    {
        PORTCbits.RC0 = 1;
        for(dem = 25; dem >= 0; dem--)
		{
			LED_CHUC = dem/10;
			LED_DVI = dem%10;
			for(i = 0; i < 500; i++)
			{
				// LED1 sang
				PORTB = Number[LED_CHUC];
				PORTCbits.RC3 = 1;
				__delay_ms(1);
				PORTCbits.RC3 = 0;
				
				// LED2 sang
				PORTB = Number[LED_DVI];
				PORTCbits.RC4 = 1;
				__delay_ms(1);
				PORTCbits.RC4 = 0;
			}
		}
        PORTCbits.RC0 = 0;
        
        PORTCbits.RC1 = 1;
        for(dem = 5; dem >= 0; dem--)
		{
			LED_CHUC = dem/10;
			LED_DVI = dem%10;
			for(i = 0; i < 500; i++)
			{
				// LED1 sang
				PORTB = Number[LED_CHUC];
				PORTCbits.RC3 = 1;
				__delay_ms(1);
				PORTCbits.RC3 = 0;
				
				// LED2 sang
				PORTB = Number[LED_DVI];
				PORTCbits.RC4 = 1;
				__delay_ms(1);
				PORTCbits.RC4 = 0;
			}
		}
        PORTCbits.RC1 = 0;
        
        PORTCbits.RC2 = 1;
        for(dem = 20; dem >= 0; dem--)
		{
			LED_CHUC = dem/10;
			LED_DVI = dem%10;
			for(i = 0; i < 500; i++)
			{
				// LED1 sang
				PORTB = Number[LED_CHUC];
				PORTCbits.RC3 = 1;
				__delay_ms(1);
				PORTCbits.RC3 = 0;
				
				// LED2 sang
				PORTB = Number[LED_DVI];
				PORTCbits.RC4 = 1;
				__delay_ms(1);
				PORTCbits.RC4 = 0;
			}
		}
        PORTCbits.RC2 = 0;

    }
    return;
}
