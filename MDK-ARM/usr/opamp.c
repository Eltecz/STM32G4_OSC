/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\opamp.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-07 11:10:59
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-09 23:06:34
 */
#include "per_conf.h"
//#define enable_user_amp_cal
void dac_opamp_init()
{
    //校准
    OPAMP1->CSR|=1<<7;    
#ifdef enable_user_amp_cal
    OPAMP1->CSR|=1<<0;
    OPAMP1->CSR|=1<<4;
    OPAMP1->CSR|=1<<11;
    u8 j;
    for(u8 i=0;i<2;i++){
        OPAMP1->CSR|=1<<12;
        for(j=0;j<16;j++){
            OPAMP1->CSR|=j<<24;
            delay_us(100);
            if(!((OPAMP1->CSR)&&(1<<30)))   break;
        }
        delay_ms(10);
        OPAMP1->CSR|=3<<12;
        for(j=0;j<16;j++){
            OPAMP1->CSR|=j<<19;
            delay_us(100);
            if(!((OPAMP1->CSR)&&(1<<30)))   break;
        }        
    }
    //校准结束
    //跟随模式，作为DAC1的输出缓冲
    OPAMP1->CSR-=1;
    OPAMP1->CSR-=1<<11;
#endif
    OPAMP1->CSR|=3<<5;
    OPAMP1->CSR|=3<<2;
    OPAMP1->TCMR|=3<<1;
    OPAMP1->TCMR|=1;
    OPAMP1->CSR|=1;


    OPAMP3->CSR|=1<<7; 
#ifdef enable_user_amp_cal
    OPAMP3->CSR|=1<<0;
    OPAMP3->CSR|=1<<4;
    OPAMP3->CSR|=1<<11;
    u8 j;
    for(u8 i=0;i<2;i++){
        OPAMP3->CSR|=1<<12;
        for(j=0;j<16;j++){
            OPAMP3->CSR|=j<<24;
            delay_us(100);
            if(!((OPAMP3->CSR)&&(1<<30)))   break;
        }
        delay_ms(10);
        OPAMP3->CSR|=3<<12;
        for(j=0;j<16;j++){
            OPAMP3->CSR|=j<<19;
            delay_us(100);
            if(!((OPAMP3->CSR)&&(1<<30)))   break;
        }        
    }
    //校准结束
    //跟随模式，作为DAC1的输出缓冲
    OPAMP3->CSR-=1;
    OPAMP3->CSR-=1<<11;
#endif
    OPAMP3->CSR|=3<<5;
    OPAMP3->CSR|=3<<2;
    OPAMP3->TCMR|=3<<1;
    OPAMP3->TCMR|=1;
    OPAMP3->CSR|=1;
}