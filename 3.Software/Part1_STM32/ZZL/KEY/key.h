/***************************************************************************************************
* Copyright (C),    //��Ȩ
* File name:key.h 	
* Author:Charmander     Version:0.1    Date:2019.1.1    //���ߡ��汾��������ڣ����棩 
* Description:    
* Other:   
* Contact: 228303760��qqname�� Obligation�� 228303760@qq.com
* TaoBao: ����ӿƼ�
* Function List:   
*			void KEY_Init(void) ; //IO��ʼ��
*			u8 KEY_Scan(u8) ;  	  //����ɨ�躯��	
* History:    //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
*      1. Data��
*         Author��Charmander
*          Modification��
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

 
#define ON 	 1	    //KEY0����
#define OFF  0		//û�а�������

void KEY_Init(void) ;//IO��ʼ��
void key_EXIT();
void beep();


#endif



