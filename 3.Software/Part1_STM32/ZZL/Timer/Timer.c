#include "stm32f10x.h"
#include "sys.h"
#include "Timer.h"
#include "mlx90614.h"

/*stm32ʹ��72MHz�������psc = 7199 Ԥ��Ƶϵ�� �����ǽ�72MHz��7199+1��Ƶ��10MHz��Ҳ��10000Hz=0.0001s*/
/*���arr = 4999�Զ�װ��ֵҲ���ǵ���4999+1��*0.0001 �����*/

float fTbody ;
sMlx90614Typ sMlx ;
float BODY;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	TIM_TimeBaseInitStrue.TIM_Period        = arr;                           //�Զ�װ��ֵ
	TIM_TimeBaseInitStrue.TIM_Prescaler     = psc;                           //Ԥ��Ƶϵ��
	TIM_TimeBaseInitStrue.TIM_CounterMode   = TIM_CounterMode_Up;            //���ϼ�����ģʽ
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                                 //ʹ��ָ����TIM3�ж�,��������ж�
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						 //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 				 //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  					 //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							 //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  										 //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx		
}

void TIM3_IRQHandler(void)  												 	   	    //TIM3??
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  				                //??TIM3????????
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  						            //??TIMx?????? 

		ucMLX90614_GetTA(&sMlx);
		ucMLX90614_GetTO1(&sMlx);
		fTbody = ucMLX90614_TempComp(&sMlx);
		BODY = win(fTbody);
	}
}