#include "stm32f10x.h"
#include "sys.h"
#include "Timer.h"
#include "mlx90614.h"

/*stm32使用72MHz晶振，如果psc = 7199 预分频系数 计算是将72MHz分7199+1分频到10MHz，也即10000Hz=0.0001s*/
/*如果arr = 4999自动装载值也就是到（4999+1）*0.0001 则归零*/

float fTbody ;
sMlx90614Typ sMlx ;
float BODY;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	TIM_TimeBaseInitStrue.TIM_Period        = arr;                           //自动装载值
	TIM_TimeBaseInitStrue.TIM_Prescaler     = psc;                           //预分频系数
	TIM_TimeBaseInitStrue.TIM_CounterMode   = TIM_CounterMode_Up;            //向上技计数模式
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                                 //使能指定的TIM3中断,允许更新中断
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						 //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 				 //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  					 //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							 //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  										 //初始化NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx		
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