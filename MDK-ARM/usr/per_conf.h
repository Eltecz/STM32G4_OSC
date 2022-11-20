/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\per_conf.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-22 11:40:42
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-11-05 11:14:31
 */
#ifndef __per_conf_h__
#define __per_conf_h__

#define MAIN_FOSC   200
//如果出现代码不稳定的情况，请将MAIN_FOSC->180

//std lib
#include "stdio.h"
#include "string.h"
#include "arm_math.h"
//user lib

#include "malloc.h"

#include "sys.h"

#include "gpio.h"

#include "usart.h"

#include "delay.h"

#include "timer.h"

#define adc_length  300
#define adc_dma_length  ((adc_length)<<2)  

#include "adc.h"

#include "dma.h"


#include "spi.h"

//#include "sdmmc.h"
#include "w25qxx.h"
#include "fatfs_app.h"

#define dac_dma_length  500

#include "dac.h"
#include "opamp.h"

#define hard_spi
#define endma

#include "lcd.h"
//lcd type:
//#define ST7735_160_80
//#define ST7789_240_135
#define ST7789_320_172
#include "st77xx_lcd_init.h"



//#define user_usb_config

#ifdef  user_usb_config
    #include "usb_top.h"
#endif

#include "osc.h"

#endif