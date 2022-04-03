/*
 * File:   main.c
 * Author: levan
 *
 * Created on February 11, 2022, 7:16 AM
 */
#include "main.h"
#include "UART.h"
#include "I2C.h"
#include "MPU6050.h"
#include <stdlib.h>

void Speed_L(int speed)
{
    if(speed > 0)
    {
        IN1 = 1;
        IN2 = 0;
    }
    else if(speed < 0)
    {
        IN1 = 0;
        IN2 = 1;
    }
    PWM1_Set_Duty(abs(speed));
}
void Speed_R(int speed)
{
    if(speed > 0)
    {
        IN3 = 1;
        IN4 = 0;
    }
    else if(speed < 0)
    {
        IN3 = 0;
        IN4 = 1;
    }
    PWM2_Set_Duty(abs(speed));
}

int16_t inputL, inputR, Offset, Output, I_L, I_R, input_lastL, input_lastR, OutputL, OutputR, MotorL, MotorR, Vgo, Vgo_L, Vgo_R;
float Kp = 38;
float Ki = 0.5;
float Kd = 0;


void main(void) 
{
    //UART_Init();
    MPU6050_Init();
    PWM1_Init();
    PWM2_Init();
    
    int Angle;
    //char tx[50];

    while(1)
    {
        Angle = MPU6050_GetAngle();
        Offset = 0;
        Vgo=0;
        Vgo_L = 0;
        Vgo_R = 0;
        inputL = Angle + Offset - Vgo_L; //Vgo>0  ch?y lui,Vgo <0 ch?y t?i
        I_L += inputL;
        OutputL = Kp * inputL + Ki * I_L + Kd * (inputL - input_lastL);
        input_lastL = inputL;
        if (OutputL > -100 && OutputL < 100) OutputL = 0;
        OutputL = constrain(OutputL, -1023, 1023);
        
        inputR = Angle + Offset - Vgo_R; //Vgo>0  ch?y lui,Vgo <0 ch?y t?i
        I_R += inputR;
        OutputR = Kp * inputR + Ki * I_R + Kd * (inputR - input_lastR);
        input_lastR = inputR;
        if (OutputR > -100 && OutputR < 100) OutputR = 0;
        OutputR = constrain(OutputR, -1023, 1023);
        
//        if (OutputL > 0) MotorL  = 0.523*OutputL + 500;
//        else if (OutputL < 0)  MotorL  = -0.523*OutputL - 500;
//        else MotorL = 0;
//
//        if (OutputR > 0) MotorR  = 0.523*OutputR + 500; //OutputR = 1    ----> M_R = -145
//        //OutputR = 4.58 ----> M_R = 0
//        //OutputR = 10   ----> M_R = 115.52
//        //OutputR = 400  ----> M_R = 391.55
//        else if (OutputR < 0) MotorR  = -0.523*OutputR - 500;
//        else MotorR = 0;
        
        Speed_L(OutputL);
        Speed_R(OutputR);
//        sprintf(tx, "Ax : %d --- %d\r\n", Angle,  OutputL);
//        UART_Transmit_Text(tx);
        __delay_ms(80);
    }
    return;
}
//        sprintf(tx, "Ax : %d\r\n", Angle);
//        UART_Transmit_Text(tx);
//        __delay_ms(10);