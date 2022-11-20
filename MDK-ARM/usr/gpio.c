/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\gpio.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-22 11:40:42
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 09:44:21
 */
#include "per_conf.h"
void gpio_init(){
    RCC->AHB2ENR|=3;
    GPIO_Set(GPIOA,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_FAST,GPIO_PUPD_PU);
    GPIO_Set(GPIOB,PIN0|PIN8,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);   
    GPIO_Set(GPIOA,PIN3,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);   
}

u8 getkey(){
	if (KR())	return	1;
	if (KL())	return	3;
	if (KM())	return	2;
	return 0;
}
