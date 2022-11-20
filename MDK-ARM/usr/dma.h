/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\dma.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-05-22 11:28:12
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 10:09:37
 */
#ifndef __dma_h__
#define __dma_h__

#include "per_conf.h"

extern vu16 fill_color_buf;

void SPI1_LCD_DMA_INIT();
void SPI1_DMA_START(u32 ndtr,u32 buftr,u8 mode);

void ADC1_DMA_CHNNAL_INIT();
void ADC1_DMA_START(u32 ndtr);

void ADC_DMA_CHNNAL_INIT();
void Dual_ADC_DMA_START(u32 ndtr);
void Dual_ADC_DMA_END();

void DAC3CH1_DMA_INIT();
void DAC3CH1_DMA_START(u32 ndtr);

void DAC3C1_DMA_OFF();

void SPI3RX_DMA_INIT();
void SPI3RX_DMA_START(u32 ndtr,u32 buftr);
void SPI3RX_DMA_END();
#endif