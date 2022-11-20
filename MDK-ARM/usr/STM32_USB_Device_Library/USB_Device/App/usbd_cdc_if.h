/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\STM32_USB_Device_Library\USB_Device\App\usbd_cdc_if.h
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-10 18:57:08
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-22 15:37:37
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "per_conf.h"

#define USB_USART_REC_LEN  200
#define CDC_POLLING_INTERVAL    1       //轮询周期，最大65ms，最小1ms

extern u8  USB_USART_RX_BUF[USB_USART_REC_LEN]; 
extern u16 USB_USART_RX_STA;   					//接收状态标记	
extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

void VCP_DataTx(uint8_t *data,uint32_t Len);
uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len);
void usb_printf(char* fmt,...); 

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_H__ */

