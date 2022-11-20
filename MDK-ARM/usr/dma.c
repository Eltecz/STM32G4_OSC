/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\dma.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-05-22 11:28:06
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 10:15:07
 */
#include "dma.h"
/*
SPI1->DMA   DMA_CHNNAL  1
ADC1->DMA	DMA_CHNNAL  2
ADC2->DMA	DMA_CHNNAL	3
DMA->DAC3C1	DMA_CHNNAL	4
DMA->SPI3	DMA_CHNNAL	6
SPI3->DMA	DMA_CHNNAL	7

*/
vu16 fill_color_buf;

extern  u16 adc1_buf[adc_dma_length];
extern  u16 adc2_buf[adc_dma_length];
DMA_Channel_TypeDef *SPI1_LCD_DMA=DMA1_Channel1;
DMAMUX_Channel_TypeDef  *SPI1_LCD_DMA_MUX=DMAMUX1_Channel0;

DMA_Channel_TypeDef *ADC1_DMA=DMA1_Channel2;
DMAMUX_Channel_TypeDef  *ADC1_DMA_MUX=DMAMUX1_Channel1;

DMA_Channel_TypeDef *ADC2_DMA=DMA1_Channel3;
DMAMUX_Channel_TypeDef  *ADC2_DMA_MUX=DMAMUX1_Channel2;

DMA_Channel_TypeDef *DAC3C1_DMA=DMA1_Channel4;
DMAMUX_Channel_TypeDef  *DAC3C1_DMA_MUX=DMAMUX1_Channel3;

DMA_Channel_TypeDef *SPI3TX_DMA=DMA1_Channel6;
DMAMUX_Channel_TypeDef	*SPI3TX_DMA_MUX=DMAMUX1_Channel5;

DMA_Channel_TypeDef *SPI3RX_DMA=DMA1_Channel6;
DMAMUX_Channel_TypeDef	*SPI3RX_DMA_MUX=DMAMUX1_Channel5;
void SPI1_LCD_DMA_INIT()
{
	RCC->AHB1ENR|=5<<0;//DMA1时钟使能 
	while(SPI1_LCD_DMA->CCR&0X01);//等待DMA可配置 
	DMA1->IFCR|=0X07;//清空之前该stream上的所有中断标志

	SPI1_LCD_DMA->CPAR=(u32)&SPI1->DR;		//DMA外设地址
	SPI1_LCD_DMA->CNDTR=0;			//DMA 传输数据长度
	SPI1_LCD_DMA->CCR=0;			//先全部复位CR寄存器值 
	
	SPI1_LCD_DMA->CCR|=1<<4;		//存储器到外设模式
	SPI1_LCD_DMA->CCR|=0<<5;		//非循环模式(即使用普通模式)
	SPI1_LCD_DMA->CCR|=0<<6;		//外设非增量模式
	SPI1_LCD_DMA->CCR|=0<<7;		//存储器非增量模式
	SPI1_LCD_DMA->CCR|=0<<8;		//外设数据长度:8位
	SPI1_LCD_DMA->CCR|=1<<10;		//存储器数据长度:8位
	SPI1_LCD_DMA->CCR|=1<<12;		//中等优先级
	SPI1_LCD_DMA_MUX->CCR=11;	//通道选择	
}
void SPI1_DMA_START(u32 ndtr,u32 buftr,u8 mode)
{
	SPI1->CR1&=~(1<<6);
	SPI1->CR2|=1<<1;				//开启SPI1 TX DMA
	SPI1->CR1|=1<<6;				//SPE=1,使能SPI1
	SPI1_LCD_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(SPI1_LCD_DMA->CCR&0X1);	//确保DMA可以被设置  
	SPI1_LCD_DMA->CNDTR=(u16)ndtr;	//DMA 传输数据长度
	SPI1_LCD_DMA->CMAR=buftr;	//DMA 存储器0地址
	if(mode){
		SPI1_LCD_DMA->CCR&=~(1<<7);		//外设非增量模式
		SPI1_LCD_DMA->CCR|=1<<10;		//存储器数据长度:8位		
	}
	else	{
		SPI1_LCD_DMA->CCR|=1<<7;		//外设非增量模式
//		SPI1_LCD_DMA->CCR&=~(1<<10);		//存储器数据长度:8位	
	}
	SPI1_LCD_DMA->CCR|=1<<0;		//开启DMA传输
	while (!((DMA1->ISR & 1)));
	DMA1->IFCR|=0X7<<0;
	SPI1->CR1&=~(1<<6);
	SPI1->CR2&=~(1<<1);
	SPI1->CR1|=1<<6;				//SPE=1,使能SPI1
}	

