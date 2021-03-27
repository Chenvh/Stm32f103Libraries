#include "gpio.h"
#include "delay.h"
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

//void delay_ms(uint32_t time) {
//	while(time) time--;
//}

int main(void)
{
	GPIO_Config(PB, 5, OUT_PP, _50MHZ);
	GPIO_Config(PE, 5, OUT_PP, _50MHZ);
	delay_init();
	LED1 = 1;
	LED0 = 1;
	while(1) {
		LED();

	}
	

}

