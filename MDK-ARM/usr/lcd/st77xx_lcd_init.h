#ifndef __lcd_init_h
#define __lcd_init_h

#include "per_conf.h"

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define STM32 1

//#define hard_spi
#define bit16 1

#ifdef ST7735_160_80
    #define OLD_SCREEN //中景园的屏幕由于批次原因，部分屏幕的时序不同，当驱动屏幕出错时可修改此参数来修正屏幕显示出错

    #if USE_HORIZONTAL==0||USE_HORIZONTAL==1
    #define LCD_W 80
    #define LCD_H 160
    #else
    #define LCD_W 160
    #define LCD_H 80
    #endif

    #ifdef  OLD_SCREEN
    #define fix_x 1
    #define fix_y 2
    #endif

#endif
#ifdef  ST7789_240_135
    #if USE_HORIZONTAL==0||USE_HORIZONTAL==1
    #define LCD_W 135
    #define LCD_H 240
    #else
    #define LCD_W 240
    #define LCD_H 135
    #endif
#endif
#ifdef  ST7789_320_172
    #if USE_HORIZONTAL==0||USE_HORIZONTAL==1
    #define LCD_W 172
    #define LCD_H 320
    #else
    #define LCD_W 320
    #define LCD_H 172
    #endif
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
#define LCD_SCLK_Clr() ((GPIOB->BSRR)|=(1<<19))//SCL=SCLK
#define LCD_SCLK_Set() ((GPIOB->BSRR)|=(1<<3))

#define LCD_MOSI_Clr() ((GPIOB->BSRR)|=(1<<21))//SDA=MOSI
#define LCD_MOSI_Set() ((GPIOB->BSRR)|=(1<<5))

#define LCD_DC_Clr() ((GPIOB->BSRR)|=(1<<20))//DC
#define LCD_DC_Set() ((GPIOB->BSRR)|=(1<<4))

#define LCD_CS_Clr()  ((GPIOA->BSRR)|=(1<<31))//CS
#define LCD_CS_Set()  ((GPIOA->BSRR)|=(1<<15))
#endif

//void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_Writ_Bus16b(u16 dat) ;
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

#endif




