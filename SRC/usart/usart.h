#ifndef _USART_H_
#define _USART_H_
#include "stdio.h"
#include "stm32f10x.h"
#include "stdbool.h"
//#include "sys.h"

#define Usart_Rec_Len			200			//接收最大字节
#define En_Usar_tRx				1			//使能1/禁止0 串口接收

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void USARTx_Interrupt(USART_TypeDef* USARTx,u8 NVIC_PP,u8 NVIC_SP,u8 NVIC_Channel,u8 NVIC_Group);	//串口中断配置
void USARTx_Config(USART_TypeDef* USARTx, u32 pclk2, u32 bound);									//带时钟使能函数
void USART_GPIO_Config(USART_TypeDef* USARTx);														//USART_GPIO配置 顺带串口复位
void SendByte(USART_TypeDef* USARTx,u8 data);
void SendString(USART_TypeDef* USARTx,unsigned char * string);

//是否打开串口中断
//typedef enum {
//	us1 = 0,
//	USART_Interrupt_DISABLE
//} USART_Type;

#endif

