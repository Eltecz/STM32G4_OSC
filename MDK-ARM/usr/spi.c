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
//以下是SPI1模块的初始化代码，配置成主机模式 						  
//SPI1口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
#ifdef	endma
	SPI1_LCD_DMA_INIT();
#endif
	u16 tempreg=0;
//	RCC->AHB2ENR|=1<<1;			//使能PORTB时钟 
	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN_Msk;		//SPI1时钟使能 
	GPIO_Set(GPIOB,PIN3|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,3,GPIO_PUPD_NONE);	//PB13~15复用功能输出	
 	GPIO_AF_Set(GPIOB,5,5);	//PB14,AF5
 	GPIO_AF_Set(GPIOB,3,5);	//PB15,AF5
	GPIOB->BSRR|=0X5<<3;
 //	GPIO_AF_Set(GPIOE,7,5);	//PB15,AF5 	
	//这里只针对SPI1口初始化
	RCC->APB1RSTR1|=1<<14;		//复位SPI1
	RCC->APB1RSTR1&=~(1<<14);	//停止复位SPI1
	
	SPI1->CR1|=1<<8;			//SSI=1,设置内部SS信号为高电平	
	SPI1->CR1|=0<<3;			//MBR[2:0]=7,设置SPI1_ker_ck为256分频.
	SPI1->CR2|=7<<8;			//DSIZE[4:0]=7,设置SPI1帧格式为8位,即字节传输
	SPI1->CR2|=0<<2;				//SSOE=0,禁止硬件NSS输出
	SPI1->CR1|=1<<9;				//SSM=1,软件管理NSS脚
	SPI1->CR1|=1<<1;				//CPOL=0,空闲状态下,SCK为低电平
	SPI1->CR1|=1<<0;				//CPHA=1,数据采样从第1个时间边沿开始
	SPI1->CR1|=0<<7;				//LSBFRST=0,MSB先传输
	SPI1->CR1|=1<<2;				//MSTR=1,主机模式
	SPI1->CR2|=1<<12;				//fifo大小为8b
	SPI1->SR|=1<<11;
	SPI1->CR2|=0<<4;				//FRF=0,摩托罗拉格式
//	SPI1->CR1|=1<<10;				//RXONLY=0,全双工通信
	SPI1->I2SCFGR&=~(1<<0);		//选择SPI1模式
	SPI1->CR1|=1<<6;			//SPE=1,使能SPI1

	SPI1_WriteByte(0xff);	//启动传输		
	SPI1_SetSpeed(0);

}   

