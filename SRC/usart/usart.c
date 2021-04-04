#include "usart.h"
#include "sys.h"


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);	//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 



//USART_GPIO配置 顺带串口复位
void USART_GPIO_Config(USART_TypeDef* USARTx)
{
	if(USARTx == USART1)
	{
		//时钟使能
		RCC->APB2ENR |= 1 << 2;		//GPIOA
		RCC->APB2ENR |= 1 << 14;	//USART1
		
		//GPIO A9 A10 配置
		GPIOA->CRH &= 0xFFFFF00F;	//GPIOA.9 GPIOA.10 配置位 清零
		GPIOA->CRH |= 0x000004B0;	//PA10: 01 00  PA9: 10 11		//浮空输入 复用推挽输出
		
		//串口复位/停止复位
		RCC->APB2RSTR |= 1 << 14;
		RCC->APB2RSTR &= ~( 1 << 14);
	}
	
	if(USARTx == USART2)
	{
		RCC->APB2ENR |= 1 << 2;		//GPIOA
		RCC->APB1ENR |= 1 << 17;	//USART2
		//GPIO A2 A3 配置
		GPIOA->CRL &= 0xFFFFF00F;	//GPIOA.3 GPIOA.2 配置位 清零
		GPIOA->CRL |= 0x000004B0;	//PA3: 01 00  PA2: 10 11		//浮空输入 复用推挽输出		
		
		//串口复位/停止复位
		RCC->APB1RSTR |= 1 << 17;
		RCC->APB1RSTR &= ~( 1 << 17);		
	}

	if(USARTx == USART3)
	{
		RCC->APB2ENR |= 1 << 3;		//GPIOB
		RCC->APB1ENR |= 1 << 18;	//USART2
		//GPIO B11 B10 配置
		GPIOA->CRH &= 0xFFFF00FF;	//GPIOB.11 GPIOB.10 配置位 清零
		GPIOA->CRH |= 0x00004B00;	//PB11: 01 00  PB10: 10 11		//浮空输入 复用推挽输出		

		//串口复位/停止复位
		RCC->APB2RSTR |= 1 << 18;
		RCC->APB2RSTR &= ~( 1 << 18);		
	}	
	
}


//串口工作配置
void USARTx_Interrupt(USART_TypeDef* USARTx,u8 NVIC_PP,u8 NVIC_SP,u8 NVIC_Channel,u8 NVIC_Group)
{
	//字长：8位，偶校验，无硬件数据流控制，无接收中断
	USARTx->CR1 |= 1 << 5;
	MY_NVIC_Init(NVIC_PP,NVIC_SP,NVIC_Channel,NVIC_Group);
}

/*
*		USARTDIV = fpCLK2 / 16 / 波特率
*/

void USARTx_Config(USART_TypeDef* USARTx, u32 pclk2, u32 bound) 
{
	float temp;
	u16 USARTDIV;		//整数部分
	u16 DIV_Fraction;	//小数部分 = ( temp - USARTDIV) *16
	//USARTDIV 计算
	temp = (float)(pclk2 * 1000000) / ( 16 * bound);
	USARTDIV = temp;
	DIV_Fraction = ( temp - USARTDIV ) * 16;
	
	USARTDIV <<= 4;
	USARTDIV += DIV_Fraction;
	
	USART_GPIO_Config(USARTx);
	
		
	//USARTx->BRR 串口波特率配置
	USARTx->BRR = USARTDIV;
	
	//字长：8位，偶校验，无硬件数据流控制，无接收中断
	USARTx->CR1 = 0x200C;	
		
}

//发送一个字节
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



//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void USART1_IRQHandler(void)
{
	u8 res;	
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	}
} 
