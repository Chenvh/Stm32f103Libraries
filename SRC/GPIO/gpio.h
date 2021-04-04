#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f10x.h"
#include "stdbool.h"

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr) 			*((volatile unsigned long *) (addr))
#define BIT_ADDR(addr, bitnum)	MEM_ADDR(BITBAND(addr, bitnum))	

//外部中线基地址 	PERIPH_BASE = 0x40000000
//APB1总线基地址		APB1PERIPH_BASE = PERIPH_BASE = 0x40000000
//APB2总线基地址		APB2PERIPH_BASE = PERIPH_BASE + 0x10000 = 0x40010000


//IO口地址映射
//ODR寄存器
#define GPIOA_ODR_Addr		(GPIOA_BASE + 12)	//GPIOA_BASE = APB2PERIPH_BASE + 0x0800, GPIOA_ODR_Addr + 12 = 0x4001080C
#define GPIOB_ODR_Addr		(GPIOB_BASE + 12)	//+0x00c
#define GPIOC_ODR_Addr		(GPIOC_BASE + 12)	//+0x00c
#define GPIOD_ODR_Addr		(GPIOD_BASE + 12)	//+0x00c
#define GPIOE_ODR_Addr		(GPIOE_BASE + 12)	//+0x00c
#define GPIOF_ODR_Addr		(GPIOF_BASE + 12)	//+0x00c
#define GPIOG_ODR_Addr		(GPIOG_BASE + 12)	//+0x00c

//IDR寄存器
#define GPIOA_IDR_Addr		(GPIOA_BASE + 8)	//GPIOA_BASE = APB2PERIPH_BASE + 0x0800, GPIOA_IDR_Addr + 8 = 0x40010808
#define GPIOB_IDR_Addr		(GPIOB_BASE + 8)	//+0x00c
#define GPIOC_IDR_Addr		(GPIOC_BASE + 8)	//+0x00c
#define GPIOD_IDR_Addr		(GPIOD_BASE + 8)	//+0x00c
#define GPIOE_IDR_Addr		(GPIOE_BASE + 8)	//+0x00c
#define GPIOF_IDR_Addr		(GPIOF_BASE + 8)	//+0x00c
#define GPIOG_IDR_Addr		(GPIOG_BASE + 8)	//+0x00c



//IO 输入输出
#define PAout(n)	BIT_ADDR(GPIOA_ODR_Addr, n)	//输出
#define PAin(n)		BIT_ADDR(GPIOA_IDR_Addr, n)	//输入

#define PBout(n)	BIT_ADDR(GPIOB_ODR_Addr, n)	//输出
#define PBin(n)		BIT_ADDR(GPIOB_IDR_Addr, n)	//输入

#define PCout(n)	BIT_ADDR(GPIOC_ODR_Addr, n)	//输出
#define PCin(n)		BIT_ADDR(GPIOC_IDR_Addr, n)	//输入

#define PDout(n)	BIT_ADDR(GPIOD_ODR_Addr, n)	//输出
#define PDin(n)		BIT_ADDR(GPIOD_IDR_Addr, n)	//输入

#define PEout(n)	BIT_ADDR(GPIOE_ODR_Addr, n)	//输出
#define PEin(n)		BIT_ADDR(GPIOE_IDR_Addr, n)	//输入

#define PFout(n)	BIT_ADDR(GPIOF_ODR_Addr, n)	//输出
#define PFin(n)		BIT_ADDR(GPIOF_IDR_Addr, n)	//输入

#define PGout(n)	BIT_ADDR(GPIOG_ODR_Addr, n)	//输出
#define PGin(n)		BIT_ADDR(GPIOG_IDR_Addr, n)	//输入

//APB2时钟用枚举类型
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
	AIN = 0x0,				//00 00 模拟输入
	IN_FLOATING = 0x4,		//01 00 浮空输入
	IPU = 0x8,				//10 00 浮空上拉
	IPD = 0x9,				//10 00 浮空下拉 + 1 区别开

	OUT_PP = 0x0,			//00 00 通用推挽输出
	OUT_PD = 0x4,			//01 00 通用开漏输出
	AF_PP  = 0x8,			//10 00 复用推挽输出
	AF_PD  = 0xC,			//11 00 复用开漏输出
	
} GPIO_MODE_Type;

/*
//CNF MODE
typedef enum {
	OUT_PP = 0x0,			//00 00 通用推挽输出
	OUT_PD = 0x4,			//01 00 通用开漏输出
	AF_PP  = 0x8,			//10 00 复用推挽输出
	AF_PD  = 0xC,			//11 00 复用开漏输出
	
} GPIO_MODE_OUT_Type;
*/
typedef enum {
	_IN_ = 0x0,		// 00 00 保留 or 输入		
	_10MHZ = 0x1,	// 00 01
	_2MHZ = 0x2,	// 00 10
	_50MHZ = 0x3,	// 00 11
	
} GPIO_SPEED_Type;

//开启GPIO时钟
void GPIO_CLOCK_ON(GPIO_Type Px);

//GPIO配置
void GPIO_Config(GPIO_Type Px , uint8_t Pin, GPIO_MODE_Type Mode, GPIO_SPEED_Type Speed);


#endif
