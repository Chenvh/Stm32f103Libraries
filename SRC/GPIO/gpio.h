#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f10x.h"
#include "stdbool.h"

//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr) 			*((volatile unsigned long *) (addr))
#define BIT_ADDR(addr, bitnum)	MEM_ADDR(BITBAND(addr, bitnum))	

//�ⲿ���߻���ַ 	PERIPH_BASE = 0x40000000
//APB1���߻���ַ		APB1PERIPH_BASE = PERIPH_BASE = 0x40000000
//APB2���߻���ַ		APB2PERIPH_BASE = PERIPH_BASE + 0x10000 = 0x40010000


//IO�ڵ�ַӳ��
//ODR�Ĵ���
#define GPIOA_ODR_Addr		(GPIOA_BASE + 12)	//GPIOA_BASE = APB2PERIPH_BASE + 0x0800, GPIOA_ODR_Addr + 12 = 0x4001080C
#define GPIOB_ODR_Addr		(GPIOB_BASE + 12)	//+0x00c
#define GPIOC_ODR_Addr		(GPIOC_BASE + 12)	//+0x00c
#define GPIOD_ODR_Addr		(GPIOD_BASE + 12)	//+0x00c
#define GPIOE_ODR_Addr		(GPIOE_BASE + 12)	//+0x00c
#define GPIOF_ODR_Addr		(GPIOF_BASE + 12)	//+0x00c
#define GPIOG_ODR_Addr		(GPIOG_BASE + 12)	//+0x00c

//IDR�Ĵ���
#define GPIOA_IDR_Addr		(GPIOA_BASE + 8)	//GPIOA_BASE = APB2PERIPH_BASE + 0x0800, GPIOA_IDR_Addr + 8 = 0x40010808
#define GPIOB_IDR_Addr		(GPIOB_BASE + 8)	//+0x00c
#define GPIOC_IDR_Addr		(GPIOC_BASE + 8)	//+0x00c
#define GPIOD_IDR_Addr		(GPIOD_BASE + 8)	//+0x00c
#define GPIOE_IDR_Addr		(GPIOE_BASE + 8)	//+0x00c
#define GPIOF_IDR_Addr		(GPIOF_BASE + 8)	//+0x00c
#define GPIOG_IDR_Addr		(GPIOG_BASE + 8)	//+0x00c



//IO �������
#define PAout(n)	BIT_ADDR(GPIOA_ODR_Addr, n)	//���
#define PAin(n)		BIT_ADDR(GPIOA_IDR_Addr, n)	//����

#define PBout(n)	BIT_ADDR(GPIOB_ODR_Addr, n)	//���
#define PBin(n)		BIT_ADDR(GPIOB_IDR_Addr, n)	//����

#define PCout(n)	BIT_ADDR(GPIOC_ODR_Addr, n)	//���
#define PCin(n)		BIT_ADDR(GPIOC_IDR_Addr, n)	//����

#define PDout(n)	BIT_ADDR(GPIOD_ODR_Addr, n)	//���
#define PDin(n)		BIT_ADDR(GPIOD_IDR_Addr, n)	//����

#define PEout(n)	BIT_ADDR(GPIOE_ODR_Addr, n)	//���
#define PEin(n)		BIT_ADDR(GPIOE_IDR_Addr, n)	//����

#define PFout(n)	BIT_ADDR(GPIOF_ODR_Addr, n)	//���
#define PFin(n)		BIT_ADDR(GPIOF_IDR_Addr, n)	//����

#define PGout(n)	BIT_ADDR(GPIOG_ODR_Addr, n)	//���
#define PGin(n)		BIT_ADDR(GPIOG_IDR_Addr, n)	//����

//APB2ʱ����ö������
typedef enum {
	PA = (int)GPIOA,
	PB = (int)GPIOB, 
	PC = (int)GPIOC, 
	PD = (int)GPIOD, 
	PE = (int)GPIOE, 
	PF = (int)GPIOF, 
	PG = (int)GPIOG
} GPIO_Type;

//CNF MODE
typedef enum {
	AIN = 0x0,				//00 00 ģ������
	IN_FLOATING = 0x4,		//01 00 ��������
	IPU = 0x8,				//10 00 ��������
	IPD = 0x9,				//10 00 �������� + 1 ����

	OUT_PP = 0x0,			//00 00 ͨ���������
	OUT_PD = 0x4,			//01 00 ͨ�ÿ�©���
	AF_PP  = 0x8,			//10 00 �����������
	AF_PD  = 0xC,			//11 00 ���ÿ�©���
	
} GPIO_MODE_Type;

/*
//CNF MODE
typedef enum {
	OUT_PP = 0x0,			//00 00 ͨ���������
	OUT_PD = 0x4,			//01 00 ͨ�ÿ�©���
	AF_PP  = 0x8,			//10 00 �����������
	AF_PD  = 0xC,			//11 00 ���ÿ�©���
	
} GPIO_MODE_OUT_Type;
*/
typedef enum {
	_IN_ = 0x0,		// 00 00 ���� or ����		
	_10MHZ = 0x1,	// 00 01
	_2MHZ = 0x2,	// 00 10
	_50MHZ = 0x3,	// 00 11
	
} GPIO_SPEED_Type;

//����GPIOʱ��
void GPIO_CLOCK_ON(GPIO_Type Px);

//GPIO����
void GPIO_Config(GPIO_Type Px , uint8_t Pin, GPIO_MODE_Type Mode, GPIO_SPEED_Type Speed);


#endif
