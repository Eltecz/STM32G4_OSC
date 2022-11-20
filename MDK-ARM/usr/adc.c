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
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ADC1_CH5																	   
void  Adc_Init(void)
{    
	//�ȳ�ʼ��IO��
 	RCC->AHB2ENR|=1<<13;    	//ʹ��ADC12ʱ�� 
	RCC->AHB2ENR|=1<<0;   		//ʹ��PORTA��ʱ��  
	RCC->CCIPR|=2<<28;			//SYSTICK��ΪADC12ʱ��
	
	GPIO_Set(GPIOA,PIN0|PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PA5,ģ������,����   

	RCC->AHB2RSTR|=1<<13;   	//ADCs��λ
	RCC->AHB2RSTR&=~(1<<13);	//��λ����	 

	ADC12_COMMON->CCR|=(2<<18);	//SYSCLK��Ƶ	
	ADC12_COMMON->CCR|=(0<<16);	//ʱ��ͬ��ģʽ
	ADC1->ISR|=1;
	ADC1->CR=0;   				//CR1��������
	ADC1->CR|=1<<28;			//������ѹ��
	delay_us(10);

	ADC1->CFGR=0;
	ADC1->CFGR|=0<<3;      		//12λģʽ
	
	ADC1->CFGR&=~(1<<13);    	//����ת��ģʽ
 	ADC1->CFGR&=~(1<<15);   	//�Ҷ���	
	ADC1->CFGR&=~(1<<10);    	//�������
	ADC1->CFGR|=1<<12;			//���ݸ���
	ADC1->CFGR|=1<<31;			//��ֹע��
	
	//PA5,ADC1_CH5_IN������ʱ��Ϊ2.5ADC����
	ADC1->SMPR1=0;
	ADC1->SQR1=0;

	ADC1->SQR1|=1<<6;//1��ת���ڹ��������� Ҳ����ֻת����������5			   
	//����ͨ��5�Ĳ���ʱ��
 	ADC1->SMPR1|=(0<<3); 	//ͨ��5  480������,��߲���ʱ�������߾�ȷ��	 
//	ADC1->SQR1|=1<<6;

	ADC1->CR|=1<<31;		//У׼ADC
	while(!(ADC1->CR&(1<<31)));//�ȴ�ת������	
 	ADC1->CR|=1;	   	//����ADת����	  	
	while(!(ADC1->ISR&(1<<0)));//�ȴ�ת������		
	ADC1->ISR|=1;	

}		

void  Adc_DMA_Init(u8 t,u8 ch)
{    
	ADC_DMA_CHNNAL_INIT();
	ADC_TIM_init(t);
	//�ȳ�ʼ��IO��
 	RCC->AHB2ENR|=1<<13;    	//ʹ��ADC12ʱ�� 
//	RCC->AHB2ENR|=1<<0;   		//ʹ��PORTA��ʱ��  
	RCC->CCIPR|=2<<28;			//SYSTICK��ΪADC12ʱ��
	
	GPIO_Set(GPIOA,PIN0|PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PA5,ģ������,����   

	RCC->AHB2RSTR|=1<<13;   	//ADCs��λ
	RCC->AHB2RSTR&=~(1<<13);	//��λ����	 

	ADC12_COMMON->CCR|=(1<<18);	//SYSCLK��Ƶ	
	ADC12_COMMON->CCR|=(0<<16);	//ʱ��ͬ��ģʽ
	ADC1->ISR|=1;
	ADC1->CR=0;   				//CR1��������
	ADC1->CR|=1<<28;			//������ѹ��

	ADC2->ISR|=1;
	ADC2->CR=0;   				//CR1��������
	ADC2->CR|=1<<28;			//������ѹ��
	delay_us(10);

	ADC1->CFGR=0;
	ADC1->CFGR|=0<<3;      		//12λģʽ
	
	ADC1->CFGR&=~(1<<13);    	//����ת��ģʽ
 	ADC1->CFGR&=~(1<<15);   	//�Ҷ���	
	ADC1->CFGR|=(1<<10);    	//�����ش���
	ADC1->CFGR|=0<<5;			//ADC1--TIM1CC1
	ADC1->CFGR|=1<<12;			//���ݸ���
	ADC1->CFGR|=1<<31;			//��ֹע��
	ADC1->CFGR|=1<<1;			//ADC����ת��ģʽ
	ADC1->CFGR|=1<<0;			//ADC DMA����
	//PA5,ADC1_CH5_IN������ʱ��Ϊ2.5ADC����
	ADC1->SMPR1=0;
	ADC1->SQR1=0;

	ADC1->SQR1|=1<<6;//1��ת���ڹ��������� Ҳ����ֻת����������5			   
	//����ͨ��5�Ĳ���ʱ��
 	ADC1->SMPR1|=(0<<3); 	//ͨ��5  480������,��߲���ʱ�������߾�ȷ��	 

	ADC1->CR|=1<<31;		//У׼ADC
	while(!(ADC1->CR&(1<<31)));//�ȴ�ת������	
 	ADC1->CR|=1;	   	//����ADת����	  	
	while(!(ADC1->ISR&(1<<0)));//�ȴ�ת������		
	ADC1->ISR|=1;	

	ADC2->CFGR=0;
	ADC2->CFGR|=0<<3;      		//12λģʽ
	
	ADC2->CFGR&=~(1<<13);    	//����ת��ģʽ
 	ADC2->CFGR&=~(1<<15);   	//�Ҷ���	
	ADC2->CFGR|=(1<<10);    	//�����ش���
	ADC2->CFGR|=1<<5;			//ADC2--TIM1CC2
	ADC2->CFGR|=1<<12;			//���ݸ���
	ADC2->CFGR|=1<<31;			//��ֹע��
	ADC2->CFGR|=1<<1;			//ADC����ת��ģʽ
	ADC2->CFGR|=1<<0;			//ADC DMA����
	//PA5,ADC2_CH5_IN������ʱ��Ϊ2.5ADC����
	ADC2->SMPR1=0;
	ADC2->SQR1=0;

	ADC2->SQR1|=2<<6;//1��ת���ڹ��������� Ҳ����ֻת����������5			   
	//����ͨ��5�Ĳ���ʱ��
 	ADC2->SMPR1|=(0<<3); 	//ͨ��5  480������,��߲���ʱ�������߾�ȷ��	 

	ADC2->CR|=1<<31;		//У׼ADC
	while(!(ADC2->CR&(1<<31)));//�ȴ�ת������	
 	ADC2->CR|=1;	   	//����ADת����	  	
	while(!(ADC2->ISR&(1<<0)));//�ȴ�ת������		
	ADC2->ISR|=1;	
}	

//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	//����ת������	  		 
	ADC1->SQR1=0;//��������1 ͨ��ch
	ADC1->SQR1|=ch<<6;
	ADC1->SMPR1|=0<<(ch*3);
	ADC1->CR|=1<<2;       //��������ת��ͨ�� 
	while(!(ADC1->ISR&1<<2));//�ȴ�ת������	 	   
	return ADC1->DR;		//����adcֵ	
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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









