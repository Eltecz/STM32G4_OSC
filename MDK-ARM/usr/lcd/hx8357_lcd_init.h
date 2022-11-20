#ifndef __hx8357_lcd_init_h
#define __hx8357_lcd_init_h

#include "per_conf.h"

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define STM32 1

#define hard_spi 0  //改为0可实现硬件spi通信
#define bit16 1

#if USE_HORIZONTAL==3||USE_HORIZONTAL==1
#define LCD_W 480
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 480
#endif
	
#if !STM32

#define LCD_SCL P1^0;//SCLK
#define LCD_SDA P1^1;//MOSI
#define LCD_RES P1^2;//RES
#define LCD_DC  P1^3;//DC
#define LCD_CS  P1^4; //CS
#define LCD_BLK P1^5; //BLK

//-----------------LCD端口定义----------------

#define LCD_SCLK_Clr() LCD_SCL=0//SCL=SCLK
#define LCD_SCLK_Set() LCD_SCL=1

#define LCD_MOSI_Clr() LCD_SDA=0//SDA=MOSI
#define LCD_MOSI_Set() LCD_SDA=1

#define LCD_RES_Clr() LCD_RES=0//RES
#define LCD_RES_Set() LCD_RES=1

#define LCD_DC_Clr() LCD_DC=0//DC
#define LCD_DC_Set() LCD_DC=1

#define LCD_CS_Clr()  LCD_CS=0//CS
#define LCD_CS_Set()  LCD_CS=1

#define LCD_BLK_Clr()  LCD_BLK=0//BLK
#define LCD_BLK_Set()  LCD_BLK=1

#else
/*
#define LCD_SCL(X) ((GPIOE->BSRR)=(BSRR|(X<<(12+(X*16)))))//SCLK
#define LCD_SDA(X) ((GPIOE->BSRR)=(BSRR|(X<<(14+(X*16)))))//MOSI
//#define LCD_RES(X) ((GPIOE->BSRR)=(BSRR|(X<<(12+(X*16)))));//RES
#define LCD_DC(X) ((GPIOE->BSRR)=(BSRR|(X<<(13+(X*16)))))//DC
#define LCD_CS(X) ((GPIOE->BSRR)=(BSRR|(X<<(11+(X*16))))) //CS
#define LCD_BLK(X) ((GPIOE->BSRR)=(BSRR|(X<<(10+(X*16))))) //BLK

#define LCD_SCLK_Clr() LCD_SCL(1)//SCL=SCLK
#define LCD_SCLK_Set() LCD_SCL(0)

#define LCD_MOSI_Clr() LCD_SDA(1)//SDA=MOSI
#define LCD_MOSI_Set() LCD_SDA(0)

//#define LCD_RES_Clr() LCD_RES(1)//RES
//#define LCD_RES_Set() LCD_RES(0)

#define LCD_DC_Clr() LCD_DC(1)//DC
#define LCD_DC_Set() LCD_DC(0)

#define LCD_CS_Clr()  LCD_CS(1)//CS
#define LCD_CS_Set()  LCD_CS(0)

#define LCD_BLK_Clr()  LCD_BLK(1)//BLK
#define LCD_BLK_Set()  LCD_BLK(0)
*/
#define LCD_SCLK_Clr() GPIOB->BSRR|=1<<19//SCL=SCLK
#define LCD_SCLK_Set() GPIOB->BSRR|=1<<3

#define LCD_MOSI_Clr() GPIOB->BSRR|=1<<21//SDA=MOSI
#define LCD_MOSI_Set() GPIOB->BSRR|=1<<5

#define LCD_DC_Clr() GPIOB->BSRR|=1<<22//DC
#define LCD_DC_Set() GPIOB->BSRR|=1<<6

#define LCD_CS_Clr()  GPIOB->BSRR|=1<<20//CS
#define LCD_CS_Set()  GPIOB->BSRR|=1<<4

#define LCD_BLK_Clr()  GPIOB->BSRR|=1<<10//BLK
#define LCD_BLK_Set()  GPIOB->BSRR|=1<<26
#endif

//void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_Writ_Bus16b(u16 dat) ;
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

#ifdef OLD_SCREEN
#define WHITE (0xffff - 0xFFFF)
#define BLACK (0xffff - 0x0000)
#define BLUE (0xffff - 0x001F)
#define BRED (0xffff - 0xF81F)
#define GRED (0xffff - 0xFFE0)
#define GBLUE (0xffff - 0x07FF)
#define RED (0xffff - 0xF800)
#define MAGENTA (0xffff - 0xF81F)
#define GREEN (0xffff - 0x07E0)
#define CYAN (0xffff - 0x7FFF)
#define YELLOW (0xffff - 0xFFE0)
#define BROWN (0xffff - 0xBC40)     
#define BRRED (0xffff - 0xFC07)     
#define GRAY (0xffff - 0x8430)      
#define DARKBLUE (0xffff - 0x01CF)   
#define LIGHTBLUE (0xffff - 0x7D7C)  
#define GRAYBLUE (0xffff - 0x5458)   
#define LIGHTGREEN (0xffff - 0x841F) 
#define LGRAY (0xffff - 0xC618)      
#define LGRAYBLUE (0xffff - 0xA651)  
#define LBBLUE (0xffff - 0x2B12)     
#else
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0xF81F
#define GRED 0xFFE0
#define GBLUE 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0xBC40  
#define BRRED 0xFC07  
#define GRAY 0x8430     
#define DARKBLUE 0x01CF   
#define LIGHTBLUE 0x7D7C 
#define GRAYBLUE 0x5458   
#define LIGHTGREEN 0x841F 
#define LGRAY 0xC618    
#define LGRAYBLUE 0xA651  
#define LBBLUE 0x2B12    
#endif
#endif




