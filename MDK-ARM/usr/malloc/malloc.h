/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\malloc\malloc.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-20 09:32:16
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-24 11:47:51
 */
#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//内存管理 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2018/7/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved  
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	 

 
#ifndef NULL
#define NULL 0
#endif

//定义六个内存池
#define SRAM1	 	0		//AXI内存池,AXI共512KB 
#define SRAM2   	1		//外部内存池(SDRAM),SDRAM共32MB
#define CCMSRAM		2		//SRAM1/2内存池,SRAM1+SRAM2,共256KB


#define SRAMBANK 	3		//定义支持的SRAM块数.	


#define MEM1_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM1_MAX_SIZE			4*1024  						//最大管理内存 6K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

#define MEM2_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM2_MAX_SIZE			0 *1024  					//最大管理内存 2K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小

#define MEM3_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM3_MAX_SIZE			0 *1024  						//最大管理内存 4K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小

//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(u8);					//初始化
	u16 (*perused)(u8);		  	    	//内存使用率
	u8 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	u32 *memmap[SRAMBANK]; 				//内存管理状态表
	u8  memrdy[SRAMBANK]; 				//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;	 //在mallco.c里面定义

void mymemset(void *s,u8 c,u32 count);	//设置内存
void mymemcpy(void *des,void *src,u32 n);//复制内存     
void my_mem_init(u8 memx);				//内存管理初始化函数(外/内部调用)
u32 my_mem_malloc(u8 memx,u32 size);	//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);		//内存释放(内部调用)
u16 my_mem_perused(u8 memx) ;			//获得内存使用率(外/内部调用) 
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(u8 memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(u8 memx,u32 size);			//内存分配(外部调用)
void *myrealloc(u8 memx,void *ptr,u32 size);//重新分配内存(外部调用)
#endif