void ADC1_DMA_CHNNAL_INIT()
{
	RCC->AHB1ENR|=1<<0;//DMA1时钟使能 
	ADC1_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(ADC1_DMA->CCR&0X01);//等待DMA可配置 
	DMA1->IFCR|=0xf0;//清空之前该stream上的所有中断标志
	
	ADC1_DMA->CPAR=(u32)&ADC1->DR;		//DMA外设地址
	ADC1_DMA->CMAR=(u32)&adc1_buf;//DMA 存储器0地址
	ADC1_DMA->CNDTR=0;	//DMA 传输数据长度
	ADC1_DMA->CCR=0;			//先全部复位CR寄存器值 
    
//    ADC1_DMA->CCR|=1<<1;     //DMA中断
	ADC1_DMA->CCR|=0<<4;		//外设到存储器模式
	ADC1_DMA->CCR|=0<<5;		//非循环模式(即使用普通模式)
	ADC1_DMA->CCR|=0<<6;		//外设非增量模式
	ADC1_DMA->CCR|=1<<7;		//存储器增量模式
	ADC1_DMA->CCR|=1<<8;		//外设数据长度:16位
	ADC1_DMA->CCR|=1<<10;		//存储器数据长度:16位
	ADC1_DMA->CCR|=1<<12;		//中等优先级
	ADC1_DMA_MUX->CCR|=(u32)5;//通道选择

}
void ADC1_DMA_START(u32 ndtr)
{
	ADC1_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(ADC1_DMA->CCR&0X1);	//确保DMA可以被设置  
	ADC1_DMA->CNDTR=ndtr;		//DMA 传输数据长度
	ADC1->CR|=1<<2;       //启动规则转换通道 
	ADC1_DMA->CCR|=1<<0;		//开启DMA传输
	TIM1->CR1|=0x01;    //使能定时器3
	TIM1->CCER|=1<<12;
	while (!((DMA1->ISR & 0x10)));
	DMA1->IFCR |= 0x10;
	TIM1->CR1-=0x01;    //使能定时器3
	TIM1->CCER-=1<<12;
}	

void ADC2_DMA_CHNNAL_INIT()
{
	RCC->AHB1ENR|=1<<0;//DMA1时钟使能 
	ADC2_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(ADC2_DMA->CCR&0X01);//等待DMA可配置 
	DMA1->IFCR|=0xf00;//清空之前该stream上的所有中断标志
	
	ADC2_DMA->CPAR=(u32)&ADC2->DR;		//DMA外设地址
	ADC2_DMA->CMAR=(u32)&adc2_buf;//DMA 存储器0地址
	ADC2_DMA->CNDTR=0;	//DMA 传输数据长度
	ADC2_DMA->CCR=0;			//先全部复位CR寄存器值 
    
//    ADC2_DMA->CCR|=1<<1;     //DMA中断
	ADC2_DMA->CCR|=0<<4;		//外设到存储器模式
	ADC2_DMA->CCR|=0<<5;		//非循环模式(即使用普通模式)
	ADC2_DMA->CCR|=0<<6;		//外设非增量模式
	ADC2_DMA->CCR|=1<<7;		//存储器增量模式
	ADC2_DMA->CCR|=1<<8;		//外设数据长度:16位
	ADC2_DMA->CCR|=1<<10;		//存储器数据长度:16位
	ADC2_DMA->CCR|=1<<12;		//中等优先级
	ADC2_DMA_MUX->CCR|=(u32)36;//通道选择

}

