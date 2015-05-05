// includes
#include "main.h"
#include "stdio.h"
#include "string.h"

// private definitions
#define TEQUILA 1
#define ORANGE 2
#define GRENADINE 3
#define VODKA 4
#define SODA 0

// private variables
char* drink[4] = { "Tequila Sunrise", "The Gilbert", "Pink Polar Bear", "Screwdriver" };
int MAX_WEIGHT=250;

void write_lcd(int selected) {
  LCD_Clear();
  LCD_DrawString(0, 8, (u8*)drink[0], strlen(drink[0]));
  LCD_DrawString(2, 8, (u8*)drink[1], strlen(drink[1]));
  LCD_DrawString(4, 8, (u8*)drink[2], strlen(drink[2]));
  LCD_DrawString(6, 8, (u8*)drink[3], strlen(drink[3]));
  LCD_DrawChar(2*selected, 0, '-');  
}

void fill_glass(int sel) {
  disable_joystick(); // Disable user input

  LCD_Clear();
  LCD_DrawString(2, 8, (u8*)"Filling...", 10);
  LCD_DrawString(4, 8, (u8*)drink[sel], strlen(drink[sel]));;

  set_all_valves(0); // Safety check; close all valves
  
  int empty_weight = get_weight();
  
  switch(sel) {
  case 0:  // Tequila Sunrise; 30.75% Tequila, 61.5% Orange Juice, 7.75% Grenadine
    LCD_DrawChar(0, 0, '1');
    fill_liquid(TEQUILA, (empty_weight + 0.3075*MAX_WEIGHT));
    LCD_DrawChar(0, 0, '2');
    fill_liquid(ORANGE, (empty_weight + 0.9225*MAX_WEIGHT));
    LCD_DrawChar(0, 0, '3');
    fill_liquid(GRENADINE, (empty_weight + MAX_WEIGHT));
    break;
  case 1: // The Gilbert; 58% Vodka, 30% Soda, 12% Orange Juice
    LCD_DrawChar(0, 0, '1');
    fill_liquid(VODKA, (empty_weight + 0.58*MAX_WEIGHT));
    LCD_DrawChar(0, 0, '2');
    fill_liquid(SODA, (empty_weight + 0.88*MAX_WEIGHT));
    LCD_DrawChar(0, 0, '3');
    fill_liquid(ORANGE, (empty_weight + MAX_WEIGHT));
    break;
  case 2: // Pink Polar Bear; 58% Vodka, 42% Grenadine
    LCD_DrawChar(0, 0, '1');
    fill_liquid(VODKA, (empty_weight + 0.58*MAX_WEIGHT));
    LCD_DrawChar(0, 0, '2');
    fill_liquid(GRENADINE, (empty_weight + MAX_WEIGHT));
    break;
  case 3: // Screwdriver; 48% Vodka, 52% Orange Juice
    LCD_DrawChar(0, 0, '1');
    fill_liquid(VODKA, (empty_weight + 0.48*MAX_WEIGHT));
    LCD_DrawChar(0, 15*8, '2');
    fill_liquid(ORANGE, (empty_weight + MAX_WEIGHT));
    break;
  default:
    break;
  }

  set_all_valves(0); // Safety check; close all valves

  LCD_Clear();
  LCD_DrawString(1, 8, (u8*)"Enjoy...", 8);
  LCD_DrawString(3, 8, (u8*)"Press Joystick", 14);
  LCD_DrawString(5, 8, (u8*)"for new drink", 13);

  enable_joystick(); // Enable user input
}

void fill_glass_debug(int sel) {
  char* msg = "Filling...";
  char* msg2;
  int w;

  while (1) {
    w = get_average(100);
    sprintf(msg2, "%d", w);
    LCD_Clear();
    LCD_DrawString(2, 16, (u8*)msg, 10);
    LCD_DrawString(4, 16, (u8*)msg2, strlen(msg2));
    Delayms(100);
  }
}

void init_weight(void) {
  // Init TypeDefs
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  // Init PA.1 as ADC Input
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Init ADC
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
  ADC_Cmd(ADC1, ENABLE);
  
  // Calibrate ADC
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
}

int get_raw(void) {
    int raw;
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    raw = ADC_GetConversionValue(ADC1);
    return raw;
}

int get_weight(void) {
    int raw;    
    raw = get_raw();
    return raw/1.4686;
}

int get_average(int n) {
  int i;
  int val;
  
  val = get_weight();

  for(i = 0; i < n; i++) {
    val += get_weight();
    val = val >> 1;
    Delayms(1);
  }

  return val;  
}

void fill_liquid(int valve, int target_weight) {
  set_valve_status(valve, 1);
  while(get_average(10) <= target_weight);
  set_valve_status(valve, 0);
}
