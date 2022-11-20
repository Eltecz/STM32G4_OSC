/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\osc.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-06-21 15:42:04
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 09:52:25
 */
#ifndef __osc_h__
#define __osc_h__

#include "per_conf.h"

#define adc_length_d2   (adc_length>>1)
#define adc_length_25   ((adc_length*3)>>2)
#define adc_dma_length  (adc_length<<2)    

#define key_seletion_color	RED

#define OSC_COLOR	GREEN
#define PWM_COLOR	BLUE
#define DAC_COLOR	RED
#define FFT_COLOR	MAGENTA
#define EX_COLOR	YELLOW

#define func_sel(x)			((x==key_seletion))
#define func_hit(x)			((key_seletion>0X80)&&((key_seletion%0X80)==x))
#define func_wcolor(x,c0,c1,c2)		((func_sel(x)?c1:(func_hit(x)?c2:c0)))
#define func_bcolor(x,c0,c1)		((func_hit(x)?c0:c1))

#define mainfunc_sel(x)		((x==key_seletion))
#define mainfunc_hit(x)		(((key_seletion>0XF)&&(x==((key_seletion>>4)%0X10))))
#define mainfunc_coi(x)		(mainfunc_hit(x)||mainfunc_sel(x))
#define mainfunc_wcolor(x,c0,c1,c2)	((mainfunc_sel(x)?c1:(mainfunc_hit(x)?c2:c0)))
#define mainfunc_bcolor(x,c0,c1)	((mainfunc_hit(x)?c0:c1))

#define	LED_ON()	LED0(1)
#define	LED_OFF()	LED0(0)


typedef struct 
{
    u16 posx;
    u8 posy;
    u16 length;
    u8 height;

    u16 mw;

    u8 trig_way;
    u16 trig32;
    u8 trig_level;
    
    u32 freq;
    u32 tb;
    u32 tdiv;

    u8 res;
    float tvote;
    u8 hsize;

    u16 vote_max;
    u16 vote_min;
    u16 vote_avg;

    u8 adc_ch;

    u8 pwm1_freq;
    u8 pwm1_duty;

    u8 pwm2_freq;
    u8 pwm2_duty;

    u32 pwm1_tb;
    u32 pwm2_tb;
} osc_dev;

void osc_init();
void osc_app();

enum{
	OSC_ADC_START=0X10,
	ADC_CHSEL,
	ADC_TDIV,
	OSC_HDIV,
	OSC_TWAY,
	OSC_TVOTE,
	OSC_MAX,
};

enum{
	OSC_PWM_START=0X20,
	OSC_PWM1_FREQ,
	OSC_PWM1_DUTY,
	OSC_PWM2_FREQ,
	OSC_PWM2_DUTY,
	PWM_MAX,
};

enum{
	OSC_DAC_START=0X30,
	OSC_DAC1_TYPE,
	OSC_DAC1_FREQ,
	OSC_DAC1_DOTS,	
	OSC_DAC1_VPP,
	OSC_DAC1_VCC,
	DAC_MAX,
};

enum{
	OSC_MEMU=1,
	PWM_MEMU,
	DAC_MEMU,
	MAIN_MEMU_MAX,
};
#endif