#include "usart.h"
#include "sys.h"


#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);	//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 



//USART_GPIO���� ˳�����ڸ�λ
void USART_GPIO_Config(USART_TypeDef* USARTx)
{
	if(USARTx == USART1)
	{
		//ʱ��ʹ��
		RCC->APB2ENR |= 1 << 2;		//GPIOA
		RCC->APB2ENR |= 1 << 14;	//USART1
		
		//GPIO A9 A10 ����
		GPIOA->CRH &= 0xFFFFF00F;	//GPIOA.9 GPIOA.10 ����λ ����
		GPIOA->CRH |= 0x000004B0;	//PA10: 01 00  PA9: 10 11		//�������� �����������
		
		//���ڸ�λ/ֹͣ��λ
		RCC->APB2RSTR |= 1 << 14;
		RCC->APB2RSTR &= ~( 1 << 14);
	}
	
	if(USARTx == USART2)
	{
		RCC->APB2ENR |= 1 << 2;		//GPIOA
		RCC->APB1ENR |= 1 << 17;	//USART2
		//GPIO A2 A3 ����
		GPIOA->CRL &= 0xFFFFF00F;	//GPIOA.3 GPIOA.2 ����λ ����
		GPIOA->CRL |= 0x000004B0;	//PA3: 01 00  PA2: 10 11		//�������� �����������		
		
		//���ڸ�λ/ֹͣ��λ
		RCC->APB1RSTR |= 1 << 17;
		RCC->APB1RSTR &= ~( 1 << 17);		
	}

	if(USARTx == USART3)
	{
		RCC->APB2ENR |= 1 << 3;		//GPIOB
		RCC->APB1ENR |= 1 << 18;	//USART2
		//GPIO B11 B10 ����
		GPIOA->CRH &= 0xFFFF00FF;	//GPIOB.11 GPIOB.10 ����λ ����
		GPIOA->CRH |= 0x00004B00;	//PB11: 01 00  PB10: 10 11		//�������� �����������		

		//���ڸ�λ/ֹͣ��λ
		RCC->APB2RSTR |= 1 << 18;
		RCC->APB2RSTR &= ~( 1 << 18);		
	}	
	
}


//���ڹ�������
void USARTx_Interrupt(USART_TypeDef* USARTx,u8 NVIC_PP,u8 NVIC_SP,u8 NVIC_Channel,u8 NVIC_Group)
{
	//�ֳ���8λ��żУ�飬��Ӳ�����������ƣ��޽����ж�
	USARTx->CR1 |= 1 << 5;
	MY_NVIC_Init(NVIC_PP,NVIC_SP,NVIC_Channel,NVIC_Group);
}

/*
*		USARTDIV = fpCLK2 / 16 / ������
*/

void USARTx_Config(USART_TypeDef* USARTx, u32 pclk2, u32 bound) 
{
	float temp;
	u16 USARTDIV;		//��������
	u16 DIV_Fraction;	//С������ = ( temp - USARTDIV) *16
	//USARTDIV ����
	temp = (float)(pclk2 * 1000000) / ( 16 * bound);
	USARTDIV = temp;
	DIV_Fraction = ( temp - USARTDIV ) * 16;
	
	USARTDIV <<= 4;
	USARTDIV += DIV_Fraction;
	
	USART_GPIO_Config(USARTx);
	
		
	//USARTx->BRR ���ڲ���������
	USARTx->BRR = USARTDIV;
	
	//�ֳ���8λ��żУ�飬��Ӳ�����������ƣ��޽����ж�
	USARTx->CR1 = 0x200C;	
		
}

//����һ���ֽ�
void SendByte(USART_TypeDef* USARTx,u8 data)
{
	USARTx->DR = data;
	while( ( USARTx->SR & 0x40) == 0);
}

void SendString(USART_TypeDef* USARTx,unsigned char * string)
{
	while( string != '\0' )
	{
		USARTx->DR = *string;
		while( ( USARTx->SR & 0x40) == 0);		
		string++;
	}
}



//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART1_IRQHandler(void)
{
	u8 res;	
	if(USART1->SR&(1<<5))	//���յ�����
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	}
} 