void ADC2_DMA_START(u32 ndtr)
{
	ADC2_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(ADC2_DMA->CCR&0X1);	//确保DMA可以被设置  
	ADC2_DMA->CNDTR=ndtr;		//DMA 传输数据长度
	ADC2->CR|=1<<2;       //启动规则转换通道 
	ADC2_DMA->CCR|=1<<0;		//开启DMA传输
	TIM1->CR1|=0x01;    //使能定时器3
	TIM1->CCER|=1<<12;
	while (!((DMA1->ISR & 0x100)));
	DMA1->IFCR |= 0x100;
	TIM1->CR1-=0x01;    //使能定时器3
	TIM1->CCER-=1<<12;
}	
void Dual_ADC_DMA_START(u32 ndtr)
{
	ADC1_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(ADC1_DMA->CCR&0X1);	//确保DMA可以被设置  
	ADC1_DMA->CNDTR=ndtr;		//DMA 传输数据长度
	ADC1->CR|=1<<2;       //启动规则转换通道 
	ADC1_DMA->CCR|=1<<0;		//开启DMA传输	
	ADC2_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(ADC2_DMA->CCR&0X1);	//确保DMA可以被设置  
	ADC2_DMA->CNDTR=ndtr;		//DMA 传输数据长度
	ADC2->CR|=1<<2;       //启动规则转换通道 
	ADC2_DMA->CCR|=1<<0;		//开启DMA传输
	TIM1->CR1|=0x01;    //使能定时器3
	TIM1->CCER|=1<<12;	
	while (!((DMA1->ISR & 0x10)));	
	while (!((DMA1->ISR & 0x100)));	
	DMA1->IFCR |= 0x110;	
	TIM1->CR1-=0x01;    //使能定时器3
	TIM1->CCER-=1<<12;		
}
void Dual_ADC_DMA_END()
{
	
}
void ADC_DMA_CHNNAL_INIT(){
	ADC1_DMA_CHNNAL_INIT();
	ADC2_DMA_CHNNAL_INIT();
}
void DAC3CH1_DMA_INIT()
{
	RCC->AHB1ENR|=1<<0;//DMA1时钟使能 
	DAC3C1_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(DAC3C1_DMA->CCR&0X01);//等待DMA可配置 
	DMA1->IFCR|=0xf000;//清空之前该stream上的所有中断标志
	
	DAC3C1_DMA->CPAR=(u32)&DAC3->DHR12R1;		//DMA外设地址
	DAC3C1_DMA->CMAR=(u32)dac_ch1.dac_buf_p;//DMA 存储器0地址
	DAC3C1_DMA->CNDTR=0;	//DMA 传输数据长度
	DAC3C1_DMA->CCR=0;			//先全部复位CR寄存器值 
    
	DAC3C1_DMA->CCR|=1<<4;		//存储器到外设模式
	DAC3C1_DMA->CCR|=1<<5;		//非循环模式(即使用普通模式)
	DAC3C1_DMA->CCR|=0<<6;		//外设非增量模式
	DAC3C1_DMA->CCR|=1<<7;		//存储器增量模式
	DAC3C1_DMA->CCR|=2<<8;		//外设数据长度:16位
	DAC3C1_DMA->CCR|=2<<10;		//存储器数据长度:16位
	DAC3C1_DMA->CCR|=2<<12;		//中等优先级
	DAC3C1_DMA_MUX->CCR|=(u32)102;//通道选择

}
void DAC3CH1_DMA_START(u32 ndtr){
	DAC3C1_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(DAC3C1_DMA->CCR&0X01);//等待DMA可配置 	
	DAC3C1_DMA->CNDTR=ndtr;		//DMA 存储器0地址 
	DAC3C1_DMA->CCR|=1<<0;		//开启DMA传输
	TIM6->CR1|=1;
}

