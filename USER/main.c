#include "gpio.h"
#include "delay.h"
#include "usart.h"
#define LED1 PBout(5)
#define LED0 PEout(5)

void LED() {
	
	LED1 = 0;	
	LED0 = 1;
	delay_ms(1000);
	
	LED1 = 1;
	LED0 = 0;	
	delay_ms(1000);
	
}


void usart_init()
{
	USARTx_Config(USART1,72, 115200);
	USARTx_Interrupt(USART1,3, 3, USART1_IRQn, 2);
}



int main(void)
{
	GPIO_CLOCK_ON(PB);
	GPIO_CLOCK_ON(PE);
	GPIO_Config(PB, 5, OUT_PP, _50MHZ);
	GPIO_Config(PE, 5, OUT_PP, _50MHZ);
	Stm32_Clock_Init(9);
	delay_init(72);
	usart_init();
	LED1 = 1;
	LED0 = 1;
	while(1) {
		//LED();
		SendByte(USART1, 'T');
		SendByte(USART1, 13);
		SendByte(USART1, 10);
		delay_ms(1000);
	}

}

