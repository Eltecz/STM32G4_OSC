/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\fatfs\source\ffsystem.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-21 17:28:25
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-21 17:39:15
 */
/*------------------------------------------------------------------------*/
/* Sample Code of OS Dependent Functions for FatFs                        */
/* (C)ChaN, 2018                                                          */
/*------------------------------------------------------------------------*/
#include "ff.h"
#include "sys.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//FATFS底层(ffsystem) 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 



//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}

//动态分配内存
void *ff_memalloc (UINT size)			
{
	return (void*)mymalloc(SRAM1,size);
}

//释放内存
void ff_memfree (void* mf)		 
{
	myfree(SRAM1,mf);
}











