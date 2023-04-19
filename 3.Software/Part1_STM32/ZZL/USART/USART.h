#ifndef _USART_H
#define _USART_H

#include "stdio.h"	
#include "sys.h"

#define    USART1_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART1_MAX_SEND_LEN		200			//����ͻ����ֽ���
#define    USART2_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART2_MAX_SEND_LEN		200			//����ͻ����ֽ���
#define    USART3_MAX_RECV_LEN		800			//�����ջ����ֽ���
#define    USART3_MAX_SEND_LEN		200			//����ͻ����ֽ���


void My_USART1_Init(void);
void send_END();
void MyPrintf(u8 uartx, const char*fmt,...);
void UsartNSendForN(u8 uartNum,u8 * ucBuffer, int ulSize);

#endif
