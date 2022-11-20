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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//FATFS�ײ�(ffsystem) ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 



//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}

//��̬�����ڴ�
void *ff_memalloc (UINT size)			
{
	return (void*)mymalloc(SRAM1,size);
}

//�ͷ��ڴ�
void ff_memfree (void* mf)		 
{
	myfree(SRAM1,mf);
}











