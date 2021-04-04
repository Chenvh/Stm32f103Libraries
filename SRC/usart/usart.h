#ifndef _USART_H_
#define _USART_H_
#include "stdio.h"
#include "stm32f10x.h"
#include "stdbool.h"
//#include "sys.h"

#define Usart_Rec_Len			200			//��������ֽ�
#define En_Usar_tRx				1			//ʹ��1/��ֹ0 ���ڽ���

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void USARTx_Interrupt(USART_TypeDef* USARTx,u8 NVIC_PP,u8 NVIC_SP,u8 NVIC_Channel,u8 NVIC_Group);	//�����ж�����
void USARTx_Config(USART_TypeDef* USARTx, u32 pclk2, u32 bound);									//��ʱ��ʹ�ܺ���
void USART_GPIO_Config(USART_TypeDef* USARTx);														//USART_GPIO���� ˳�����ڸ�λ
void SendByte(USART_TypeDef* USARTx,u8 data);
void SendString(USART_TypeDef* USARTx,unsigned char * string);

//�Ƿ�򿪴����ж�
//typedef enum {
//	us1 = 0,
//	USART_Interrupt_DISABLE
//} USART_Type;

#endif

