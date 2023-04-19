#include "sys.h"
#include "usart1.h"	  
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




////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
////u16 USART1_RX_STA=0;       //接收状态标记	  
//  
//void USART1_Init(u32 bound)
//{
//	//GPIO端口设置
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
//	
//	//USART1_TX   GPIOA.9
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//USART1_RX	  GPIOA.10初始化
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//Usart1 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//		//USART1 初始化设置
//	USART_InitStructure.USART_BaudRate = bound;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//	USART_Init(USART1, &USART_InitStructure); //初始化串口1
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
//	USART_Cmd(USART1, ENABLE);                    //使能串口1 
//}
//void USART2_Init(u32 bound)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOA时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2
//	//USART2_TX   GPIOA.2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//USART2_RX	  GPIOA.3初始化
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//Usart2 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//		//USART1 初始化设置
//	USART_InitStructure.USART_BaudRate = bound;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//	USART_Init(USART2, &USART_InitStructure); //初始化串口1
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
//	USART_Cmd(USART2, ENABLE);                    //使能串口1 
//}
//void USART3_Init(u32 bound)   
//{  
//    USART_InitTypeDef USART_InitStructure;  
//    NVIC_InitTypeDef NVIC_InitStructure;   
//    GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO  
//    //使能串口的RCC时钟  
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟  
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

//    //串口使用的GPIO口配置  
//    // Configure USART3 Rx (PB.11) as input floating    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
//    GPIO_Init(GPIOB, &GPIO_InitStructure);  

//    // Configure USART3 Tx (PB.10) as alternate function push-pull  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
//    GPIO_Init(GPIOB, &GPIO_InitStructure);  

//    //配置串口  
//    USART_InitStructure.USART_BaudRate = bound;  
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
//    USART_InitStructure.USART_Parity = USART_Parity_No;  
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  


//    // Configure USART3   
//    USART_Init(USART3, &USART_InitStructure);//配置串口3 
//    // Enable USART3 Receive interrupts 使能串口接收中断  
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
//    // Enable the USART3   
//    USART_Cmd(USART3, ENABLE);//使能串口3  

//    //串口中断配置  
//    //Configure the NVIC Preemption Priority Bits     
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  

//    // Enable the USART3 Interrupt   
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;   
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure);       
//} 

