/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\spi.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2021-08-16 16:23:32
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-11-05 10:10:07
 */
#include "spi.h"
#include "sys.h"
//#include "dma.h"
//������SPI1ģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI1�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{	 
#ifdef	endma
	SPI1_LCD_DMA_INIT();
#endif
	u16 tempreg=0;
//	RCC->AHB2ENR|=1<<1;			//ʹ��PORTBʱ�� 
	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN_Msk;		//SPI1ʱ��ʹ�� 
	GPIO_Set(GPIOB,PIN3|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,3,GPIO_PUPD_NONE);	//PB13~15���ù������	
 	GPIO_AF_Set(GPIOB,5,5);	//PB14,AF5
 	GPIO_AF_Set(GPIOB,3,5);	//PB15,AF5
	GPIOB->BSRR|=0X5<<3;
 //	GPIO_AF_Set(GPIOE,7,5);	//PB15,AF5 	
	//����ֻ���SPI1�ڳ�ʼ��
	RCC->APB1RSTR1|=1<<14;		//��λSPI1
	RCC->APB1RSTR1&=~(1<<14);	//ֹͣ��λSPI1
	
	SPI1->CR1|=1<<8;			//SSI=1,�����ڲ�SS�ź�Ϊ�ߵ�ƽ	
	SPI1->CR1|=0<<3;			//MBR[2:0]=7,����SPI1_ker_ckΪ256��Ƶ.
	SPI1->CR2|=7<<8;			//DSIZE[4:0]=7,����SPI1֡��ʽΪ8λ,���ֽڴ���
	SPI1->CR2|=0<<2;				//SSOE=0,��ֹӲ��NSS���
	SPI1->CR1|=1<<9;				//SSM=1,�������NSS��
	SPI1->CR1|=1<<1;				//CPOL=0,����״̬��,SCKΪ�͵�ƽ
	SPI1->CR1|=1<<0;				//CPHA=1,���ݲ����ӵ�1��ʱ����ؿ�ʼ
	SPI1->CR1|=0<<7;				//LSBFRST=0,MSB�ȴ���
	SPI1->CR1|=1<<2;				//MSTR=1,����ģʽ
	SPI1->CR2|=1<<12;				//fifo��СΪ8b
	SPI1->SR|=1<<11;
	SPI1->CR2|=0<<4;				//FRF=0,Ħ��������ʽ
//	SPI1->CR1|=1<<10;				//RXONLY=0,ȫ˫��ͨ��
	SPI1->I2SCFGR&=~(1<<0);		//ѡ��SPI1ģʽ
	SPI1->CR1|=1<<6;			//SPE=1,ʹ��SPI1

	SPI1_WriteByte(0xff);	//��������		
	SPI1_SetSpeed(0);

}   

