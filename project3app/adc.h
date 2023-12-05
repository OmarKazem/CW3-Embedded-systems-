#ifndef __ADC__
#define __ADC__

void Adc_Init(void);

unsigned short Adc_ReadChannel(unsigned char channel);

char* intarg(int num, char* str, int base);

#endif