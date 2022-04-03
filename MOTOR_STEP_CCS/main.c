#include <main.h>

Signed int8 Dir_M1;
Signed int8 Dir_M2;
unsigned int16 Count1_timer0 = 0, Count2_timer0 = 0;                 
unsigned int16 Count_TOP1, Count_BOT1, Count_TOP2, Count_BOT2;  

void Speed(Signed int16 x, Signed int16 y) 
{
  if (x < 0) 
  {
    Dir_M1 = -1;
    //DIR1 = 0;
    x =abs(x);
  }
  else if (x > 0) 
  {
    Dir_M1 = 1;
    //DIR1 = 1;
  }
  else Dir_M1 = 0;
  
   Count_BOT1 = x;
   Count_TOP1 = x/2;
   
  if (y < 0) 
  {
    Dir_M2 = -1;
    //DIR2 = 0;
    y = abs(y);
  }
  else if (y > 0) 
  {
    Dir_M2 = 1;
    //DIR2 = 1;
  }
  else Dir_M2 = 0;
  

   Count_BOT2 = y;
   Count_TOP2 = y/2;
}


#INT_TIMER0
void timer0_isr(void)
{
   clear_interrupt(INT_TIMER0);
   set_timer0(217);
   
   if (Dir_M1 != 0) 
    {                                                          
        Count1_timer0++;
        if (Count1_timer0 <= Count_TOP1) output_bit(PIN_B2, 1);  
        else output_bit(PIN_B2, 0);

    }  
    if (Dir_M2 != 0) 
    {                                                          
        Count2_timer0++;
        if (Count2_timer0 <= Count_TOP2) output_bit(PIN_D7, 1);
        else output_bit(PIN_D7, 0);

    } 
    if (Count1_timer0 >= Count_BOT1) 
    {
        Count1_timer0 = 0; 
    }
    if (Count2_timer0 >= Count_BOT2) 
    {
        Count2_timer0 = 0;
    }
    
}


void main()
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);   // Configure Timer0 module with internal oscillator and 256 prescaler
   set_timer0(217);                              // Timer0 preload value = 61
   clear_interrupt(INT_TIMER0);                 // Clear Timer0 interrupt flag bit
   enable_interrupts(INT_TIMER0);               // Enable Timer0 interrupt
   enable_interrupts(GLOBAL);                   // Enable global interrupts
   output_low(PIN_B2);
   output_low(PIN_D7);

   while(TRUE)
   {
      Speed(400, -400);
  

      //TODO: User Code
   }

}
