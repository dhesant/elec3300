#include "stm32f10x.h"

GPIO_TypeDef* valve_port[] = {
  GPIOA,
  GPIOA,
  GPIOA,
  GPIOA,
  GPIOA,
  GPIOC,
  GPIOC,
  GPIOB,
  GPIOB,
  GPIOB,
  GPIOF,
  GPIOF,
  GPIOF,
  GPIOF,
  GPIOF,
  GPIOG
};

const uint16_t valve_pin[] = {  
  GPIO_Pin_3,
  GPIO_Pin_4,
  GPIO_Pin_5,
  GPIO_Pin_6,
  GPIO_Pin_7,
  GPIO_Pin_4,
  GPIO_Pin_5,
  GPIO_Pin_0,
  GPIO_Pin_1,
  GPIO_Pin_2,
  GPIO_Pin_11,
  GPIO_Pin_12,
  GPIO_Pin_13,
  GPIO_Pin_14,
  GPIO_Pin_15,
  GPIO_Pin_0
};

bool get_valve_status(int id) {
  return GPIO_ReadOutputDataBit(valve_port[id], valve_pin[id]);
}

void set_valve_status(int id, bool status) {
  if (status) {
    valve_port[id]->BSRR = valve_pin[id];
  }
  else {
    valve_port[id]->BRR = valve_pin[id];
  }
}

void set_all_valves(bool status) {
  int i;
  for(i = 0; i < 16; i++) {
    set_valve_status(i, status);
  }
}

void init_valves(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
    GPIO_Pin_14 | GPIO_Pin_15 ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}
