/*
 * File:   main.c
 * Author: levan
 *
 * Created on February 28, 2022, 1:58 PM
 */
#include <pic16f877a.h>

#include "main.h"
#include "spi.h"

#define NRF_CE_SET() { TRISBbits.TRISB0 = 0; }
#define NRF_CE_SetLow() { PORTBbits.RB0 = 0; }
#define NRF_CE_SetHigh() { PORTBbits.RB0 = 1; }

#define NRF_CSN_SET() { TRISBbits.TRISB1 = 0; }
#define NRF_CSN_SetLow() { PORTBbits.RB1 = 0; }
#define NRF_CSN_SetHigh() { PORTBbits.RB1 = 1; }

void RxSetUp(){
    /**/
    NRF_CE_SetLow(); //PRX, CRC (2-byte) enabled

    NRF_CSN_SetLow(); // 2nS minimum delay required
    SPI_WriteByte(0x20);
    SPI_WriteByte(0x3D); // 0x3D: receive, 0x7C: transmit mode (no interrupts)
    NRF_CSN_SetHigh(); 
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //disable auto-ack for all channels
    SPI_WriteByte(0x21); 
    SPI_WriteByte(0x00);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //address width = 4 bytes
    SPI_WriteByte(0x23);
    SPI_WriteByte(0x02); // 4 byte address width
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); // ARC = 0
    SPI_WriteByte(0x24);
    SPI_WriteByte(0x00);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //set RF channel
    SPI_WriteByte(0x25);
    SPI_WriteByte(0x13);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //data rate = 250kbps
    SPI_WriteByte(0x26);
    SPI_WriteByte(0x26);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //set Receive address (LSB first)
    SPI_WriteByte(0x2A); //Write 0x0A (Rx_ADDR_P0)
    SPI_WriteByte(0xDE); //'11011110'
    SPI_WriteByte(0xDE); //'11011110'
    SPI_WriteByte(0xE1); //'11100001'
    SPI_WriteByte(0x13); //'00010011'
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //set TX address (LSB first)
    SPI_WriteByte(0x30);
    SPI_WriteByte(0xDE); //'11011110'
    SPI_WriteByte(0xDE); //'11011110'
    SPI_WriteByte(0xE1); //'11100001'
    SPI_WriteByte(0x13); //'00010011'
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //8 byte payload
    SPI_WriteByte(0x31);
    SPI_WriteByte(0x08);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); // flush TX
    SPI_WriteByte(0xE1); 
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); // FLUSH_RX
    SPI_WriteByte(0xE2);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required

    NRF_CSN_SetLow(); //PWR_UP = 1
    SPI_WriteByte(0x20);
    SPI_WriteByte(0x3F); // 0x3F: receive, 0x7E: transmit (no interrupts)
    NRF_CSN_SetHigh(); // active low SPI enable
    //delay(1); // 50nS minimum delay required

    NRF_CE_SetHigh(); // monitor the air
}

void TxSetUp(){
    
    uint8_t RFID3 = 0x02; //set RF frequency
    
    //3 Most Significant Address Bytes, 4th Byte Derived from Frequency
    uint8_t RFID0 = 0xAA; //'10101010'
    uint8_t RFID1 = 0xAA;
    uint8_t RFID2 = 0xAA;
    
    NRF_CE_SetHigh(); // TX mode
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x20); // Address 0x00:(CONFIG)
    SPI_WriteByte(0x7C); // PTX, CRC (2-byte) enabled, 0x7C: transmit mode (no interrupts) // MASK_RX-DR=1,MASK_TX_DS=1,MASK_MAX_RT=1,EN_CRC=1,CRCO=1,PWR_UP=0,PRIM_RX=0
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x21); // Address 0x01: (EN_AA)
    SPI_WriteByte(0x00); // disable auto-ack for all channels // EN_AAP5-4=0
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x23); // Address 0x03: (SETUP_AW)
    SPI_WriteByte(0x02); // address width = 4 bytes // RX/TX ADDRESS FIELD WITH, '10'-4 BYTES
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x24); // Address 0x04: (SETUP_RETR)
    SPI_WriteByte(0x00); // ARC = 0 // ARD-WAIT 250µs, ARC-RETRANSMIT DIABLED
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow(); 
    SPI_WriteByte(0x25); // Address 0x05:
    SPI_WriteByte(RFID3); // set RF frequency // set RF frequency - CHANNEL 2,(Which is the default?)                            
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x26); // Address 0x06:
    SPI_WriteByte(0x26); // data rate = 250kbps, 0dBm '00100110'
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x30); // Address 0x10: set RF TX address ID
    SPI_WriteByte(RFID0); // (0x20 | 0x10)
    SPI_WriteByte(RFID1); // 4 bytes of TRANSMIT ADDRESS. USED FOR A PTX DEVICE ONLY
    SPI_WriteByte(RFID2); // RFIDx - A 4-BYTE USER DEFINED ADDRESS???
    SPI_WriteByte(RFID3);
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x31); // Address 0x11: 8 byte payload (0X20 | OX11)
    SPI_WriteByte(0x08); // NUMBER OF BYTES IN RX PAYLOAD IN DATAPIPE 0 // 8 BYTES
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0xE1); // flush TX // FLUSH TX FIFO, USED IN TX MODE
    NRF_CSN_SetHigh();
    __delay_us(1); // 50nS minimum delay required
    
    NRF_CSN_SetLow();
    SPI_WriteByte(0x20); // PWR_UP = 1
    SPI_WriteByte(0x7E); // 0x3F: receive, 0x7E: transmit (no interrupts) // MASK_RX-DR=1,MASK_TX_DS=1,MASK_MAX_RT=1,EN_CRC=1,CRCO=1,PWR_UP=0,PRIM_RX=1 // MAKES PRX??
    NRF_CSN_SetHigh(); // active low SPI enable
    //__delay_us(1); // 50nS minimum delay required
    
    NRF_CE_SetLow();
}

void main(void) 
{
    SPI_Initialize();
    TxSetUp();
    
   
    while(1)
    {
        
    }
    return;
}