//SPI1速度设置函数
//SpeedSet:0~7
//SPI1速度=SPI1_ker_ck/2^(SpeedSet+1)
//SPI1_ker_ck我们选择来自pll1_q_ck,为200Mhz
void SPI1_SetSpeed(u8 SpeedSet)
{ 
	SpeedSet&=0X07;					//限制范围
 	SPI1->CR1&=~(1<<6); 			//SPE=0,SPI1设备失能
	SPI1->CR1&=~(7<<3); 			//MBR[2:0]=0,清除原来的分频设置
	SPI1->CR1|=(u16)SpeedSet<<3;	//MBR[2:0]=SpeedSet,设置SPI1速度  
	SPI1->CR1|=1<<6; 				//SPE=1,SPI1设备使能	 	  
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{			 	   
	u8 RxData=0;	
	SPI1->CR1|=1<<6;  				//CSTART=1,启动传输
	

	*(vu8 *)&SPI1->DR=TxData;		//发送一个byte,以传输长度访问TXDR寄存器   
	RxData=*(vu8 *)&SPI1->DR;		//接收一个byte,以传输长度访问RXDR寄存器	
	while(((SPI1->SR)&(1<<1))==0);		//等待发送区空 	
	SPI1->SR|=3<<5;				//EOTC和TXTFC置1,清除EOT和TXTFC位 
	SPI1->CR1-=(1<<6);  				//CSTART=1,启动传输
	return RxData;					//返回收到的数据
}

void SPI1_WriteByte(u8 TxData)
{			 	  
	*(vu8 *)&SPI1->DR=TxData;		//发送一个byte,以传输长度访问TXDR寄存器  
	while(((SPI1->SR)&(3<<9))==0);		//等待发送区空 	 
	SPI1->SR|=3<<5;				//EOTC和TXTFC置1,清除EOT和TXTFC位 
}


//以下是SPI3模块的初始化代码，配置成主机模式 						  
//SPI3口初始化
//这里针是对SPI3的初始化
void SPI3_Init(void)
{	 
#ifdef	endma
//	SPI3_LCD_DMA_INIT();
#endif
	u16 tempreg=0;
	RCC->AHB2ENR|=1<<2;			//使能PORTB时钟 
	RCC->APB1ENR1|=1<<15;		//SPI3时钟使能 
//	GPIO_Set_withAF(GPIOE,PIN15,GPIO_OTYPE_PP,3,GPIO_PUPD_NONE,5);	//PB13~15复用功能输出	
	GPIO_Set_withAF(GPIOC,PIN10|PIN11|PIN12,GPIO_OTYPE_PP,3,GPIO_PUPD_NONE,6);	//PB13~15复用功能输出	
	//这里只针对SPI3口初始化
	
	SPI3->CR1|=1<<8;			//SSI=1,设置内部SS信号为高电平	
	SPI3->CR1|=0<<3;			//MBR[2:0]=7,设置SPI3_ker_ck为256分频.
	SPI3->CR2|=7<<8;			//DSIZE[4:0]=7,设置SPI3帧格式为8位,即字节传输
	SPI3->CR2|=0<<2;				//SSOE=0,禁止硬件NSS输出
	SPI3->CR1|=1<<9;				//SSM=1,软件管理NSS脚
	SPI3->CR1|=0<<1;				//CPOL=0,空闲状态下,SCK为低电平
	SPI3->CR1|=0<<0;				//CPHA=1,数据采样从第1个时间边沿开始
	SPI3->CR1|=0<<7;				//LSBFRST=0,MSB先传输
	SPI3->CR1|=1<<2;				//MSTR=1,主机模式
	SPI3->CR2|=1<<12;				//fifo大小为8b
	SPI3->SR|=1<<11;
	SPI3->CR2|=0<<4;				//FRF=0,摩托罗拉格式
//	SPI3->CR1|=1<<10;				//RXONLY=0,全双工通信
	SPI3->I2SCFGR&=~(1<<0);		//选择SPI3模式
	SPI3->CR1|=1<<6;			//SPE=1,使能SPI3

//	SPI3_WriteByte(0xff);	//启动传输		
//	SPI3_SetSpeed(2);

}   

//SPI3速度设置函数
//SpeedSet:0~7
//SPI3速度=SPI3_ker_ck/2^(SpeedSet+1)
//SPI3_ker_ck我们选择来自pll1_q_ck,为200Mhz
void SPI3_SetSpeed(u8 SpeedSet)
{ 
	SpeedSet&=0X07;					//限制范围
 	SPI3->CR1&=~(1<<6); 			//SPE=0,SPI3设备失能
	SPI3->CR1&=~(7<<3); 			//MBR[2:0]=0,清除原来的分频设置
	SPI3->CR1|=(u16)SpeedSet<<3;	//MBR[2:0]=SpeedSet,设置SPI3速度  
	SPI3->CR1|=1<<6; 				//SPE=1,SPI3设备使能	 	  
} 

//SPI3 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{			 	   
	u8 RxData=0;	
	

	*(vu8 *)&SPI3->DR=TxData;		//发送一个byte,以传输长度访问TXDR寄存器  
	while(((SPI3->SR)&(1<<1))==0);		//等待发送区空 
	delay_us(5);
	if(((SPI3->SR)&(1<<0))){			//等待发送区空 	
		RxData=*(vu8 *)&SPI3->DR;		//接收一个byte,以传输长度访问RXDR寄存器	
	};					
	
		
	SPI3->SR|=3<<5;				//EOTC和TXTFC置1,清除EOT和TXTFC位 
	return RxData;					//返回收到的数据
}

void SPI3_WriteByte(u8 TxData)
{	
	u8 RxData=0;		 	  
	*(vu8 *)&SPI3->DR=TxData;		//发送一个byte,以传输长度访问TXDR寄存器  
	while(((SPI3->SR)&(1<<1))==0);		//等待发送区空 
	while(((SPI3->SR)&(1<<0))==0);			//等待发送区空 	
	RxData=*(vu8 *)&SPI3->DR;		//接收一个byte,以传输长度访问RXDR寄存器	
}

u8 SPI3_ReadByte(u8 TxData)
{			 	   
	*(vu8 *)&SPI3->DR=TxData;		//发送一个byte,以传输长度访问TXDR寄存器  
	while(((SPI3->SR)&(1<<0))==0);			//等待发送区空 		
	return *(vu8 *)&SPI3->DR;			//接收一个byte,以传输长度访问RXDR寄存器,返回收到的数据
}





