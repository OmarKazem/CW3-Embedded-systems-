#ifndef __LCD__
#define __LCD__ 

void LCD_Command(unsigned char cmnd);
void LCD_Char (unsigned char char_data);
void LCD_Init (void);
void LCD_String (char *str);


#endif /*__LCD__*/
