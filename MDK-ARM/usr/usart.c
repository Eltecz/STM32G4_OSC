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
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
//֧��keil AC5��AC6,by Eltecz
#if 1
#ifdef  __CC_ARM
#pragma import(__use_no_semihosting)
#elif defined ( __GNUC__ ) || defined (__clang__)
__asm (".global __use_no_semihosting\n\t");
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)//��ʹ��microlib����ע��
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//��׼����Ҫ��֧�ֺ���
	struct __FILE
	{
		int handle;
		/* Whatever you require here. If the only file you are using is */
		/* standard output using printf() for debugging, no file handling */
		/* is required. */
	};
	/* FILE is typedef�� d in stdio.h. */
	FILE __stdout;
#endif

//���HAL��ʹ��ʱ,ĳЩ������ܱ����bug
int _ttywrch(int ch)
{
    ch=ch;
	return ch;
}


//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR = (unsigned char) ch;      
	return ch;
}
#endif

//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//ע��:USART1��ʱ��,�ǿ���ͨ��RCC_D2CCIP2Rѡ���
//��������һ����Ĭ�ϵ�ѡ�񼴿�(Ĭ��ѡ��rcc_pclk2��Ϊ����1ʱ��)
//pclk2��APB2��ʱ��Ƶ��,һ��Ϊ100Mhz
//bound:������ 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	u32	temp;	   
	temp=(pclk2*500000)/bound;	//�õ�USARTDIV@OVER8=0,���������������
	RCC->AHB2ENR|=1<<2;   	//ʹ��PORTC��ʱ��  
	RCC->APB2ENR|=1<<14;  	//ʹ�ܴ���1ʱ�� 
	RCC->CCIPR|=1<<0;
	GPIO_Set_withAF(GPIOC,PIN4|PIN5,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU,7);//PC4,PC5,���ù���,������� 
	//����������
 	USART1->BRR=temp; 		//����������@OVER8=0 	
	USART1->CR1=0;		 	//����CR1�Ĵ���
	USART1->CR1|=0<<28;	 	//����M1=0
	USART1->CR1|=0<<12;	 	//����M0=0&M1=0,ѡ��8λ�ֳ� 
	USART1->CR1|=0<<15; 	//����OVER8=0,16�������� 
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
//	USART1->ISR=0x6200e2;
#if EN_USART1_RX		  	//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<2;  	//���ڽ���ʹ��
	USART1->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
	USART1->CR1|=1<<0;  	//����ʹ��
}
void uart1_sendonebyte(u8 dat)
{
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR = (u8) dat;      	
}