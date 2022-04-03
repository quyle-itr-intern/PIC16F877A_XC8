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
#define   RAD2DEG   57.295779
// PID
int16_t constrain(float a, int16_t min, int16_t max)
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
void PWM1_Init()
{
    IN1_SET = 0;
    IN2_SET = 0;
    PR2 = _XTAL_FREQ/(4*PWM_Frequency*TMR2PRESCALE) - 1; 
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    if(TMR2PRESCALE == 1)
    {
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 0;
    }
    if(TMR2PRESCALE == 4)
    {
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 1;
    }
    if(TMR2PRESCALE == 16)
    {
        T2CONbits.T2CKPS1 = 1;
        T2CONbits.T2CKPS0 = 1;
    }
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;
    TRISCbits.TRISC2 = 0;
    
}
void PWM2_Init()
{
    IN3_SET = 0;
    IN4_SET = 0;
    PR2 = _XTAL_FREQ/(4*PWM_Frequency*TMR2PRESCALE) - 1; 
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    if(TMR2PRESCALE == 1)
    {
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 0;
    }
    if(TMR2PRESCALE == 4)
    {
        T2CONbits.T2CKPS1 = 0;
        T2CONbits.T2CKPS0 = 1;
    }
    if(TMR2PRESCALE == 16)
    {
        T2CONbits.T2CKPS1 = 1;
        T2CONbits.T2CKPS0 = 1;
    }
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;
    TRISCbits.TRISC1 = 0;
}
void PWM1_Set_Duty(unsigned int duty)
{
    if(duty < 1024)
    {
        duty = ((float)duty/1023)*(_XTAL_FREQ/(PWM_Frequency*TMR2PRESCALE));
        CCP1X = duty & 1;
        CCP1Y = duty & 2;
        CCPR1L = duty >> 2;
    }
}
void PWM2_Set_Duty(unsigned int duty)
{
    if(duty < 1024)
    {
        duty = ((float)duty/1023)*(_XTAL_FREQ/(PWM_Frequency*TMR2PRESCALE));
        CCP2X = duty & 1;
        CCP2Y = duty & 2;
        CCPR2L = duty >> 2;
    }
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

int MPU6050_GetAngle()
{
  int Ax;
  Ax = MPU6050_Read(ACCEL_XOUT_H) << 8 | MPU6050_Read(ACCEL_XOUT_L);
  Ax /= 180;
  return Ax;
}
