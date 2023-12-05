#include <util/delay.h>
#include <stdlib.h>
#include "adc.h"
#include "lcd.h"
#include "Dio.h"
#include "pwm.h"
#include "Uart.h"



int main(void) {
    
  unsigned short adc_reading;
  unsigned char buffer[4];
  unsigned char buffer2[4];
    Adc_Init();
    LCD_Init();
    DIO_init();
    PWM_Init();
    int SetTemp = 25;

    while (1) {

      adc_reading = Adc_ReadChannel(1);
        intarg(SetTemp, buffer2, 10);

        
        float temperatureCelsius = ((adc_reading / 1024.0) * 5 * 100.0);
        intarg(temperatureCelsius, buffer, 10);
        int RPS2  = readPinState(2);
        int RPS3  = readPinState(3);


          LCD_Command(0x80);
          LCD_String("Temp:  ");
          LCD_String(buffer);
          LCD_Command(0xC0);
          LCD_String("SetTemp:  ");
          LCD_String(buffer2);
        
         
        if (RPS2 == 0) {
          _delay_ms(50);
          SetTemp = SetTemp+1;
        } 
        else if(RPS3 == 0) {
          _delay_ms(50);
          SetTemp = SetTemp-1;
       }
        
        if (temperatureCelsius > SetTemp) {
            Set_DutyCycle(0,3);
            Set_DutyCycle(255,11);
        } else {
            Set_DutyCycle(255,3);
            Set_DutyCycle(0,11);
        }

        _delay_ms(200); // Delay for 0.2 second
    } /* End event loop */
    
    return 0;
}