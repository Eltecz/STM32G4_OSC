#include "hx8357_lcd_init.h"
//��׼ȷ��ʱ����


void 	lcd_gpio_init()
{
	RCC->AHB4ENR|=1<<1;
	GPIO_Set(GPIOB,PIN3|PIN4|PIN5|PIN6|PIN7,1,0,3,1);
//	GPIOE->BSRR|=31<<10;
//	GPIOE->BSRR|=1<<3;
}

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
#if hard_spi
	u8 i;
//	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
//  LCD_CS_Set();	
#else
//  LCD_CS_Clr();	
	SPI1_ReadWriteByte(dat);	
//  LCD_CS_Set();	
#endif
}
void LCD_Writ_Bus16b(u16 dat) 
{	
#if hard_spi
	u8 i;
//	LCD_CS_Clr();
	for(i=0;i<16;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x8000)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
//  LCD_CS_Set();	
#else
//  LCD_CS_Clr();	
	SPI1_ReadWriteByte(dat>>8);	
	SPI1_ReadWriteByte(dat);	
//  LCD_CS_Set();	
#endif
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
#if 0
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+24);
		LCD_WR_DATA(x2+24);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+24);
		LCD_WR_DATA(x2+24);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+24);
		LCD_WR_DATA(y2+24);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+24);
		LCD_WR_DATA(y2+24);
		LCD_WR_REG(0x2c);//������д
	}
#else
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
#endif
}

void LCD_Init(void)
{
	
	lcd_gpio_init();	
//	LCD_CS_Set();
	#if !hard_spi
	SPI1_Init();
//	SPI1_SetSpeed(6);
	#endif
	delay_ms(100);    
	LCD_CS_Clr();
//	LCD_RES_Clr();//��λ

//	LCD_RES_Set();
	
    delay_ms(100);
	
	LCD_WR_REG(0x11);//Sleep exit 
	delay_ms(20);                //Delay 120ms 
	LCD_WR_REG(0x21);    
	LCD_WR_REG(0x29);      
    delay_ms(10);
    LCD_WR_REG(0x3a);     
	LCD_WR_DATA8(0x55);   
    LCD_WR_REG(0x36);     
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0xc8);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x28);
	else LCD_WR_DATA8(0xe8);  
	LCD_BLK_Set();//�򿪱���   
}








