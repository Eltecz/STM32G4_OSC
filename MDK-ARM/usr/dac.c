/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\dac.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-07 11:04:26
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-22 12:59:14
 */
#include "dac.h"

#ifdef en_dac_cal
u8 dac_cal_flag=1;
#endif

u16 dac_point_lut[9]={10,20,30,40,100,200,300,400,500};
u16 dac_v_lut[11]={0,410,819,1229,1638,2048,2458,2867,3277,3686,4095};

u32 dac1_lut[dac_dma_length]={0};


dac_chnnal_config dac_ch1={0,4,2047,5,2047,5,WAVE_SINE,&dac1_lut};

u16 q12_mod(int32_t din){
    if(din<0)   return 0;
    else if(din>4095)   return 4095;
    else return din;
}
void disable_all_dac_per(){
    TIM6->CR1&=~(u32)(1);
    TIM7->CR1&=~(u32)(1);
    DAC3C1_DMA_OFF();
    DAC3->CR&=~(u32)(1);
}
void dac_init(){

    disable_all_dac_per();
    dac_ch1.vdc=dac_v_lut[dac_ch1.vdc_lut];
    dac_ch1.vpp=dac_v_lut[dac_ch1.vpp_lut];
    RCC->AHB2ENR|=1<<18;    
    DAC3CH1_DMA_INIT();   
    dac_timer_init(dac_ch1.tdiv,1);
    RCC->AHB2ENR|=3;
    GPIO_Set(GPIOA,PIN2,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);
    dac_opamp_init();    
    #ifdef  en_dac_cal
        if(dac_cal_flag){
            /*
            目前不进行校准
            TO-DO 校准
            */
            dac_cal_flag=0;
        }
        DAC3->CR|=1<<14;    
    #endif
    DAC3->CR|=1<<1;
    DAC3->CR|=7<<2;     //DAC3_CH1--TIM6
    DAC3->CR|=1<<12;
    

    DAC3->MCR|=2<<14;
    DAC3->MCR|=3;
    DAC3->MCR|=3<<16;
    if(dac_ch1.wave_type!=WAVE_OFF){
        dac_data_pre(0,dac_point_lut[dac_ch1.dlength]);        
        DAC3->CR|=1<<0;    
        DAC3CH1_DMA_START(dac_point_lut[dac_ch1.dlength]);  
    }

}

void dac_data_pre(u8 ch,u16 dlength){
    u16 i;
    u16 drg_fix,lengthd2;
    u16 a,b,c;
    dac_chnnal_config * p;
    u32 *buf_p;
    p=&dac_ch1;
    buf_p=dac1_lut;
    lengthd2=dlength>>1;
    drg_fix=0x7fff/dlength;
    switch (p->wave_type)
    {
    case WAVE_SINE:
        for(i=0;i<dlength;i++)
            buf_p[i]=q12_mod(p->vdc+(p->vpp*arm_sin_q15(drg_fix*i)>>15));
        break;
    case WAVE_SQR:
        for(i=0;i<(lengthd2);i++)
            buf_p[i]=q12_mod(p->vdc+p->vpp);
        for(i;i<dlength;i++)
            buf_p[i]=q12_mod(p->vdc-p->vpp);
        break;
    case WAVE_TRI:
        a=q12_mod(p->vdc-p->vpp);
        b=q12_mod(p->vpp<<1)/lengthd2;
        c=q12_mod(p->vdc+p->vpp);
        for(i=0;i<(lengthd2);i++)
        {
            buf_p[i]=q12_mod(a+b*i);
            buf_p[i+lengthd2]=q12_mod(c-b*i);            
        }
        break;
    case WAVE_PULSE:
        for(i=0;i<dlength;i++)
            buf_p[i]=p->vdc;
        buf_p[lengthd2]=buf_p[lengthd2+1]=q12_mod(p->vdc+p->vpp);
        break;
    case WAVE_DC:
        for(i=0;i<dlength;i++)
            buf_p[i]=p->vdc;
        break;
    case WAVE_SAW:
        a=q12_mod(p->vdc-p->vpp);
        b=q12_mod(p->vpp<<1)/dlength;
        for(i=0;i<dlength;i++)
            buf_p[i]=q12_mod(a+b*i);
        break;
    default:
        break;
    }
}
