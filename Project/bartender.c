#include "stm32f10x.h"
#include "lcd.h"
#include "pneu.h"

void write_lcd(int selected) {
  LCD_Clear();
  char* drink1 = "Water";
  char* drink2 = "More Water";
  char* drink3 = "Too Much Water";
  char* drink4 = "Tequila";
  LCD_DrawString(0, 16, (u8*)drink1, 5);
  LCD_DrawString(2, 16, (u8*)drink2, 10);
  LCD_DrawString(4, 16, (u8*)drink3, 14);
  LCD_DrawString(6, 16, (u8*)drink4, 7);
  LCD_DrawChar(2*selected, 0, '@');  
}


void fill_glass(int selected) {
  LCD_Clear();
  char* msg = "Filling...";
  LCD_DrawString(3, 16, (u8*)msg, 10);
  while(1);
}
