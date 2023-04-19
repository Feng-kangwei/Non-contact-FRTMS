#include "key.h"
#include "led.h"


float  valuable=0;
float  Up_limit = 46;
float  Low_limit = 30;
/**********************************************************************************
 * ��������KEY_Init
 * ����  ��������ʼ������,�͵�ƽ��Ч��
 **********************************************************************************/
void KEY_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure ;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE) ;		    //ʹ��PORTBʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;					//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 					//���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure) ;			    			//��ʼ��KEY0-KEY1
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;						//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 					//���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure) ;			    			//��ʼ��KEY0-KEY1
}

void beep()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(100);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(100);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(4000);
	
}
	
	
void key_EXIT()
{
	EXTI_InitTypeDef   EXTI_InitStrue;
	NVIC_InitTypeDef   NVIC_InitStrue; 

	/*	�������ù��� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 

	/* ����IO�����ж���ӳ���ϵ   ","ǰ�������ӹ�ϵ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);


	/*	��ʼ�������ж�  ���ô�������  */
	EXTI_InitStrue.EXTI_Line=EXTI_Line6;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	
	
	EXTI_InitStrue.EXTI_Line=EXTI_Line7;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	


	/* �����жϷ���   ʹ���ж�  */
	NVIC_InitStrue.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue);		
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		delay_ms(100);
		if(KEYB8 == 0&&KEYB6==0&&KEYB5==0)
		{
			valuable -= 0.1;
			LED=~LED;
		}
		if(KEYB8 == 0&&KEYB6==0&&KEYB4==0)
		{
			valuable -= 1;
			LED=~LED;
		}
		if(KEYB9==0&&KEYB5 == 0&&KEYB6==0)
		{
			Up_limit -= 1;
			LED=~LED;
		}
		if(KEYB9 == 0&&KEYB4==0&&KEYB6==0)
		{
			Low_limit -= 1;
			LED=~LED;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
    if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
		delay_ms(100);
		if(KEYB8 == 0&&KEYB7==0&&KEYB5==0)
		{
			valuable += 0.1;
			LED=~LED;
		}
		if(KEYB8 == 0&&KEYB7==0&&KEYB4==0)
		{
			valuable += 1;
			LED=~LED;
		}
		if(KEYB9 == 0&&KEYB5==0&&KEYB7==0)
		{
			Up_limit += 1;
			LED=~LED;
		}
		if(KEYB9 == 0&&KEYB4==0&&KEYB7==0)
		{
			Low_limit += 1;
			LED=~LED;
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}


