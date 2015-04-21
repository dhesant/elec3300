#include "stm32f10x.h"
#include "lcd.h"
#include "pneu.h"

void fill_glass(int selected) {
  LCD_Clear();
  char* msg = "Filling...";
  LCD_DrawString(3, 16, (u8*)msg, 10);
  while(1);
}
