#include <util/delay.h>
#include <stdlib.h>
#include <EEPROM.h>
#include "adc.h"
#include "lcd.h"
#include "Dio.h"
#include "pwm.h"
#include "Uart.h"

// Function to toggle the system power state
void toggleSystemPower(unsigned char port,int buttonPin, int *powerState) {
    // Toggle the power state when the button is pressed
    *powerState = (*powerState == HIGH) ? LOW : HIGH;
}


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
    int powerState = LOW;        // Initial power state

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
        int RPS2  = readPinState('c', 2);
        int RPS3  = readPinState('c', 3);
        int RPS4  = readPinState('c', 4);
        int RPS5  = readPinState('d', 2);

        // Read state of the power button
        int powerButtonState = readPinState('c', 5);

        // Toggle the system power if the power button is pressed
        if (powerButtonState == HIGH) {
            toggleSystemPower('c', 5, &powerState);
            _delay_ms(200);  // Debounce delay
        }

        // Display temperature and set temperature on LCD
        LCD_Command(0x80);
        LCD_String("Temp: ");
        LCD_String(buffer);
        LCD_Command(0xC0);
        LCD_String("SetTemp: ");
        LCD_String(buffer2);

        // Check if the "Increase SetTemp" button is pressed
        if (RPS2 == HIGH) {
            ++SetTemp;
            EEPROM.write(address, SetTemp);  // Write the updated set temperature to EEPROM
            Uart_SendString("Target Increased", 16);
            Uart_SendString("\n", 2);
            _delay_ms(200);
        } 
        // Check if the "Decrease SetTemp" button is pressed
        else if (RPS3 == HIGH) {
            --SetTemp;
            EEPROM.write(address, SetTemp);  // Write the updated set temperature to EEPROM
            Uart_SendString("Target Decreased", 16);
            Uart_SendString("\n", 2);
            _delay_ms(200);
        }

        // Check if the "Increase Speed" button is pressed
        if (RPS4 == HIGH) {
            speed = 255;
            Uart_SendString("Speed increased  \n", 16);
            Uart_SendString("\n", 2);
        }
        // Check if the "Decrease Speed" button is pressed
        else if (RPS5 == HIGH) {
            speed = 190;
            Uart_SendString("Speed decreased  \n", 16);
            Uart_SendString("\n", 2);
        }

        // Control the heating/cooling system based on temperature and set temperature
        if (powerState == HIGH) {
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
        } else {
            // System is turned off
            Set_DutyCycle(0, 3);
            Set_DutyCycle(0, 11);
            Uart_SendString("System turned off", 16);
            Uart_SendString("\n", 2);
            DIO_SetPinState('b', 5, 'l');
            DIO_SetPinState('b', 4, 'l');
        }
    } /* End event loop */

    return 0;
}


