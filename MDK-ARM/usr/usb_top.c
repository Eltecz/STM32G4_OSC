/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\usb_top.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-11 10:57:24
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-24 17:10:03
 */
#include "usb_top.h"

USBD_HandleTypeDef USBD_Device;

u8 usb_connent_cdc(){
	USBD_Init(&USBD_Device,&CDC_Desc,0);
	USBD_RegisterClass(&USBD_Device,USBD_CDC_CLASS);
	USBD_CDC_RegisterInterface(&USBD_Device,&USBD_CDC_fops);
	USBD_Start(&USBD_Device);
    return bDeviceState;
}
u8 usb_Msc_connent(){
	USBD_Init(&USBD_Device,&MSC_Desc,0);
	USBD_RegisterClass(&USBD_Device,USBD_MSC_CLASS);
	USBD_MSC_RegisterStorage(&USBD_Device,&USBD_DISK_fops);
	USBD_Start(&USBD_Device);
    return bDeviceState;
}
u8 usb_connent(){
	USBD_Init(&USBD_Device,&Mult_Desc,0);
	USBD_RegisterClass(&USBD_Device,USBD_Mult_CLASS);	
//	USBD_MSC_RegisterStorage(&USBD_Device,&USBD_DISK_fops);
	USBD_Start(&USBD_Device);
    return bDeviceState;
}
void usb_disconnent(){
    USBD_Stop(&USBD_Device);
}