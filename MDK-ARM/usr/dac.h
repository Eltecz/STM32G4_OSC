/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\dac.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-07 11:03:42
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-10 10:15:43
 */
#ifndef __DAC_H
#define __DAC_H	
#include "per_conf.h"
//#define en_dac_cal
 
typedef struct 
{
    u8 tdiv;
    u8 dlength;
    u16 vpp;
    u8 vpp_lut;
    u16 vdc;
    u8 vdc_lut;
    u8 wave_type;
    u32 *dac_buf_p;
}dac_chnnal_config;

enum{
    WAVE_OFF,
    WAVE_SINE,
    WAVE_SQR,
    WAVE_TRI,
    WAVE_PULSE,
    WAVE_SAW,
    WAVE_DC,
};


extern dac_chnnal_config dac_ch1;

extern u16 dac_point_lut[9];
extern u16 dac_v_lut[11];

void dac_init();
void dac_data_pre(u8 ch,u16 dlength);
#endif
