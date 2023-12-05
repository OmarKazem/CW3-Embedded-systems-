#ifndef __DIO__
#define __DIO__

#define DIO_DIR_OUTPUT          1
#define DIO_DIR_INPUT           0
#define DIO_STATE_HIGH          1 
#define DIO_STATE_LOW           0

#define PINB_REG                   0x23
#define DDRB_REG                   0x24
#define PORTB_REG                  0x25
#define PINC_REG                   0x26
#define DDRC_REG                   0x27
#define PORTC_REG                  0x28
#define PIND_REG                   0x29
#define DDRD_REG                   0x2A
#define PORTD_REG                  0x2B


#define PIN_B                   (volatile uint8_t *)(PINB_REG)
#define DDR_B                   (volatile uint8_t *)(DDRB_REG)
#define PORT_B                  (volatile uint8_t *)(PORTB_REG)
#define PIN_C                   (volatile uint8_t *)(PINC_REG)
#define DDR_C                   (volatile uint8_t *)(DDRC_REG)
#define PORT_C                  (volatile uint8_t *)(PORTC_REG)
#define PIN_D                   (volatile uint8_t *)(PIND_REG)
#define DDR_D                   (volatile uint8_t *)(DDRD_REG)
#define PORT_D                  (volatile uint8_t *)(PORTD_REG)

void DIO_init();

inline void Dio_SetRegisterBit(volatile unsigned char* Register, unsigned char Bit);

inline void Dio_ResetRegisterBit(volatile unsigned char* Register, unsigned char Bit);

inline void Dio_ToggleRegisterBit(volatile unsigned char* Register, unsigned char Bit);

void Dio_SetPinDirection(volatile unsigned char* Register, unsigned char Pin, unsigned char Direction);

void Dio_SetPinState(volatile unsigned char* Register, unsigned char Pin, unsigned char State);

unsigned char Dio_GetPinState(volatile unsigned char* Register, unsigned char Pin);

int readPinState1();

int readPinState2();

#endif /*__DIO__*/
