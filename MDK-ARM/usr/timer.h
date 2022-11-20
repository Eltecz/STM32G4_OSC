/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\timer.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-03 20:29:41
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-09 23:22:28
 */
#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc); 
void TIM4_Int_Init(u16 arr,u16 psc);
void ADC_TIM_init(u8 tims);

extern u16 adc_delay[][2];
extern u16 pwm_lut[][2];
extern u16 dac_trig_lut[][2];

#define timer_length 16
#define Encoder_init()  TIM4_Int_Init(256,100)
#define Encoder_reset() ((TIM4->CNT)=128)
#define Encoder_get()   ((TIM4->CNT)-128)

void PWM_TIM_INIT(u8 t1,u8 t2,u8 d1,u8 d2);

void dac_timer_init(u8 t1,u8 t2);
#endif























