/***************************************************************************************************
* Copyright (C),    //版权
* File name:key.h 	
* Author:Charmander     Version:0.1    Date:2019.1.1    //作者、版本、完成日期（初版） 
* Description:    
* Other:   
* Contact: 228303760（qqname： Obligation） 228303760@qq.com
* TaoBao: 捷睿电子科技
* Function List:   
*			void KEY_Init(void) ; //IO初始化
*			u8 KEY_Scan(u8) ;  	  //按键扫描函数	
* History:    //修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
*      1. Data：
*         Author：Charmander
*          Modification：
***********************************************************************************************************/

#ifndef __KEY_H
#define __KEY_H	 

#include "stm32f10x.h"
#include "delay.h"


#define KEYB3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define KEYB4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define KEYB5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define KEYB6  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define KEYB7  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define KEYB8  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define KEYB9  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define KEYA15  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)

 
#define ON 	 1	    //KEY0按下
#define OFF  0		//没有按键按下

void KEY_Init(void) ;//IO初始化
void key_EXIT();
void beep();


#endif



