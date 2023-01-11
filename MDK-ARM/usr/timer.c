/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\timer.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-03 20:29:41
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-09 23:41:48
 */
#include "per_conf.h" 
/*
APB1=180Mhz
timedelay=180/10=18

LPTIM1	RES
TIM1	->ADC1,ADC2
TIM2	RES
TIM3	RES
TIM4	->encoder
TIM6	->DAC3CH1
TIM8	RES
TIM15	RES
TIM16	->PWM1
TIM17	->PWM2

*/
#define	pre_fix	MAIN_FOSC/180
u16 adc_delay[][2] = {
	18*pre_fix, 2,		//50M,software
	18*pre_fix, 2,		//20M,software
	18*pre_fix, 2,		//10M,software
	18*pre_fix,	2,		//5M
	45*pre_fix,	2,		//2M
	45*pre_fix,	4,		//1M
	45*pre_fix,	8,		//500K
	45*pre_fix,	20,		//200K
	90*pre_fix,	20,		//100K
	90*pre_fix,	40,		//50K
	90*pre_fix,	100,	//20K
	90*pre_fix,	200,	//10K
	180*pre_fix,200,	//5K
	180*pre_fix,500,	//2K
	360*pre_fix,500,	//1K
	360*pre_fix,1000,	//500
};	
u16 pwm_lut[][2] = {
	0,	0,
	9*pre_fix,	1,		//20M
	9*pre_fix,	2,		//10M
	9*pre_fix,	4,		//5M	
	9*pre_fix,	10,		//2M
	9*pre_fix,	20,		//1M
	9*pre_fix,	40,		//500K
	9*pre_fix,	100,	//200K
	9*pre_fix,	200,	//100K
	9*pre_fix, 	400,	//50K
	9*pre_fix,	1000,	//20K
	9*pre_fix,	2000,	//10K
	9*pre_fix,	4000,	//5K
	9*pre_fix,	10000,	//2K
	9*pre_fix,	20000,	//1K
	9*pre_fix,	40000,	//500
};
u16 dac_trig_lut[][2]={
	9*pre_fix,	2,		//10M	
	9*pre_fix,	4,		//5M	
	9*pre_fix,	10,		//2M
	9*pre_fix,	20,		//1M	
	9*pre_fix,	40,		//500K
	9*pre_fix,	100,	//200K	
	9*pre_fix,	200,	//100K
	9*pre_fix, 	400,	//50K
	9*pre_fix,	1000,	//20K
	9*pre_fix,	2000,	//10K
	9*pre_fix,	4000,	//5K
	9*pre_fix,	10000,	//2K
	9*pre_fix,	20000,	//1K
	9*pre_fix,	40000,	//500	
};
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR1|=1<<1;				//TIM3时钟使能  
	//如果开启TIM3时钟后,直接开始配置TIM3,则会有问题
	while((RCC->APB1ENR1&(1<<1))==0);	//等待时钟设置OK  
 	TIM3->ARR=arr;  				//设定计数器自动重装值 
	TIM3->PSC=psc;  				//预分频器
	TIM3->SMCR=0;					//SMS[3:0]=0000,CK_INT作为时钟源
	TIM3->DIER|=1<<0;  				//允许更新中断	  
	TIM3->CR1&=~(3<<5);				//CMS[1:0]=00,边沿对齐模式
	TIM3->CR1&=~(1<<4);				//DIR=0,向上计数模式
	TIM3->CR1|=0x01;				//使能定时器3
}
void TIM4_Int_Init(u16 arr,u16 psc)	//encoder
{
	RCC->APB1ENR1|=1<<2;				//TIM4时钟使能  
	//如果开启TIM4时钟后,直接开始配置TIM4,则会有问题
	while((RCC->APB1ENR1&(1<<2))==0);	//等待时钟设置OK  
	RCC->AHB2ENR|=1<<3;//PD
	GPIO_Set_withAF(GPIOD,PIN12|PIN13,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU,2);
 	TIM4->ARR=arr;  				//设定计数器自动重装值 
	TIM4->PSC=psc;  				//预分频器
	TIM4->CCMR1|=1<<8;
	TIM4->CCMR1|=0xf<<4;
	TIM4->CCMR2|=1<<8;
	TIM4->CCMR2|=0xf<<4;
	TIM4->SMCR=3;					//SMS[3:0]=0000,CK_INT作为时钟源
//	TIM4->DIER|=1<<0;  				//允许更新中断	  
//	TIM4->CR1&=~(3<<5);				//CMS[1:0]=00,边沿对齐模式
//	TIM4->CR1&=~(1<<4);				//DIR=0,向上计数模式
//	TIM4->CR1|=1<<4;
	TIM4->CR1|=1<<7;
	TIM4->CR1|=0x01;				//使能定时器3
}
void ADC_TIM_init(u8 tims)
{

	RCC->APB2ENR|=1<<11;	//TIM1时钟使能  
	while((RCC->APB2ENR&(1<<11))==0);	//等待时钟设置OK  
 	TIM1->ARR=(u32)(adc_delay[tims][0]-1);  	//设定计数器自动重装值 
	TIM1->PSC=adc_delay[tims][1]-1;  	//预分频器	
	TIM1->CR1&=~(3<<8);
	TIM1->CR1&=~(1<<7);	
	TIM1->RCR=0;
//oc1	
	TIM1->CCMR1|=6<<4;
	TIM1->CCR1=((u32)(adc_delay[tims][0]-1));
	TIM1->CCER|=0xf<<0;

//oc2
	TIM1->CCMR1|=6<<12;
	TIM1->CCR2=((u32)(adc_delay[tims][0]-1));
	TIM1->CCER|=0xf<<4;

	TIM1->BDTR|=1<<15;
	TIM1->CR1|=0x01;    //使能定时器3
}
void PWM_TIM_INIT(u8 t1,u8 t2,u8 d1,u8 d2)
{
	RCC->APB2RSTR |= 3 << 17;
	delay_ms(1);
	RCC->APB2RSTR -= 3 << 17;
	RCC->AHB2ENR|=3;
	GPIO_Set_withAF(GPIOB,PIN6|PIN7,GPIO_OTYPE_PP,3,GPIO_PUPD_PU,1);	
	if(t1){
		RCC->APB2ENR|=1<<17;				//TIM16时钟使能  

		//如果开启TIM16时钟后,直接开始配置TIM16,则会有问题
		while((RCC->APB2ENR&(1<<17))==0);	//等待时钟设置OK  
		TIM16->ARR=pwm_lut[t1][0]-1;	//设定计数器自动重装值 
		TIM16->PSC=pwm_lut[t1][1]-1;	//预分频器
		TIM16->CCR1=d1;
		TIM16->CR1|=1<<7;				//使能自动装载

		TIM16->CCMR1 |= 0X6 << 4;
		TIM16->CCMR1 |= 3 << 2;
		TIM16->EGR |= 1 << 0;
		TIM16->CCER |= 1 << 2;
		TIM16->BDTR |= 1<<15;
		TIM16->CR1 |= 1 << 0;
	}
	if(t2){
		RCC->APB2ENR|=1<<18;				//TIM17时钟使能  
		//如果开启TIM17时钟后,直接开始配置TIM17,则会有问题
		while((RCC->APB2ENR&(1<<18))==0);	//等待时钟设置OK  
		TIM17->ARR=pwm_lut[t2][0]-1;	//设定计数器自动重装值 
		TIM17->PSC=pwm_lut[t2][1]-1;	//预分频器
		TIM17->CCR1=d2;
		TIM17->CR1|=1<<7;				//使能自动装载

		TIM17->CCMR1 |= 0X6 << 4;
		TIM17->CCMR1 |= 3 << 2;
		TIM17->EGR |= 1 << 0;
		TIM17->CCER |= 1 << 2;
		TIM17->BDTR |= 1<< 15;
		TIM17->CR1 |= 1 << 0;
	}	
}

