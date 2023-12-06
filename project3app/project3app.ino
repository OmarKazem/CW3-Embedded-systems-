#include <util/delay.h>
#include <stdlib.h>
#include <EEPROM.h>
#include "adc.h"
#include "lcd.h"
#include "Dio.h"
#include "pwm.h"
#include "Uart.h"

// Function prototypes and global variables declarations

int main(void) {
    
    unsigned short adc_reading;
    unsigned char buffer[4];    // Buffer for converting numerical values to strings
    unsigned char buffer2[4];   // Another buffer for numerical to string conversion
    Adc_Init();
    LCD_Init();
    PWM_Init();
    Uart_Init();
    int SetTemp;                 // Variable to store the set temperature
    int address = 0;             // EEPROM address for storing set temperature
    int speed = 190;             // Default speed value
    
    while (1) {
        // Read the set temperature from EEPROM
        SetTemp = EEPROM.read(address);

        // Read temperature from ADC
        adc_reading = Adc_ReadChannel(1);
        float temperatureCelsius = ((adc_reading / 1024.0) * 5 * 100.0);
        
        // Convert numerical values to strings for LCD display and UART transmission
        intarg(temperatureCelsius, buffer, 10);
        intarg(SetTemp, buffer2, 10);
        
        // Read state of switches
        int RPS2  = readPinState('c',2);
        int RPS3  = readPinState('c',3);
        int RPS4  = readPinState('c',4);
        int RPS5  = readPinState('d',2);

        
        
        // Display temperature and set temperature on LCD
        LCD_Command(0x80);
        LCD_String("Temp: ");
        LCD_String(buffer);
        LCD_Command(0xC0);
        LCD_String("SetTemp: ");
        LCD_String(buffer2);

        // Check if the "Increase SetTemp" button is pressed
        if (RPS2 == 0) {
            ++SetTemp;
            EEPROM.write(address, SetTemp);  // Write the updated set temperature to EEPROM
            Uart_SendString("Target Increased", 16);
            Uart_SendString("\n", 2);
            _delay_ms(200);
        } 
        // Check if the "Decrease SetTemp" button is pressed
        else if (RPS3 == 0) {
            --SetTemp;
            EEPROM.write(address, SetTemp);  // Write the updated set temperature to EEPROM
            Uart_SendString("Target Decreased", 16);
            Uart_SendString("\n", 2);
            _delay_ms(200);
        }

        // Check if the "Increase Speed" button is pressed
        if (RPS4 == 0) {
            speed = 255;
            Uart_SendString("Speed increased  \n", 16);
            Uart_SendString("\n", 2);
        }
        // Check if the "Decrease Speed" button is pressed
        else if (RPS5 == 0) {
            speed = 190;
            Uart_SendString("Speed decreased  \n", 16);
            Uart_SendString("\n", 2);
        }

        // Control the heating/cooling system based on temperature and set temperature
        if (temperatureCelsius > SetTemp) {
            // Cooling mode
            Set_DutyCycle(0, 3);
            Set_DutyCycle(speed, 11);
            DIO_SetPinState('b', 4, 'l');
            DIO_SetPinState('b', 5, 'h');
            Uart_SendString("System cooling", 16);
            Uart_SendString("\n", 2);
        } else {
            // Heating mode
            Set_DutyCycle(speed, 3);
            Set_DutyCycle(0, 11);
            DIO_SetPinState('b', 5, 'l');
            DIO_SetPinState('b', 4, 'h');
            Uart_SendString("System heating", 16);
            Uart_SendString("\n", 2);
        }
    } /* End event loop */

    return 0;
}
