#include "gpio.h"


//开启GPIO时钟
void GPIO_CLOCK_ON(GPIO_Type Px) 
{
	
	switch(Px) {
		case PA: RCC->APB2ENR |= 1 << 2; break;
		case PB: RCC->APB2ENR |= 1 << 3; break;
		case PC: RCC->APB2ENR |= 1 << 4; break;
		case PD: RCC->APB2ENR |= 1 << 5; break;
		case PE: RCC->APB2ENR |= 1 << 6; break;
		case PF: RCC->APB2ENR |= 1 << 7; break;
		case PG: RCC->APB2ENR |= 1 << 8; break;		
		default: break;
	}
	
}


//GPIO配置
void GPIO_Config(GPIO_Type Px , uint8_t Pin, GPIO_MODE_Type Mode, GPIO_SPEED_Type Speed)
{
	uint32_t targetMode = Mode + Speed;
	
	//GPIO_CLOCK_ON(Px);	//时钟使能
	
	//上拉输入
	if( Mode == IPU)
	{
		((GPIO_TypeDef *) Px )->ODR |= ( 1 << Pin);
	}		
	
	
	//下拉输入
	if( Mode == IPD)
	{
		((GPIO_TypeDef *) Px )->ODR &= ~( 1 << Pin);
	}	
	
	//操作 GPIO->CRH	寄存器
	if(Pin > 7) 
	{
		Pin = (Pin - 8) * 4;
		targetMode <<= Pin;
		( (GPIO_TypeDef *) Px )->CRH &= ( 0xfffffff0 << Pin);
		( (GPIO_TypeDef *) Px )->CRH |= targetMode;		
	}
	else 	//操作 GPIO->CRL 寄存器
	{
		Pin *= 4;
		targetMode <<= Pin;
		( (GPIO_TypeDef *) Px )->CRL &= ( 0xfffffff0 << Pin);
		( (GPIO_TypeDef *) Px )->CRL |= targetMode;
	}
}