void dac_timer_init(u8 t1,u8 t2){
	RCC->APB1RSTR1 |= 3 << 4;
	delay_ms(1);
	RCC->APB1RSTR1 -= 3 << 4;
	RCC->AHB2ENR|=1;	
	RCC->APB1ENR1|=1<<4;				//TIM6时钟使能  
	//如果开启TIM6时钟后,直接开始配置TIM6,则会有问题
	while((RCC->APB1ENR1&(1<<4))==0);	//等待时钟设置OK  
	TIM6->ARR=dac_trig_lut[t1][0]-1;	//设定计数器自动重装值 
	TIM6->PSC=dac_trig_lut[t1][1]-1;	//预分频器
	TIM6->EGR|=1;		
	TIM6->CR1|=1<<2;
	TIM6->CR1|=1<<7;				//使能自动装载
	TIM6->CR2|=2<<4;
	RCC->APB1ENR1|=1<<5;				//TIM7时钟使能  
	//如果开启TIM7时钟后,直接开始配置TIM7,则会有问题
	while((RCC->APB1ENR1&(1<<5))==0);	//等待时钟设置OK  
	TIM7->ARR=dac_trig_lut[t2][0]-1;	//设定计数器自动重装值 
	TIM7->PSC=dac_trig_lut[t2][1]-1;	//预分频器
	TIM7->EGR|=1;
	TIM7->CR1|=1<<2;
	TIM7->CR1|=1<<7;				//使能自动装载
	TIM7->CR2|=2<<4;	
}









