#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define    USART1_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART1_MAX_SEND_LEN		200			//����ͻ����ֽ���
#define    USART2_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART2_MAX_SEND_LEN		200			//����ͻ����ֽ���
#define    USART3_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART3_MAX_SEND_LEN		200			//����ͻ����ֽ���

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void MyPrintf(u8 uartx, const char*fmt,...);
#endif
