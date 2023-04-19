#include "EXTI.h"

void stepkey_Init()
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}


void key_Init()
{
	GPIO_InitTypeDef    GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}


void key_EXIT()
{
	EXTI_InitTypeDef   EXTI_InitStrue;
	NVIC_InitTypeDef   NVIC_InitStrue; 

	/*	开启复用功能 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 

//	/* 设置IO口与中断线映射关系   ","前后链连接关系 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);	

	/*	初始化线上中断  设置触发条件  */
//	EXTI_InitStrue.EXTI_Line=EXTI_Line2;
//	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
//	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStrue);

	EXTI_InitStrue.EXTI_Line=EXTI_Line3;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	

	EXTI_InitStrue.EXTI_Line=EXTI_Line4;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	
	
	EXTI_InitStrue.EXTI_Line=EXTI_Line5;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	
//	
//	EXTI_InitStrue.EXTI_Line=EXTI_Line7;
//	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
//	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStrue);	

//	EXTI_InitStrue.EXTI_Line=EXTI_Line10;
//	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
//	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStrue);	

	EXTI_InitStrue.EXTI_Line=EXTI_Line12;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	

	EXTI_InitStrue.EXTI_Line=EXTI_Line15;
	EXTI_InitStrue.EXTI_LineCmd=ENABLE;
	EXTI_InitStrue.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStrue.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStrue);	

	/* 配置中断分组   使能中断  */
	NVIC_InitStrue.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue); 

	NVIC_InitStrue.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue);	

	NVIC_InitStrue.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue);		

	NVIC_InitStrue.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue);			
}

//void EXTI15_10_IRQHandler(void)
//{

//    if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
//	{
//		if(KEYA10 == 0)
//		{
//			if (flag == 0)
//			{
//				TIM_SetCompare1(TIM1,8000);
//				flag = ~flag;
//				LED=~LED;
//			}
//			else
//			{
//				TIM_SetCompare1(TIM1,0);
//				flag = ~flag;
//			}
//		}
//	}
//	EXTI_ClearITPendingBit(EXTI_Line10);
//}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
		if(KEYA12 == 0)
		{
			delay_ms(100);			
			if(KEYA12 == 0)
			{
				PA10_J=~PA10_J;
				LED=~LED;
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line12);


    if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		if(KEYA15 == 0)
		{	
			delay_ms(100);			
			if(KEYA15 == 0)
			{
				PA11_J=~PA11_J;
				LED=~LED;
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line15);
}


void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{	
		if(KEYA3 == 0)
		{
			delay_ms(30);
			while(KEYA3 == 0)
			{
				fan();
				LED=~LED;
			}	
		}		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		delay_ms(100);
		while(KEYB4 == 0)
		{
			zheng();
			LED=~LED;
		}		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		delay_ms(100);
		if(KEYB5 == 0&&flag == 0)
		{
			delay_ms(200);
			pitch_qian = pitch_qian+13;
			flag = 1;
		}
		if(KEYB5 == 0&&flag == 1)
		{
			delay_ms(200);
			pitch_qian = pitch_qian-13;
			flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
	}


}