#ifndef  _stepMotor_H
#define  _stepMotor_H
#include "sys.h"

#define KEYB3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define KEYA3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define KEYB4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define KEYB5 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define KEYB6 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define KEYB7 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

#define KEYA10 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)
#define KEYA12 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define KEYA15 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)

#define PA10_J PAout(10)
#define PA11_J PAout(11)

void PWM_Init_TIM1(u16 psc,u16 per);
void stepkey_Init();
void key_Init();
void key_EXIT();
void lihe();
void zheng();
void fan();
void stop();



#endif



