/*
 * File:   newmain.c
 * Author: levan
 *
 * Created on February 7, 2022, 8:13 AM
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
#include <stdio.h>
#define _XTAL_FREQ 20000000

#define SET_TX TRISCbits.TRISC6
#define SET_RX TRISCbits.TRISC7
#define SizeData 50

char RX_Buffer[SizeData];
char rx_index = 0;

void IO_Init();
void UART_Init();
void UART_Transmit(char c);
void UART_Transmit_Text(char *str);
void UART_Receive();
void HandleData(char *RX);
void ClearRX_Buffer(void);

#define DHT11_TRIS TRISDbits.TRISD1
#define DHT11_PORT PORTDbits.RD1

void IO_Init()
{
    SET_TX = 0;
    SET_RX = 1;
    TRISB = 0;
    PORTB = 0x00;
}
void UART_Init()
{
    // TX Transmit
    SPBRG = 32;  // Set (Asynchronous) Baud Rate 9600baud
    SPEN = 1;    // Serial port enabled (configures RC7/RX/DT and RC6/TX/CK pins as serial port pins)
    TXEN = 1;    // Transmit enabled
    CREN = 1;    // Enables continuous receive
    RCIE = 1;    // Enables interrupts
    INTCONbits.GIE = 1;      // Enables interrupts global
    INTCONbits.PEIE = 1;     // Enables interrupts peripheral
}
void UART_Transmit(char c)
{
    while(!TRMT); // Wait until TSR Register empty
    TXREG = c;
}
void UART_Transmit_Text(char *str)
{
    unsigned char i=0;
    while(str[i] != 0)
    {
        UART_Transmit(str[i]);
        i++;
    }
}
void UART_Receive()
{
    if(RCIF)   // Wait until RSR Register complete
    {
        char ReceivedData;
        ReceivedData = RCREG;
        if(ReceivedData != '\n')
        {
            RX_Buffer[rx_index++] = RCREG;
        }
        else
        {
            HandleData(RX_Buffer);
            ClearRX_Buffer();
        }
    }
}
void HandleData(char *RX)
{
    UART_Transmit_Text(RX);
}
void ClearRX_Buffer(void)
{
	char i;
	for(i = 0; i < SizeData; i++)
	{
		RX_Buffer[i] = 0;		
	}
	rx_index = 0;
}
void TMR0_Config(void)
{
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;
}
unsigned char TMR0_Get_Value(void)
{
    return TMR0;
}
void TMR0_Set_Value(unsigned char value)
{
    TMR0 = value;
}
void DHT11_Init(void)
{
    DHT11_TRIS = 0;
    DHT11_PORT = 0;
    TMR0_Config();
}
unsigned char ReadDHT11(unsigned char* u8Buff)
{
    unsigned char u8Check, i, u8TimeOut;
    DHT11_TRIS = 0;
    DHT11_PORT = 0;
    __delay_ms(20);
    DHT11_PORT = 1;
    
    u8TimeOut = 1;
    TMR0_Set_Value(0);
    while(TMR0_Get_Value() < 15)  // 12us
    {
        if(DHT11_PORT) 
        {
            u8TimeOut = 0;
            break;
        }
    }
    if(u8TimeOut) return 0;
    
    DHT11_TRIS = 1;
    u8TimeOut = 1;
    TMR0_Set_Value(0);
    while(TMR0_Get_Value() < 70)  // 56us
    {
        if(!DHT11_PORT) 
        {
            u8TimeOut = 0;
            break;
        }
    }
    if(u8TimeOut) return 0;
    
    u8TimeOut = 1;
    TMR0_Set_Value(0);
    while(TMR0_Get_Value() < 115)  // 92us
    {
        if(DHT11_PORT)
        {
            u8TimeOut = 0;
            break;
        }
    }
    
    if(u8TimeOut) return 0;
    
    u8TimeOut = 1;
    TMR0_Set_Value(0);
    while(TMR0_Get_Value() < 115)  // 92us
    {
        if(!DHT11_PORT)
        {
            u8TimeOut = 0;
            break;
        }
    }
    if(u8TimeOut) return 0;
    
    // Read byte 1 -------------------------------------------------------------
    for(i=0;i<8;i++)
    {
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 85)  // 68us
        {
            if(DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 110)  // 88us
        {
            if(!DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8Buff[0] <<= 1;
        if(TMR0_Get_Value() > 60)
        {
            u8Buff[0] |= 0x01;
        }
    }
    
    // Read byte 2 -------------------------------------------------------------
    for(i=0;i<8;i++)
    {
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 85)  // 68us
        {
            if(DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 110)  // 88us
        {
            if(!DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8Buff[1] <<= 1;
        if(TMR0_Get_Value() > 60)
        {
            u8Buff[1] |= 0x01;
        }
    }
    
    // Read byte 3 -------------------------------------------------------------
    for(i=0;i<8;i++)
    {
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 85)  // 68us
        {
            if(DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 110)  // 88us
        {
            if(!DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8Buff[2] <<= 1;
        if(TMR0_Get_Value() > 60)
        {
            u8Buff[2] |= 0x01;
        }
    }
    
    // Read byte 4 -------------------------------------------------------------
    for(i=0;i<8;i++)
    {
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 85)  // 68us
        {
            if(DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 110)  // 88us
        {
            if(!DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8Buff[3] <<= 1;
        if(TMR0_Get_Value() > 60)
        {
            u8Buff[3] |= 0x01;
        }
    }
    
    // Read byte 5 -------------------------------------------------------------
    for(i=0;i<8;i++)
    {
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 85)  // 68us
        {
            if(DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8TimeOut = 1;
        TMR0_Set_Value(0);
        while(TMR0_Get_Value() < 110)  // 88us
        {
            if(!DHT11_PORT)
            {
                u8TimeOut = 0;
                break;
            }
        }
        if(u8TimeOut) return 0;
        
        u8Buff[4] <<= 1;
        if(TMR0_Get_Value() > 60)
        {
            u8Buff[4] |= 0x01;
        }
    }
    
    // Check
    u8Check =  u8Buff[0] + u8Buff[1] + u8Buff[2] + u8Buff[3];
    if(u8Check != u8Buff[4])
    {
        return 0;
    }
    return 1;
}

void main(void) 
{
    unsigned char data[5];
    char text[40];
    IO_Init();
    UART_Init();
    DHT11_Init();
    
    while(1)
    {
        if(ReadDHT11(data) == 1)
        {
            sprintf(text, "Temperature: %d, Humidity: %d\n", data[2], data[0]);
            UART_Transmit_Text(text);
        }
        else
        {
            UART_Transmit_Text("Error\n");
        }
        __delay_ms(500);
    }
    return;
}

void __interrupt() RX() 
{
    if(RCIE)
    {
        if(RCIF)   // Wait until RSR Register complete
        {
            char ReceivedData;
            ReceivedData = RCREG;
            if(ReceivedData != '\n')
            {
                RX_Buffer[rx_index++] = RCREG;
            }
            else
            {
                HandleData(RX_Buffer);
                ClearRX_Buffer();
            }
        }
    }
}