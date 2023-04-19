#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define    USART1_MAX_RECV_LEN		800			//最大接收缓存字节数
#define    USART1_MAX_SEND_LEN		200			//最大发送缓存字节数
#define    USART2_MAX_RECV_LEN		800			//最大接收缓存字节数
#define    USART2_MAX_SEND_LEN		200			//最大发送缓存字节数
#define    USART3_MAX_RECV_LEN		800			//最大接收缓存字节数
#define    USART3_MAX_SEND_LEN		200			//最大发送缓存字节数

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void MyPrintf(u8 uartx, const char*fmt,...);
#endif
