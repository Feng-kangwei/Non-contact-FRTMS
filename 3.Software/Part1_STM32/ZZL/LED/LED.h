#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"
#define LED PCout(13)

void LED_Int(GPIO_TypeDef* GPIOx,uint16_t GPIO_PinX,uint32_t time);
 
#endif

