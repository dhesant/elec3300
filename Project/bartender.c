#include "main.h"
#include "stdio.h"

void write_lcd(int selected) {
  char* drink1 = "Water";
  char* drink2 = "More Water";
  char* drink3 = "Too Much Water";
  char* drink4 = "Tequila";

  LCD_Clear();
  LCD_DrawString(0, 16, (u8*)drink1, 5);
  LCD_DrawString(2, 16, (u8*)drink2, 10);
  LCD_DrawString(4, 16, (u8*)drink3, 14);
  LCD_DrawString(6, 16, (u8*)drink4, 7);
  LCD_DrawChar(2*selected, 0, '-');  
}

void fill_glass(int selected) {
  char* msg = "Filling...";
  char* msg2;
  float w;

  LCD_Clear();
  LCD_DrawString(2, 16, (u8*)msg, 10);
  while (1) {
    w = get_weight();
    sprintf(msg2, "%F", w);
    LCD_DrawString(4, 16, (u8*)msg2, 9);
    Delayms(100);
  }
}

void init_weight(void) {
  // Init TypeDefs
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  // Init PC.4 as ADC Input
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
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

float get_weight(void) {
    int raw;
    float val;
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    raw = ADC_GetConversionValue(ADC1);
    val = 3.3*raw/(float)0xfff;
    val = (val - 0.00967) * 0.9188;
    return val;
}
