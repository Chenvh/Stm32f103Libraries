#include "gpio.h"


//����GPIOʱ��
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


//GPIO����
void GPIO_Config(GPIO_Type Px , uint8_t Pin, GPIO_MODE_Type Mode, GPIO_SPEED_Type Speed)
{
	uint32_t targetMode = Mode + Speed;
	
	//GPIO_CLOCK_ON(Px);	//ʱ��ʹ��
	
	//��������
	if( Mode == IPU)
	{
		((GPIO_TypeDef *) Px )->ODR |= ( 1 << Pin);
	}		
	
	
	//��������
	if( Mode == IPD)
	{
		((GPIO_TypeDef *) Px )->ODR &= ~( 1 << Pin);
	}	
	
	//���� GPIO->CRH	�Ĵ���
	if(Pin > 7) 
	{
		Pin = (Pin - 8) * 4;
		targetMode <<= Pin;
		( (GPIO_TypeDef *) Px )->CRH &= ( 0xfffffff0 << Pin);
		( (GPIO_TypeDef *) Px )->CRH |= targetMode;		
	}
	else 	//���� GPIO->CRL �Ĵ���
	{
		Pin *= 4;
		targetMode <<= Pin;
		( (GPIO_TypeDef *) Px )->CRL &= ( 0xfffffff0 << Pin);
		( (GPIO_TypeDef *) Px )->CRL |= targetMode;
	}
}








