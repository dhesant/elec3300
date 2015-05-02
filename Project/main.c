/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
uint32_t EXTI_Line;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int drink_stat = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void init_lcd(void) {
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);
   STM3210E_LCD_Init();
}

void init_buttons(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);

  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_ClearITPendingBit(EXTI_Line11);
  EXTI_ClearITPendingBit(EXTI_Line12);
  EXTI_ClearITPendingBit(EXTI_Line13);
  EXTI_ClearITPendingBit(EXTI_Line14);
  EXTI_ClearITPendingBit(EXTI_Line15);
}

void Delayms(u32 m) {
  u32 i;
  for(; m != 0; m--)
    for (i=0; i<50000; i++);
}

void enable_joystick(void) {
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void disable_joystick(void) {
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
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

  init_weight();
  init_valves();
  init_buttons();
  init_lcd();
  write_lcd(0);
  enable_joystick();

  while (1) {
    set_valve_status(3, 1);
    set_valve_status(3, 0);
  }
}
