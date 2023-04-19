#include "sys.h"
#include "usart1.h"	  
#include "stdarg.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
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
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);//�ȴ����ͽ���	
		}
		else if(uartNum==2)
		{
			USART_SendData(USART2,*(ucBuffer+i));	
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);//�ȴ����ͽ���				
		}
		else if(uartNum==3)
		{
			USART_SendData(USART3,*(ucBuffer+i));	
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!=SET);//�ȴ����ͽ���				
		}
	}
}
//u8 uartx ѡ��Ҫ���ĸ����ڷ���
void MyPrintf(u8 uartx, const char*fmt,...)			
{
	//������Ϊָ�봴��������ռ䣬һ��100�ֽھ͹��ˣ����ݸ�����Ҫ�����ԼӴ���߸�С
	#define PRINTF_NUM  100
	int ret;
	char *pinh;
	char temp[PRINTF_NUM];//�������Ҷ�����100����ô��MyPrintf���������ôһ�ξͲ�Ҫ����100�ֽڣ�������ˣ��͸ļӴ�һ��PRINTF_NUM
	va_list ap; 
	pinh= temp;   
	va_start(ap,fmt);
	ret=vsprintf(pinh,fmt,ap);//ret��ֵ��Ҫ���͵����ֽ��� ��int�� ע�ⳤ�Ȳ�Ҫ����65535
	UsartNSendForN(uartx,(u8*)pinh,ret);	
	va_end(ap);
}




////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ
////u16 USART1_RX_STA=0;       //����״̬���	  
//  
//void USART1_Init(u32 bound)
//{
//	//GPIO�˿�����
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
//	
//	//USART1_TX   GPIOA.9
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//USART1_RX	  GPIOA.10��ʼ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//Usart1 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//	
//		//USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
//	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
//}
//void USART2_Init(u32 bound)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOAʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2
//	//USART2_TX   GPIOA.2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//USART2_RX	  GPIOA.3��ʼ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//Usart2 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//	
//		//USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
//	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 
//}
//void USART3_Init(u32 bound)   
//{  
//    USART_InitTypeDef USART_InitStructure;  
//    NVIC_InitTypeDef NVIC_InitStructure;   
//    GPIO_InitTypeDef GPIO_InitStructure;    //����һ���ṹ�������������ʼ��GPIO  
//    //ʹ�ܴ��ڵ�RCCʱ��  
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //ʹ��UART3����GPIOB��ʱ��  
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

//    //����ʹ�õ�GPIO������  
//    // Configure USART3 Rx (PB.11) as input floating    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
//    GPIO_Init(GPIOB, &GPIO_InitStructure);  

//    // Configure USART3 Tx (PB.10) as alternate function push-pull  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
//    GPIO_Init(GPIOB, &GPIO_InitStructure);  

//    //���ô���  
//    USART_InitStructure.USART_BaudRate = bound;  
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;  
//    USART_InitStructure.USART_Parity = USART_Parity_No;  
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  


//    // Configure USART3   
//    USART_Init(USART3, &USART_InitStructure);//���ô���3 
//    // Enable USART3 Receive interrupts ʹ�ܴ��ڽ����ж�  
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
//    // Enable the USART3   
//    USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3  

//    //�����ж�����  
//    //Configure the NVIC Preemption Priority Bits     
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  

//    // Enable the USART3 Interrupt   
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;   
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure);       
//} 

