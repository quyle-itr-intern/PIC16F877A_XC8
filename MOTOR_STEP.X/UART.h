/* 
 * File:   UART.h
 * Author: levan
 *
 * Created on February 10, 2022, 3:00 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"
    
#define SET_TX TRISCbits.TRISC6
#define SET_RX TRISCbits.TRISC7
#define SizeData 50

char RX_Buffer[SizeData];
char rx_index = 0;

void UART_Init();
void UART_Transmit(char c);
void UART_Transmit_Text(char *str);
void UART_Receive();
void HandleData(char *RX);
void ClearRX_Buffer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

