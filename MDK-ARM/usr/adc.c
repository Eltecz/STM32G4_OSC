/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\adc.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-05-21 15:30:47
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-22 12:52:45
 */
#include "adc.h"	 

u16 adc1_buf[adc_dma_length]__attribute__((at(0X20004000)));
u16 adc2_buf[adc_dma_length]__attribute__((at(0X20004000 + sizeof(adc1_buf))));
//初始化ADC
//这里我们仅以规则通道为例
//我们默认仅开启ADC1_CH5																	   
void  Adc_Init(void)
{    
	//先初始化IO口
 	RCC->AHB2ENR|=1<<13;    	//使能ADC12时钟 
	RCC->AHB2ENR|=1<<0;   		//使能PORTA口时钟  
	RCC->CCIPR|=2<<28;			//SYSTICK作为ADC12时钟
	
	GPIO_Set(GPIOA,PIN0|PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PA5,模拟输入,下拉   

	RCC->AHB2RSTR|=1<<13;   	//ADCs复位
	RCC->AHB2RSTR&=~(1<<13);	//复位结束	 

	ADC12_COMMON->CCR|=(2<<18);	//SYSCLK分频	
	ADC12_COMMON->CCR|=(0<<16);	//时钟同步模式
	ADC1->ISR|=1;
	ADC1->CR=0;   				//CR1设置清零
	ADC1->CR|=1<<28;			//启动稳压器
	delay_us(10);

	ADC1->CFGR=0;
	ADC1->CFGR|=0<<3;      		//12位模式
	
	ADC1->CFGR&=~(1<<13);    	//单次转换模式
 	ADC1->CFGR&=~(1<<15);   	//右对齐	
	ADC1->CFGR&=~(1<<10);    	//软件触发
	ADC1->CFGR|=1<<12;			//数据覆盖
	ADC1->CFGR|=1<<31;			//禁止注入
	
	//PA5,ADC1_CH5_IN，采样时间为2.5ADC周期
	ADC1->SMPR1=0;
	ADC1->SQR1=0;

	ADC1->SQR1|=1<<6;//1个转换在规则序列中 也就是只转换规则序列5			   
	//设置通道5的采样时间
 	ADC1->SMPR1|=(0<<3); 	//通道5  480个周期,提高采样时间可以提高精确度	 
//	ADC1->SQR1|=1<<6;

	ADC1->CR|=1<<31;		//校准ADC
	while(!(ADC1->CR&(1<<31)));//等待转换结束	
 	ADC1->CR|=1;	   	//开启AD转换器	  	
	while(!(ADC1->ISR&(1<<0)));//等待转换结束		
	ADC1->ISR|=1;	

}		

void  Adc_DMA_Init(u8 t,u8 ch)
{    
	ADC_DMA_CHNNAL_INIT();
	ADC_TIM_init(t);
	//先初始化IO口
 	RCC->AHB2ENR|=1<<13;    	//使能ADC12时钟 
//	RCC->AHB2ENR|=1<<0;   		//使能PORTA口时钟  
	RCC->CCIPR|=2<<28;			//SYSTICK作为ADC12时钟
	
	GPIO_Set(GPIOA,PIN0|PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PA5,模拟输入,下拉   

	RCC->AHB2RSTR|=1<<13;   	//ADCs复位
	RCC->AHB2RSTR&=~(1<<13);	//复位结束	 

	ADC12_COMMON->CCR|=(1<<18);	//SYSCLK分频	
	ADC12_COMMON->CCR|=(0<<16);	//时钟同步模式
	ADC1->ISR|=1;
	ADC1->CR=0;   				//CR1设置清零
	ADC1->CR|=1<<28;			//启动稳压器

	ADC2->ISR|=1;
	ADC2->CR=0;   				//CR1设置清零
	ADC2->CR|=1<<28;			//启动稳压器
	delay_us(10);

	ADC1->CFGR=0;
	ADC1->CFGR|=0<<3;      		//12位模式
	
	ADC1->CFGR&=~(1<<13);    	//单次转换模式
 	ADC1->CFGR&=~(1<<15);   	//右对齐	
	ADC1->CFGR|=(1<<10);    	//上升沿触发
	ADC1->CFGR|=0<<5;			//ADC1--TIM1CC1
	ADC1->CFGR|=1<<12;			//数据覆盖
	ADC1->CFGR|=1<<31;			//禁止注入
	ADC1->CFGR|=1<<1;			//ADC连续转换模式
	ADC1->CFGR|=1<<0;			//ADC DMA开启
	//PA5,ADC1_CH5_IN，采样时间为2.5ADC周期
	ADC1->SMPR1=0;
	ADC1->SQR1=0;

	ADC1->SQR1|=1<<6;//1个转换在规则序列中 也就是只转换规则序列5			   
	//设置通道5的采样时间
 	ADC1->SMPR1|=(0<<3); 	//通道5  480个周期,提高采样时间可以提高精确度	 

	ADC1->CR|=1<<31;		//校准ADC
	while(!(ADC1->CR&(1<<31)));//等待转换结束	
 	ADC1->CR|=1;	   	//开启AD转换器	  	
	while(!(ADC1->ISR&(1<<0)));//等待转换结束		
	ADC1->ISR|=1;	

	ADC2->CFGR=0;
	ADC2->CFGR|=0<<3;      		//12位模式
	
	ADC2->CFGR&=~(1<<13);    	//单次转换模式
 	ADC2->CFGR&=~(1<<15);   	//右对齐	
	ADC2->CFGR|=(1<<10);    	//上升沿触发
	ADC2->CFGR|=1<<5;			//ADC2--TIM1CC2
	ADC2->CFGR|=1<<12;			//数据覆盖
	ADC2->CFGR|=1<<31;			//禁止注入
	ADC2->CFGR|=1<<1;			//ADC连续转换模式
	ADC2->CFGR|=1<<0;			//ADC DMA开启
	//PA5,ADC2_CH5_IN，采样时间为2.5ADC周期
	ADC2->SMPR1=0;
	ADC2->SQR1=0;

	ADC2->SQR1|=2<<6;//1个转换在规则序列中 也就是只转换规则序列5			   
	//设置通道5的采样时间
 	ADC2->SMPR1|=(0<<3); 	//通道5  480个周期,提高采样时间可以提高精确度	 

	ADC2->CR|=1<<31;		//校准ADC
	while(!(ADC2->CR&(1<<31)));//等待转换结束	
 	ADC2->CR|=1;	   	//开启AD转换器	  	
	while(!(ADC2->ISR&(1<<0)));//等待转换结束		
	ADC2->ISR|=1;	
}	

//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR1=0;//规则序列1 通道ch
	ADC1->SQR1|=ch<<6;
	ADC1->SMPR1|=0<<(ch*3);
	ADC1->CR|=1<<2;       //启动规则转换通道 
	while(!(ADC1->ISR&1<<2));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}  









