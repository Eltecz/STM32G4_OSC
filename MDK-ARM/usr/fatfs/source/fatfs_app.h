/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\fatfs\source\fatfs_app.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-23 15:14:14
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 00:16:12
 */
#ifndef __FATFS_APP_H__
#define __FATFS_APP_H__

#include "per_conf.h"
#include "ff.h"  
#include "exfuns.h" 
#include "fattester.h"

void fatfs_test();
void usb_disk_init();
void usb_disk_dir_list();
void fatfs_show_bmp(u8 delays,u8 modes);
u8 stdbmp_decode(const u8 *filename); 
#endif