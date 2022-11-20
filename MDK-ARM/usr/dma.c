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
	RCC->AHB1ENR|=5<<0;//DMA1ʱ��ʹ�� 
	while(SPI1_LCD_DMA->CCR&0X01);//�ȴ�DMA������ 
	DMA1->IFCR|=0X07;//���֮ǰ��stream�ϵ������жϱ�־

	SPI1_LCD_DMA->CPAR=(u32)&SPI1->DR;		//DMA�����ַ
	SPI1_LCD_DMA->CNDTR=0;			//DMA �������ݳ���
	SPI1_LCD_DMA->CCR=0;			//��ȫ����λCR�Ĵ���ֵ 
	
	SPI1_LCD_DMA->CCR|=1<<4;		//�洢��������ģʽ
	SPI1_LCD_DMA->CCR|=0<<5;		//��ѭ��ģʽ(��ʹ����ͨģʽ)
	SPI1_LCD_DMA->CCR|=0<<6;		//���������ģʽ
	SPI1_LCD_DMA->CCR|=0<<7;		//�洢��������ģʽ
	SPI1_LCD_DMA->CCR|=0<<8;		//�������ݳ���:8λ
	SPI1_LCD_DMA->CCR|=1<<10;		//�洢�����ݳ���:8λ
	SPI1_LCD_DMA->CCR|=1<<12;		//�е����ȼ�
	SPI1_LCD_DMA_MUX->CCR=11;	//ͨ��ѡ��	
}
void SPI1_DMA_START(u32 ndtr,u32 buftr,u8 mode)
{
	SPI1->CR1&=~(1<<6);
	SPI1->CR2|=1<<1;				//����SPI1 TX DMA
	SPI1->CR1|=1<<6;				//SPE=1,ʹ��SPI1
	SPI1_LCD_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(SPI1_LCD_DMA->CCR&0X1);	//ȷ��DMA���Ա�����  
	SPI1_LCD_DMA->CNDTR=(u16)ndtr;	//DMA �������ݳ���
	SPI1_LCD_DMA->CMAR=buftr;	//DMA �洢��0��ַ
	if(mode){
		SPI1_LCD_DMA->CCR&=~(1<<7);		//���������ģʽ
		SPI1_LCD_DMA->CCR|=1<<10;		//�洢�����ݳ���:8λ		
	}
	else	{
		SPI1_LCD_DMA->CCR|=1<<7;		//���������ģʽ
//		SPI1_LCD_DMA->CCR&=~(1<<10);		//�洢�����ݳ���:8λ	
	}
	SPI1_LCD_DMA->CCR|=1<<0;		//����DMA����
	while (!((DMA1->ISR & 1)));
	DMA1->IFCR|=0X7<<0;
	SPI1->CR1&=~(1<<6);
	SPI1->CR2&=~(1<<1);
	SPI1->CR1|=1<<6;				//SPE=1,ʹ��SPI1
}	

void ADC1_DMA_CHNNAL_INIT()
{
	RCC->AHB1ENR|=1<<0;//DMA1ʱ��ʹ�� 
	ADC1_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(ADC1_DMA->CCR&0X01);//�ȴ�DMA������ 
	DMA1->IFCR|=0xf0;//���֮ǰ��stream�ϵ������жϱ�־
	
	ADC1_DMA->CPAR=(u32)&ADC1->DR;		//DMA�����ַ
	ADC1_DMA->CMAR=(u32)&adc1_buf;//DMA �洢��0��ַ
	ADC1_DMA->CNDTR=0;	//DMA �������ݳ���
	ADC1_DMA->CCR=0;			//��ȫ����λCR�Ĵ���ֵ 
    
//    ADC1_DMA->CCR|=1<<1;     //DMA�ж�
	ADC1_DMA->CCR|=0<<4;		//���赽�洢��ģʽ
	ADC1_DMA->CCR|=0<<5;		//��ѭ��ģʽ(��ʹ����ͨģʽ)
	ADC1_DMA->CCR|=0<<6;		//���������ģʽ
	ADC1_DMA->CCR|=1<<7;		//�洢������ģʽ
	ADC1_DMA->CCR|=1<<8;		//�������ݳ���:16λ
	ADC1_DMA->CCR|=1<<10;		//�洢�����ݳ���:16λ
	ADC1_DMA->CCR|=1<<12;		//�е����ȼ�
	ADC1_DMA_MUX->CCR|=(u32)5;//ͨ��ѡ��

}
void ADC1_DMA_START(u32 ndtr)
{
	ADC1_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(ADC1_DMA->CCR&0X1);	//ȷ��DMA���Ա�����  
	ADC1_DMA->CNDTR=ndtr;		//DMA �������ݳ���
	ADC1->CR|=1<<2;       //��������ת��ͨ�� 
	ADC1_DMA->CCR|=1<<0;		//����DMA����
	TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	TIM1->CCER|=1<<12;
	while (!((DMA1->ISR & 0x10)));
	DMA1->IFCR |= 0x10;
	TIM1->CR1-=0x01;    //ʹ�ܶ�ʱ��3
	TIM1->CCER-=1<<12;
}	

