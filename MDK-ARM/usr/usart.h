/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\usart.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-21 23:46:33
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-08-21 23:54:00
 */
#ifndef __usart_h__
#define __usart_h__
void uart1_init(u32 pclk2,u32 bound); 
void uart1_sendonebyte(u8 dat);//发送一个字节
#endif