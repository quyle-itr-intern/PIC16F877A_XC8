#ifndef _LCD_H_
#define _LCD_H_

#include "main.h"

void LCD_Cmd(unsigned char cmd);
void LCD_Init();
void LCD_Char(char c);    // Current position
void LCD_String(char *str);
void LCD_GotoCursor(unsigned char row, unsigned char col);
void LCD_GotoCursor_Char(unsigned char row, unsigned char col, char c);       // Xuat ra man hinh LCD 1 ky tu, ngay tai vi tri dong row, cot col
void LCD_GotoCursor_String(unsigned char row, unsigned char col, char *str);  // Xuat ra man hinh LCD 1 ky tu, ngay tai vi tri dong row, cot col

#endif
