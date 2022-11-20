/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\usart.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-21 23:46:28
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-11 11:01:33
 */
#include "per_conf.h"
#include "stdio.h"
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
//支持keil AC5及AC6,by Eltecz
#if 1
#ifdef  __CC_ARM
#pragma import(__use_no_semihosting)
#elif defined ( __GNUC__ ) || defined (__clang__)
__asm (".global __use_no_semihosting\n\t");
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)//不使用microlib，请注释
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//标准库需要的支持函数
	struct __FILE
	{
		int handle;
		/* Whatever you require here. If the only file you are using is */
		/* standard output using printf() for debugging, no file handling */
		/* is required. */
	};
	/* FILE is typedef’ d in stdio.h. */
	FILE __stdout;
#endif

//解决HAL库使用时,某些情况可能报错的bug
int _ttywrch(int ch)
{
    ch=ch;
	return ch;
}


//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART1->TDR = (unsigned char) ch;      
	return ch;
}
#endif

//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//注意:USART1的时钟,是可以通过RCC_D2CCIP2R选择的
//但是我们一般用默认的选择即可(默认选择rcc_pclk2作为串口1时钟)
//pclk2即APB2的时钟频率,一般为100Mhz
//bound:波特率 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	u32	temp;	   
	temp=(pclk2*500000)/bound;	//得到USARTDIV@OVER8=0,采用四舍五入计算
	RCC->AHB2ENR|=1<<2;   	//使能PORTC口时钟  
	RCC->APB2ENR|=1<<14;  	//使能串口1时钟 
	RCC->CCIPR|=1<<0;
	GPIO_Set_withAF(GPIOC,PIN4|PIN5,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU,7);//PC4,PC5,复用功能,上拉输出 
	//波特率设置
 	USART1->BRR=temp; 		//波特率设置@OVER8=0 	
	USART1->CR1=0;		 	//清零CR1寄存器
	USART1->CR1|=0<<28;	 	//设置M1=0
	USART1->CR1|=0<<12;	 	//设置M0=0&M1=0,选择8位字长 
	USART1->CR1|=0<<15; 	//设置OVER8=0,16倍过采样 
	USART1->CR1|=1<<3;  	//串口发送使能 
//	USART1->ISR=0x6200e2;
#if EN_USART1_RX		  	//如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<2;  	//串口接收使能
	USART1->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
	USART1->CR1|=1<<0;  	//串口使能
}
void uart1_sendonebyte(u8 dat)
{
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART1->TDR = (u8) dat;      	
}