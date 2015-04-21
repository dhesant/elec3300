/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "lcd.h"
#include "pneu.h"

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
uint32_t EXTI_Line;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int drink_stat = 0;

void init_lcd(void) {
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG, ENABLE);
   STM3210E_LCD_Init();
}

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

void Delayms(u32 m) {
  u32 i;
  for(; m != 0; m--)
    for (i=0; i<50000; i++);
}

int main(void) {  
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
  
  /* Configure all unused GPIO port pins in Analog Input mode (floating input
     trigger OFF), this will reduce the power consumption and increase the device
     immunity against EMI/EMC *************************************************/

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOF | 
                         RCC_APB2Periph_GPIOG, ENABLE);  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | 
                         RCC_APB2Periph_GPIOG, DISABLE);  
  
  /* Global initialization ends above.*/
  /* -------------------------------------------------------------------------- */

  init_valves();
  init_lcd();
  write_lcd(1);

  while (1) {
    int i;
    for(i = 0; i < 16; i++) {
      set_valve_status(i, 1);
      write_lcd(i % 4);
      Delayms(1000);
    }
    for(i = 0; i < 16; i++) {
      set_valve_status(i, 0);
      write_lcd(i % 4);
      Delayms(1000);
    }
  }
}
