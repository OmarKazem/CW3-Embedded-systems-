#include <util/delay.h>
#include <stdlib.h>
#include <EEPROM.h>
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
    PWM_Init();
    Uart_Init();
    int SetTemp;
    int address=0;
    
    

    while (1) {
        SetTemp = EEPROM.read(address); // Read the set temperature from EEPROM
        adc_reading = Adc_ReadChannel(1);
        float temperatureCelsius = ((adc_reading / 1024.0) * 5 * 100.0);
        intarg(temperatureCelsius, buffer, 10);
        intarg(SetTemp, buffer2, 10);
        int RPS2  = readPinState(2);
        int RPS3  = readPinState(3);

        LCD_Command(0x80);
        LCD_String("Temp:  ");
        LCD_String(buffer);
        LCD_Command(0xC0);
        LCD_String("SetTemp:  ");
        LCD_String(buffer2);

        if (RPS2 == 0) {
            
            ++SetTemp;
            EEPROM.write(address,SetTemp);// Write the set temperature to EEPROM
            Uart_SendString("Target Increased  \n", 16);
            Uart_SendString("\n", 2);

            _delay_ms(200);
        } else if (RPS3 == 0) {
           
            --SetTemp;
            EEPROM.write(address,SetTemp);// Write the set temperature to EEPROM
            Uart_SendString("Target Decreased  \n", 16);
            Uart_SendString("\n", 2);
            _delay_ms(200);
        }

        

        if (temperatureCelsius > SetTemp) {
            Set_DutyCycle(0, 3);
            Set_DutyCycle(255, 11);
            DIO_SetPinState('b',4,'h');
            DIO_SetPinState('b',5,'l');

        } else {
            Set_DutyCycle(255, 3);
            Set_DutyCycle(0, 11);
            DIO_SetPinState('b',5,'h');
            DIO_SetPinState('b',4,'l');

        }

      // Delay for 0.2 second
    } /* End event loop */

    return 0;
}
