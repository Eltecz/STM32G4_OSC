/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\gpio.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-22 11:40:42
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 00:06:17
 */

#ifndef __gpio_h__
#define __gpio_h__
#include "per_conf.h"
void    gpio_init();
u8      getkey();
#define LED0(X) (GPIOA->BSRR|=(1<<((X?8:24))))
#define KM()    ((GPIOB->IDR)&((1<<8)))
#define KR()    ((GPIOA->IDR)&((1<<3)))
#define KL()    ((GPIOB->IDR)&((1<<0)))

enum{
	KL=1,KM,KR
};

#endif