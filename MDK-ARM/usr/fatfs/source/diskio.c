/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "diskio.h"			/* FatFs lower layer API */
#include "w25qxx.h"
#include "malloc.h"	 

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//FATFS�ײ�(diskio) ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


#define EX_FLASH 	0			//�ⲿspi flash,���Ϊ1
//����W25Q256
//ǰ25M�ֽڸ�fatfs��,25M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��6.01M.	ʣ�ಿ��,���ͻ��Լ���	 
#define SPI_FLASH_SECTOR_SIZE 	512	
#define SPI_FLASH_SECTOR_COUNT 	1024*16*2	//W25Q256,ǰ25M�ֽڸ�FATFSռ��	
#define SPI_FLASH_BLOCK_SIZE   	8     		//ÿ��BLOCK��8������		
  
  
  
//��ô���״̬
DSTATUS disk_status (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{ 
	return RES_OK;
}  


//��ʼ������
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	u8 res=0;	    
	W25QXX_Init();  //W25QXX��ʼ��
	if(res)return  STA_NOINIT;
	else return 0; //��ʼ���ɹ� 
} 
//������
//pdrv:���̱��0~9
//*buff:���ݽ��ջ����׵�ַ
//sector:������ַ
//count:��Ҫ��ȡ��������
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������	
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*SPI_FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE);
				sector++;
				buff+=SPI_FLASH_SECTOR_SIZE;
			}
   //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    return RES_OK;	 
}
//д����
//pdrv:���̱��0~9
//*buff:���������׵�ַ
//sector:������ַ
//count:��Ҫд��������� 
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	W25QXX_Write((u8*)buff,sector*SPI_FLASH_SECTOR_SIZE,SPI_FLASH_SECTOR_SIZE);
	sector++;
	buff+=SPI_FLASH_SECTOR_SIZE;
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    return RES_OK;	 
} 
//����������Ļ��
//pdrv:���̱��0~9
//ctrl:���ƴ���
//*buff:����/���ջ�����ָ�� 
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
DRESULT res;
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = SPI_FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = SPI_FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SPI_FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
		}
    return res;
} 




















