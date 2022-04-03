/*
 * File:   UART.c
 * Author: levan
 *
 * Created on February 10, 2022, 3:00 PM
 */
#include "UART.h"

void UART_Init()
{
    SET_TX = 0;
    SET_RX = 1;
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
void __interrupt() isr1() 
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
