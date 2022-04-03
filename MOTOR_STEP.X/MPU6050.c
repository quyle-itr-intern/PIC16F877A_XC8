/*
 * File:   MPU6050.c
 * Author: levan
 *
 * Created on February 10, 2022, 9:49 PM
 */
//-------------[ MPU6050 Routines ]------------------
//---------------------------------------------------
#include "main.h"
#include "I2C.h"
#include "MPU6050.h"

#include "UART.h"  // for debugging serial terminal
#include <stdio.h>
#include <math.h>

int constrain(float a, int min, int max)
{
    if(a < min)
    {
        return min;
    }
    else if (a > max)
    {
        return max;
    }
    return a;
}
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//------------------------------------------------------------------------------
void MPU6050_Write(unsigned char add,unsigned char data)
{
   I2C_Start();
   I2C_Write(0xD0);
   I2C_Write(add);
   I2C_Write(data);
   I2C_Stop();
}
int MPU6050_Read(unsigned char add)
{
   int retval;
   I2C_Start();
   I2C_Write(0xD0);
   I2C_Write(add);
   I2C_Stop();
   I2C_Start();
   I2C_Write(0xD1);
   retval = I2C_Read(1);
   I2C_Stop();
   return retval;
}

void MPU6050_Init()
{
  // Power-Up Delay & I2C_Init
  __delay_ms(100);
  I2C_Init();
 
  // Setting The Sample (Data) Rate
  MPU6050_Write(SMPLRT_DIV, 0x07);
 
  // Setting The Clock Source
  MPU6050_Write(PWR_MGMT_1, 0x01);

  // Configure The DLPF
  MPU6050_Write(CONFIG, 0x00);

  // Configure The ACCEL (FSR= +-2g)
  MPU6050_Write(ACCEL_CONFIG, 0x00);
 
  // Configure The GYRO (FSR= +-2000d/s)
  MPU6050_Write(GYRO_CONFIG, 0x18);

  // Enable Data Ready Interrupts
  //MPU6050_Write(INT_ENABLE, 0x01);
  
//  MPU6050_Write(PWR_MGMT_1, 0x80);
//   __delay_ms(100);
//   MPU6050_Write(PWR_MGMT_1, 0x00);
//   __delay_ms(100);
//   MPU6050_Write(CONFIG, 0x01);
//   __delay_ms(10);
//   MPU6050_Write(GYRO_CONFIG, 0x00);

}

void MPU6050_Readvalue()
{
  INTCONbits.PEIE = 0;
  char buffer[40];
  int Ax;
  
  Ax = ((int)MPU6050_Read(ACCEL_YOUT_H)<<8)|(MPU6050_Read(ACCEL_YOUT_L));
  
  
//  if(Ax <=181 && Ax >=179)
//    {
//      stopMotor();
//    }
//    else
//    {
//      if(Ax < 250 && Ax >110)
//      {
//        Pid(Ax);
//        Motor();
//      }
////      else
////      {
////        stopMotor();
////      }
//    }
////    if(Ax < 250 && Ax >110)
////      {
////        Pid(Ax);
////        Motor();
////      }
//  sprintf(buffer,"Ay = %d     ",Ax);
//  UART_Transmit_Text(buffer);
// 
//  sprintf(buffer," Ay = %d    ",Ay);
//  UART_Transmit_Text(buffer);
 
  sprintf(buffer," Ax = %d    \n",Ax);
  UART_Transmit_Text(buffer);
 
//  sprintf(buffer," Gx = %d    ",Gx);
//  UART_Transmit_Text(buffer);
// 
//  sprintf(buffer," Gy = %d    ",Gy);
//  UART_Transmit_Text(buffer);
 
//  sprintf(buffer," Gz = %d\r\n",Gz);
//  UART_Transmit_Text(buffer);
  INTCONbits.PEIE = 1;
}
