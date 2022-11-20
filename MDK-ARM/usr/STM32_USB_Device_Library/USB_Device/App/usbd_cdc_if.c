/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           USB_Device/CDC_Standalone/USB_Device/App/usbd_cdc_if.c
  * @author         MCD Application Team
  * @brief          This file implements the USB device descriptors.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"
#include "stdarg.h"
#include "stdio.h"

uint8_t UserRxBufferFS[USB_USART_REC_LEN];


uint8_t UserTxBufferFS[USB_USART_REC_LEN];

/* USER CODE BEGIN PRIVATE_VARIABLES */
USBD_CDC_LineCodingTypeDef LineCoding =
{
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};
uint32_t BuffLength;
uint32_t UserTxBufPtrIn;/* Increment this pointer or roll it back to
                               start address when data are received over USART */
uint32_t UserTxBufPtrOut; /* Increment this pointer or roll it back to
                                 start address when data are sent over USB */

__IO uint32_t uwPrescalerValue;

extern USBD_HandleTypeDef USBD_Device;
static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t * pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t * pbuf, uint32_t * Len);
static int8_t CDC_Itf_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum);
//���⴮�����ú���(��USB�ں˵���)
USBD_CDC_ItfTypeDef USBD_CDC_fops= 
{
    CDC_Itf_Init,
    CDC_Itf_DeInit,
    CDC_Itf_Control,
    CDC_Itf_Receive,
    CDC_Itf_TransmitCplt
};

//��ʼ��VCP
//����ֵ:USBD_OK
static int8_t CDC_Itf_Init(void)
{
    USBD_CDC_SetRxBuffer(&USBD_Device,UserRxBufferFS);
    return (USBD_OK);
}

//��λVCP
//����ֵ:USBD_OK
static int8_t CDC_Itf_DeInit(void)
{
    return (USBD_OK);
}

//����VCP������
//buf:�������ݻ�����/�������滺����
//len:���ݳ���
//����ֵ:USBD_OK
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t * pbuf, uint16_t length)
{
    switch (cmd)
    {
        case CDC_SEND_ENCAPSULATED_COMMAND:break;
        case CDC_GET_ENCAPSULATED_RESPONSE:break;
        case CDC_SET_COMM_FEATURE:break;
        case CDC_GET_COMM_FEATURE:break;
        case CDC_CLEAR_COMM_FEATURE:break;
        case CDC_SET_LINE_CODING:
            LineCoding.bitrate = (uint32_t) (pbuf[0] | (pbuf[1] << 8) |
                                     (pbuf[2] << 16) | (pbuf[3] << 24));
            LineCoding.format = pbuf[4];
            LineCoding.paritytype = pbuf[5];
            LineCoding.datatype = pbuf[6];
			//��ӡ���ò���
			printf("linecoding.format:%d\r\n",LineCoding.format);
			printf("linecoding.paritytype:%d\r\n",LineCoding.paritytype);
			printf("linecoding.datatype:%d\r\n",LineCoding.datatype);
			printf("linecoding.bitrate:%d\r\n",LineCoding.bitrate);
            break;

        case CDC_GET_LINE_CODING:
            pbuf[0] = (uint8_t) (LineCoding.bitrate);
            pbuf[1] = (uint8_t) (LineCoding.bitrate >> 8);
            pbuf[2] = (uint8_t) (LineCoding.bitrate >> 16);
            pbuf[3] = (uint8_t) (LineCoding.bitrate >> 24);
            pbuf[4] = LineCoding.format;
            pbuf[5] = LineCoding.paritytype;
            pbuf[6] = LineCoding.datatype;
            break;

        case CDC_SET_CONTROL_LINE_STATE:break;
        case CDC_SEND_BREAK:break;
        default:break;
    }

    return (USBD_OK);
}

//CDC���ݽ��պ���
//Buf:�������ݻ�����
//Len:���յ������ݳ���
static int8_t CDC_Itf_Receive(uint8_t *Buf,uint32_t *Len)
{
    USBD_CDC_ReceivePacket(&USBD_Device);
    VCP_DataRx(Buf,*Len);
    return (USBD_OK);
}

static int8_t CDC_Itf_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  return result;
}

//�����USB���⴮�ڽ��յ�������
//databuffer:���ݻ�����
//Nb_bytes:���յ����ֽ���.
//����ֵ:USBD_OK
uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	u8 i;
	u8 res;
	for(i=0;i<Len;i++)
	{  
		res=Buf[i]; 
	}  
	return USBD_OK;
}

//ͨ��USB��������
//data:Ҫ���͵�����
//length:���ݳ���
void VCP_DataTx(uint8_t *data,uint32_t Len)
{
    USBD_CDC_SetTxBuffer(&USBD_Device,data,Len);
    USBD_CDC_TransmitPacket(&USBD_Device);
    delay_ms(CDC_POLLING_INTERVAL);
}

//usb���⴮��,printf ����
//ȷ��һ�η������ݲ���USB_USART_REC_LEN�ֽ�
void usb_printf(char* fmt,...)  
{  
	u16 i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)UserTxBufferFS,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UserTxBufferFS);     //�˴η������ݵĳ���
    VCP_DataTx(UserTxBufferFS,i);              //��������
} 







