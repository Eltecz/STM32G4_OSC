/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\usb_top.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-11 10:55:30
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-22 16:37:26
 */
#ifndef __USB_TOP_H
#define __USB_TOP_H

#include "per_conf.h"


typedef struct _USBD_CDC_Itf
{
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length);
  int8_t (* Receive)(uint8_t *Buf, uint32_t *Len);
  int8_t (* TransmitCplt)(uint8_t *Buf, uint32_t *Len, uint8_t epnum);
} USBD_CDC_ItfTypeDef;

typedef struct _USBD_STORAGE
{
  int8_t (* Init)(uint8_t lun);
  int8_t (* GetCapacity)(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
  int8_t (* IsReady)(uint8_t lun);
  int8_t (* IsWriteProtected)(uint8_t lun);
  int8_t (* Read)(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
  int8_t (* Write)(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
  int8_t (* GetMaxLun)(void);
  int8_t *pInquiry;

} USBD_StorageTypeDef;

#include "usbd_cdc_if.h"
#include "usbd_storage_if.h"

#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"

#include "usbd_cdc.h"

#include "usbd_msc_bot.h"
#include "usbd_msc_data.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc.h"

#include "usbd_mult.h"

#include "usbd_conf.h"

extern USBD_HandleTypeDef USBD_Device;

u8 usb_connent();
u8 usb_Msc_connent();
void usb_disconnent();

#endif