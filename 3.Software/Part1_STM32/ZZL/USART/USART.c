#include "stm32f10x.h"
#include "USART.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
#include "delay.h"
#include "oled.h"
#include "stdarg.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
void UsartNSendForN(u8 uartNum,u8 * ucBuffer, int ulSize)
{
	u8 i;
	for(i=0;i<ulSize;i++)
	{
		if(uartNum==1)
		{
			USART_SendData(USART1,*(ucBuffer+i));
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);//等待发送结束	
		}
		else if(uartNum==2)
		{
			USART_SendData(USART2,*(ucBuffer+i));	
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);//等待发送结束				
		}
		else if(uartNum==3)
		{
			USART_SendData(USART3,*(ucBuffer+i));	
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!=SET);//等待发送结束				
		}
	}
}
//u8 uartx 选择要用哪个串口发送
void MyPrintf(u8 uartx, const char*fmt,...)			
{
	//在这里为指针创建个缓存空间，一般100字节就够了，根据个人需要，可以加大或者改小
	#define PRINTF_NUM  100
	int ret;
	char *pinh;
	char temp[PRINTF_NUM];//在这里我定义了100，那么用MyPrintf来输出，那么一次就不要超过100字节，如果超了，就改加大一下PRINTF_NUM
	va_list ap; 
	pinh= temp;   
	va_start(ap,fmt);
	ret=vsprintf(pinh,fmt,ap);//ret的值是要发送的总字节数 是int型 注意长度不要超过65535
	UsartNSendForN(uartx,(u8*)pinh,ret);	
	va_end(ap);
}


unsigned char end[3] = {0xff,0xff,0xff};
char ByteSend[3];
char txt1[16];


void USART_SendStr(USART_TypeDef* USARTx , u8 *str)
{  
    while(*str!='\0')
    {
        USART_SendData(USARTx,*str);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)!= SET);  
        str++;
    }
}

void My_USART1_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef  USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
	USART_Cmd(USART1,ENABLE);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
}


void USART1_IRQHandler()
{
	u8 res1;
	char txt1[16];

	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		res1 = USART_ReceiveData(USART1); 
		if(res1 == '1')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"FKW\"%s",end);
			MyPrintf(1,"t2.txt=\"是\"%s",end);
		}
		if(res1 == '2')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"FKW\"%s",end);
			MyPrintf(1,"t2.txt=\"否\"%s",end);
		}
		if(res1 == '3')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"WXY\"%s",end);
			MyPrintf(1,"t2.txt=\"是\"%s",end);
		}
		if(res1 == '4')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"WXY\"%s",end);
			MyPrintf(1,"t2.txt=\"否\"%s",end);
		}
		if(res1 == '5')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"ZZL\"%s",end);
			MyPrintf(1,"t2.txt=\"是\"%s",end);
		}
		if(res1 == '6')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"ZZL\"%s",end);
			MyPrintf(1,"t2.txt=\"否\"%s",end);
		}
		if(res1 == '7')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"ZZL\"%s",end);
			MyPrintf(1,"t2.txt=\"是\"%s",end);
		}
		if(res1 == '8')
		{
			LED = ~LED;
			delay_ms(500);
			MyPrintf(1,"t0.txt=\"ZZL\"%s",end);
			MyPrintf(1,"t2.txt=\"否\"%s",end);
		}
	}
}

