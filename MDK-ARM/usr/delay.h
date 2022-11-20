/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\delay.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-22 11:40:42
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-05 23:55:38
 */
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "per_conf.h"	  

void delay_init(u16 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























