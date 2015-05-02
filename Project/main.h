#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "pneu.h"
#include "bartender.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

void init_lcd(void);
void init_buttons(void);
void Delayms(u32 m);
void enable_joystick(void);
void disable_joystick(void);