void ADC2_DMA_CHNNAL_INIT()
{
	RCC->AHB1ENR|=1<<0;//DMA1ʱ��ʹ�� 
	ADC2_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(ADC2_DMA->CCR&0X01);//�ȴ�DMA������ 
	DMA1->IFCR|=0xf00;//���֮ǰ��stream�ϵ������жϱ�־
	
	ADC2_DMA->CPAR=(u32)&ADC2->DR;		//DMA�����ַ
	ADC2_DMA->CMAR=(u32)&adc2_buf;//DMA �洢��0��ַ
	ADC2_DMA->CNDTR=0;	//DMA �������ݳ���
	ADC2_DMA->CCR=0;			//��ȫ����λCR�Ĵ���ֵ 
    
//    ADC2_DMA->CCR|=1<<1;     //DMA�ж�
	ADC2_DMA->CCR|=0<<4;		//���赽�洢��ģʽ
	ADC2_DMA->CCR|=0<<5;		//��ѭ��ģʽ(��ʹ����ͨģʽ)
	ADC2_DMA->CCR|=0<<6;		//���������ģʽ
	ADC2_DMA->CCR|=1<<7;		//�洢������ģʽ
	ADC2_DMA->CCR|=1<<8;		//�������ݳ���:16λ
	ADC2_DMA->CCR|=1<<10;		//�洢�����ݳ���:16λ
	ADC2_DMA->CCR|=1<<12;		//�е����ȼ�
	ADC2_DMA_MUX->CCR|=(u32)36;//ͨ��ѡ��

}

void ADC2_DMA_START(u32 ndtr)
{
	ADC2_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(ADC2_DMA->CCR&0X1);	//ȷ��DMA���Ա�����  
	ADC2_DMA->CNDTR=ndtr;		//DMA �������ݳ���
	ADC2->CR|=1<<2;       //��������ת��ͨ�� 
	ADC2_DMA->CCR|=1<<0;		//����DMA����
	TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	TIM1->CCER|=1<<12;
	while (!((DMA1->ISR & 0x100)));
	DMA1->IFCR |= 0x100;
	TIM1->CR1-=0x01;    //ʹ�ܶ�ʱ��3
	TIM1->CCER-=1<<12;
}	
void Dual_ADC_DMA_START(u32 ndtr)
{
	ADC1_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(ADC1_DMA->CCR&0X1);	//ȷ��DMA���Ա�����  
	ADC1_DMA->CNDTR=ndtr;		//DMA �������ݳ���
	ADC1->CR|=1<<2;       //��������ת��ͨ�� 
	ADC1_DMA->CCR|=1<<0;		//����DMA����	
	ADC2_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(ADC2_DMA->CCR&0X1);	//ȷ��DMA���Ա�����  
	ADC2_DMA->CNDTR=ndtr;		//DMA �������ݳ���
	ADC2->CR|=1<<2;       //��������ת��ͨ�� 
	ADC2_DMA->CCR|=1<<0;		//����DMA����
	TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	TIM1->CCER|=1<<12;	
	while (!((DMA1->ISR & 0x10)));	
	while (!((DMA1->ISR & 0x100)));	
	DMA1->IFCR |= 0x110;	
	TIM1->CR1-=0x01;    //ʹ�ܶ�ʱ��3
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
	RCC->AHB1ENR|=1<<0;//DMA1ʱ��ʹ�� 
	DAC3C1_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(DAC3C1_DMA->CCR&0X01);//�ȴ�DMA������ 
	DMA1->IFCR|=0xf000;//���֮ǰ��stream�ϵ������жϱ�־
	
	DAC3C1_DMA->CPAR=(u32)&DAC3->DHR12R1;		//DMA�����ַ
	DAC3C1_DMA->CMAR=(u32)dac_ch1.dac_buf_p;//DMA �洢��0��ַ
	DAC3C1_DMA->CNDTR=0;	//DMA �������ݳ���
	DAC3C1_DMA->CCR=0;			//��ȫ����λCR�Ĵ���ֵ 
    
	DAC3C1_DMA->CCR|=1<<4;		//�洢��������ģʽ
	DAC3C1_DMA->CCR|=1<<5;		//��ѭ��ģʽ(��ʹ����ͨģʽ)
	DAC3C1_DMA->CCR|=0<<6;		//���������ģʽ
	DAC3C1_DMA->CCR|=1<<7;		//�洢������ģʽ
	DAC3C1_DMA->CCR|=2<<8;		//�������ݳ���:16λ
	DAC3C1_DMA->CCR|=2<<10;		//�洢�����ݳ���:16λ
	DAC3C1_DMA->CCR|=2<<12;		//�е����ȼ�
	DAC3C1_DMA_MUX->CCR|=(u32)102;//ͨ��ѡ��

}
void DAC3CH1_DMA_START(u32 ndtr){
	DAC3C1_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(DAC3C1_DMA->CCR&0X01);//�ȴ�DMA������ 	
	DAC3C1_DMA->CNDTR=ndtr;		//DMA �洢��0��ַ 
	DAC3C1_DMA->CCR|=1<<0;		//����DMA����
	TIM6->CR1|=1;
}