//SPI1�ٶ����ú���
//SpeedSet:0~7
//SPI1�ٶ�=SPI1_ker_ck/2^(SpeedSet+1)
//SPI1_ker_ck����ѡ������pll1_q_ck,Ϊ200Mhz
void SPI1_SetSpeed(u8 SpeedSet)
{ 
	SpeedSet&=0X07;					//���Ʒ�Χ
 	SPI1->CR1&=~(1<<6); 			//SPE=0,SPI1�豸ʧ��
	SPI1->CR1&=~(7<<3); 			//MBR[2:0]=0,���ԭ���ķ�Ƶ����
	SPI1->CR1|=(u16)SpeedSet<<3;	//MBR[2:0]=SpeedSet,����SPI1�ٶ�  
	SPI1->CR1|=1<<6; 				//SPE=1,SPI1�豸ʹ��	 	  
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{			 	   
	u8 RxData=0;	
	SPI1->CR1|=1<<6;  				//CSTART=1,��������
	

	*(vu8 *)&SPI1->DR=TxData;		//����һ��byte,�Դ��䳤�ȷ���TXDR�Ĵ���   
	RxData=*(vu8 *)&SPI1->DR;		//����һ��byte,�Դ��䳤�ȷ���RXDR�Ĵ���	
	while(((SPI1->SR)&(1<<1))==0);		//�ȴ��������� 	
	SPI1->SR|=3<<5;				//EOTC��TXTFC��1,���EOT��TXTFCλ 
	SPI1->CR1-=(1<<6);  				//CSTART=1,��������
	return RxData;					//�����յ�������
}

void SPI1_WriteByte(u8 TxData)
{			 	  
	*(vu8 *)&SPI1->DR=TxData;		//����һ��byte,�Դ��䳤�ȷ���TXDR�Ĵ���  
	while(((SPI1->SR)&(3<<9))==0);		//�ȴ��������� 	 
	SPI1->SR|=3<<5;				//EOTC��TXTFC��1,���EOT��TXTFCλ 
}


//������SPI3ģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI3�ڳ�ʼ��
//�������Ƕ�SPI3�ĳ�ʼ��
void SPI3_Init(void)
{	 
#ifdef	endma
//	SPI3_LCD_DMA_INIT();
#endif
	u16 tempreg=0;
	RCC->AHB2ENR|=1<<2;			//ʹ��PORTBʱ�� 
	RCC->APB1ENR1|=1<<15;		//SPI3ʱ��ʹ�� 
//	GPIO_Set_withAF(GPIOE,PIN15,GPIO_OTYPE_PP,3,GPIO_PUPD_NONE,5);	//PB13~15���ù������	
	GPIO_Set_withAF(GPIOC,PIN10|PIN11|PIN12,GPIO_OTYPE_PP,3,GPIO_PUPD_NONE,6);	//PB13~15���ù������	
	//����ֻ���SPI3�ڳ�ʼ��
	
	SPI3->CR1|=1<<8;			//SSI=1,�����ڲ�SS�ź�Ϊ�ߵ�ƽ	
	SPI3->CR1|=0<<3;			//MBR[2:0]=7,����SPI3_ker_ckΪ256��Ƶ.
	SPI3->CR2|=7<<8;			//DSIZE[4:0]=7,����SPI3֡��ʽΪ8λ,���ֽڴ���
	SPI3->CR2|=0<<2;				//SSOE=0,��ֹӲ��NSS���
	SPI3->CR1|=1<<9;				//SSM=1,�������NSS��
	SPI3->CR1|=0<<1;				//CPOL=0,����״̬��,SCKΪ�͵�ƽ
	SPI3->CR1|=0<<0;				//CPHA=1,���ݲ����ӵ�1��ʱ����ؿ�ʼ
	SPI3->CR1|=0<<7;				//LSBFRST=0,MSB�ȴ���
	SPI3->CR1|=1<<2;				//MSTR=1,����ģʽ
	SPI3->CR2|=1<<12;				//fifo��СΪ8b
	SPI3->SR|=1<<11;
	SPI3->CR2|=0<<4;				//FRF=0,Ħ��������ʽ
//	SPI3->CR1|=1<<10;				//RXONLY=0,ȫ˫��ͨ��
	SPI3->I2SCFGR&=~(1<<0);		//ѡ��SPI3ģʽ
	SPI3->CR1|=1<<6;			//SPE=1,ʹ��SPI3

//	SPI3_WriteByte(0xff);	//��������		
//	SPI3_SetSpeed(2);

}   

//SPI3�ٶ����ú���
//SpeedSet:0~7
//SPI3�ٶ�=SPI3_ker_ck/2^(SpeedSet+1)
//SPI3_ker_ck����ѡ������pll1_q_ck,Ϊ200Mhz
void SPI3_SetSpeed(u8 SpeedSet)
{ 
	SpeedSet&=0X07;					//���Ʒ�Χ
 	SPI3->CR1&=~(1<<6); 			//SPE=0,SPI3�豸ʧ��
	SPI3->CR1&=~(7<<3); 			//MBR[2:0]=0,���ԭ���ķ�Ƶ����
	SPI3->CR1|=(u16)SpeedSet<<3;	//MBR[2:0]=SpeedSet,����SPI3�ٶ�  
	SPI3->CR1|=1<<6; 				//SPE=1,SPI3�豸ʹ��	 	  
} 

//SPI3 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{			 	   
	u8 RxData=0;	
	

	*(vu8 *)&SPI3->DR=TxData;		//����һ��byte,�Դ��䳤�ȷ���TXDR�Ĵ���  
	while(((SPI3->SR)&(1<<1))==0);		//�ȴ��������� 
	delay_us(5);
	if(((SPI3->SR)&(1<<0))){			//�ȴ��������� 	
		RxData=*(vu8 *)&SPI3->DR;		//����һ��byte,�Դ��䳤�ȷ���RXDR�Ĵ���	
	};					
	
		
	SPI3->SR|=3<<5;				//EOTC��TXTFC��1,���EOT��TXTFCλ 
	return RxData;					//�����յ�������
}

void SPI3_WriteByte(u8 TxData)
{	
	u8 RxData=0;		 	  
	*(vu8 *)&SPI3->DR=TxData;		//����һ��byte,�Դ��䳤�ȷ���TXDR�Ĵ���  
	while(((SPI3->SR)&(1<<1))==0);		//�ȴ��������� 
	while(((SPI3->SR)&(1<<0))==0);			//�ȴ��������� 	
	RxData=*(vu8 *)&SPI3->DR;		//����һ��byte,�Դ��䳤�ȷ���RXDR�Ĵ���	
}

u8 SPI3_ReadByte(u8 TxData)
{			 	   
	*(vu8 *)&SPI3->DR=TxData;		//����һ��byte,�Դ��䳤�ȷ���TXDR�Ĵ���  
	while(((SPI3->SR)&(1<<0))==0);			//�ȴ��������� 		
	return *(vu8 *)&SPI3->DR;			//����һ��byte,�Դ��䳤�ȷ���RXDR�Ĵ���,�����յ�������
}





