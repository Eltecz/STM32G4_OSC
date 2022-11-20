/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\spi.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-07-18 16:08:29
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-26 23:09:35
 */
#ifndef __SPI_H
#define __SPI_H
#include "per_conf.h"

// SPI总线速度设置 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7

void SPI1_Init(void);
void SPI1_SetSpeed(u8 SpeedSet);
u8 SPI1_ReadWriteByte(u8 TxData);
void SPI1_WriteByte(u8 TxData);

void SPI3_Init(void);
void SPI3_SetSpeed(u8 SpeedSet);
u8 SPI3_ReadWriteByte(u8 TxData);
void SPI3_WriteByte(u8 TxData);
u8 SPI3_ReadByte(u8 TxData);
#endif