void DAC3C1_DMA_OFF(){
	DAC3C1_DMA->CCR&=~(1<<0);
}
void SPI3RX_DMA_INIT()
{
	RCC->AHB1ENR|=5<<0;//DMA1ʱ��ʹ�� 
	while(SPI3RX_DMA->CCR&0X01);//�ȴ�DMA������ 
	DMA1->IFCR|=0Xf00000;//���֮ǰ��stream�ϵ������жϱ�־

	SPI3RX_DMA->CPAR=(u32)&SPI3->DR;		//DMA�����ַ
	SPI3RX_DMA->CNDTR=0;			//DMA �������ݳ���
	SPI3RX_DMA->CCR=0;			//��ȫ����λCR�Ĵ���ֵ 
	
	SPI3RX_DMA->CCR|=0<<4;		//�洢��������ģʽ
	SPI3RX_DMA->CCR|=0<<5;		//��ѭ��ģʽ(��ʹ����ͨģʽ)
	SPI3RX_DMA->CCR|=0<<6;		//���������ģʽ
	SPI3RX_DMA->CCR|=1<<7;		//�洢������ģʽ
	SPI3RX_DMA->CCR|=0<<8;		//�������ݳ���:8λ
	SPI3RX_DMA->CCR|=0<<10;		//�洢�����ݳ���:8λ
	SPI3RX_DMA->CCR|=1<<12;		//�е����ȼ�
	SPI3RX_DMA_MUX->CCR=14;	//ͨ��ѡ��	
}
void SPI3RX_DMA_START(u32 ndtr,u32 buftr)
{
	SPI3->CR1&=~(1<<6);
	SPI3->CR2|=1<<0;				//����SPI3 TX DMA
	SPI3->CR1|=1<<6;				//SPE=1,ʹ��SPI3
	SPI3RX_DMA->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(SPI3RX_DMA->CCR&0X1);	//ȷ��DMA���Ա�����  
	SPI3RX_DMA->CNDTR=(u16)ndtr;	//DMA �������ݳ���
	SPI3RX_DMA->CMAR=buftr;	//DMA �洢��0��ַ
	SPI3RX_DMA->CCR|=1<<0;		//����DMA����

}
void SPI3RX_DMA_END(){
	while (!((DMA1->ISR & 1)));
	DMA1->IFCR|=0X7<<0;
	SPI3->CR1&=~(1<<6);
	SPI3->CR2&=~(1<<0);
	SPI3->CR1|=1<<6;				//SPE=1,ʹ��SPI3	
}	
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,��Χ:0~7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  

void MYDMA_Config(DMA_Channel_TypeDef *DMA_Streamx,u8 chx,u32 par,u32 mar,u16 ndtr)
{ 
	DMA_TypeDef *DMAx=DMA1;
	u8 streamx;
	RCC->AHB1ENR|=1<<21;//DMA1ʱ��ʹ�� 
	while(DMA_Streamx->CCR&0X01);//�ȴ�DMA������ 
	streamx=(((u32)DMA_Streamx-(u32)DMAx)-0X8)/0X14;		//�õ�streamͨ����
	DMAx->IFCR|=0X07<<(3*(streamx));//���֮ǰ��stream�ϵ������жϱ�־
	
	DMA_Streamx->CPAR=par;		//DMA�����ַ
	DMA_Streamx->CMAR=mar;		//DMA �洢��0��ַ
	DMA_Streamx->CNDTR=ndtr;	//DMA �������ݳ���
	DMA_Streamx->CCR=0;			//��ȫ����λCR�Ĵ���ֵ 
	
	DMA_Streamx->CCR|=1<<6;		//�洢��������ģʽ
	DMA_Streamx->CCR|=0<<8;		//��ѭ��ģʽ(��ʹ����ͨģʽ)
	DMA_Streamx->CCR|=0<<9;		//���������ģʽ
	DMA_Streamx->CCR|=1<<10;		//�洢������ģʽ
	DMA_Streamx->CCR|=0<<11;		//�������ݳ���:8λ
	DMA_Streamx->CCR|=0<<13;		//�洢�����ݳ���:8λ
	DMA_Streamx->CCR|=1<<16;		//�е����ȼ�
	DMA_Streamx->CCR|=0<<21;		//����ͻ�����δ���
	DMA_Streamx->CCR|=0<<23;		//�洢��ͻ�����δ���
	DMA_Streamx->CCR|=(u32)chx<<25;//ͨ��ѡ��
	//DMA_Streamx->FCR=0X21;	//FIFO���ƼĴ���
} 
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void MYDMA_Enable(DMA_Channel_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Streamx->CCR&=~(1<<0); 	//�ر�DMA���� 
	while(DMA_Streamx->CCR&0X1);	//ȷ��DMA���Ա�����  
	DMA_Streamx->CNDTR=ndtr;		//DMA �洢��0��ַ 
	DMA_Streamx->CCR|=1<<0;		//����DMA����
}	  