void DAC3C1_DMA_OFF(){
	DAC3C1_DMA->CCR&=~(1<<0);
}
void SPI3RX_DMA_INIT()
{
	RCC->AHB1ENR|=5<<0;//DMA1时钟使能 
	while(SPI3RX_DMA->CCR&0X01);//等待DMA可配置 
	DMA1->IFCR|=0Xf00000;//清空之前该stream上的所有中断标志

	SPI3RX_DMA->CPAR=(u32)&SPI3->DR;		//DMA外设地址
	SPI3RX_DMA->CNDTR=0;			//DMA 传输数据长度
	SPI3RX_DMA->CCR=0;			//先全部复位CR寄存器值 
	
	SPI3RX_DMA->CCR|=0<<4;		//存储器到外设模式
	SPI3RX_DMA->CCR|=0<<5;		//非循环模式(即使用普通模式)
	SPI3RX_DMA->CCR|=0<<6;		//外设非增量模式
	SPI3RX_DMA->CCR|=1<<7;		//存储器增量模式
	SPI3RX_DMA->CCR|=0<<8;		//外设数据长度:8位
	SPI3RX_DMA->CCR|=0<<10;		//存储器数据长度:8位
	SPI3RX_DMA->CCR|=1<<12;		//中等优先级
	SPI3RX_DMA_MUX->CCR=14;	//通道选择	
}
void SPI3RX_DMA_START(u32 ndtr,u32 buftr)
{
	SPI3->CR1&=~(1<<6);
	SPI3->CR2|=1<<0;				//开启SPI3 TX DMA
	SPI3->CR1|=1<<6;				//SPE=1,使能SPI3
	SPI3RX_DMA->CCR&=~(1<<0); 	//关闭DMA传输 
	while(SPI3RX_DMA->CCR&0X1);	//确保DMA可以被设置  
	SPI3RX_DMA->CNDTR=(u16)ndtr;	//DMA 传输数据长度
	SPI3RX_DMA->CMAR=buftr;	//DMA 存储器0地址
	SPI3RX_DMA->CCR|=1<<0;		//开启DMA传输

}
void SPI3RX_DMA_END(){
	while (!((DMA1->ISR & 1)));
	DMA1->IFCR|=0X7<<0;
	SPI3->CR1&=~(1<<6);
	SPI3->CR2&=~(1<<0);
	SPI3->CR1|=1<<6;				//SPE=1,使能SPI3	
}	
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,范围:0~7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  

void MYDMA_Config(DMA_Channel_TypeDef *DMA_Streamx,u8 chx,u32 par,u32 mar,u16 ndtr)
{ 
	DMA_TypeDef *DMAx=DMA1;
	u8 streamx;
	RCC->AHB1ENR|=1<<21;//DMA1时钟使能 
	while(DMA_Streamx->CCR&0X01);//等待DMA可配置 
	streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X8)/0X14;		//得到stream通道号
	DMAx->IFCR|=0X07<<(3*(streamx));//清空之前该stream上的所有中断标志
	
	DMA_Streamx->CPAR=par;		//DMA外设地址
	DMA_Streamx->CMAR=mar;		//DMA 存储器0地址
	DMA_Streamx->CNDTR=ndtr;	//DMA 传输数据长度
	DMA_Streamx->CCR=0;			//先全部复位CR寄存器值 
	
	DMA_Streamx->CCR|=1<<6;		//存储器到外设模式
	DMA_Streamx->CCR|=0<<8;		//非循环模式(即使用普通模式)
	DMA_Streamx->CCR|=0<<9;		//外设非增量模式
	DMA_Streamx->CCR|=1<<10;		//存储器增量模式
	DMA_Streamx->CCR|=0<<11;		//外设数据长度:8位
	DMA_Streamx->CCR|=0<<13;		//存储器数据长度:8位
	DMA_Streamx->CCR|=1<<16;		//中等优先级
	DMA_Streamx->CCR|=0<<21;		//外设突发单次传输
	DMA_Streamx->CCR|=0<<23;		//存储器突发单次传输
	DMA_Streamx->CCR|=(u32)chx<<25;//通道选择
	//DMA_Streamx->FCR=0X21;	//FIFO控制寄存器
} 
//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Channel_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Streamx->CCR&=~(1<<0); 	//关闭DMA传输 
	while(DMA_Streamx->CCR&0X1);	//确保DMA可以被设置  
	DMA_Streamx->CNDTR=ndtr;		//DMA 存储器0地址 
	DMA_Streamx->CCR|=1<<0;		//开启DMA传输
}	  
