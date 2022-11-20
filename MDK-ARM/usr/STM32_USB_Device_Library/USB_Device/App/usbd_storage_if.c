/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           USB_Device/MSC_Standalone/USB_Device/App/usbd_storage_if.c
  * @author         MCD Application Team
  * @brief          Memory management layer
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */


/* Private function prototypes ----------------------------------------------- */
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t * block_num,uint16_t * block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t * buf, uint32_t blk_addr,uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t * buf, uint32_t blk_addr,uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);

USBD_StorageTypeDef USBD_DISK_fops = {
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata_FS,
};

//初始化存储设备
//lun:逻辑单元编号,0,SD卡;1,SPI FLASH
//返回值:0,成功;
//    其他,错误代码
int8_t STORAGE_Init (uint8_t lun)
{
	W25QXX_Init();
	return 0; 
} 

//获取存储设备的容量和块大小
//lun:逻辑单元编号,0,SD卡;1,SPI FLASH
//block_num:块数量(扇区数)
//block_size:块大小(扇区大小)
//返回值:0,成功;
//    其他,错误代码
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{     
	*block_size=512;  
	*block_num=1024*1024*9/512;	//SPI FLASH的前面25M字节,文件系统用
} 

//查看存储设备是否就绪
//lun:逻辑单元编号,0,SD卡;1,SPI FLASH
//返回值:0,就绪;
//    其他,未就绪
int8_t  STORAGE_IsReady (uint8_t lun)
{ 
	return 0;
} 

//查看存储设备是否写保护
//lun:逻辑单元编号,0,SD卡;1,SPI FLASH
//返回值:0,没有写保护;
//    其他,写保护(只读)
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
	return  0;
} 

//从存储设备读取数据
//lun:逻辑单元编号,0,SD卡;1,SPI FLASH
//buf:数据存储区首地址指针
//blk_addr:要读取的地址(扇区地址)
//blk_len:要读取的块数(扇区数) 
//返回值:0,成功;
//    其他,错误代码 
int8_t STORAGE_Read (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len)
{
	W25QXX_Read(buf,blk_addr*512,blk_len*512);
	return 0;
}

//向存储设备写数据
//lun:逻辑单元编号,0,SD卡;1,SPI FLASH
//buf:数据存储区首地址指针
//blk_addr:要写入的地址(扇区地址)
//blk_len:要写入的块数(扇区数) 
//返回值:0,成功;
//    其他,错误代码 
int8_t STORAGE_Write (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len) 
{
	W25QXX_Write(buf,blk_addr*512,blk_len*512);
	return 0; 
}

//获取支持的最大逻辑单元个数
//返回值:支持的逻辑单元个数-1
int8_t STORAGE_GetMaxLun (void)
{
	return STORAGE_LUN_NBR-1;
}
