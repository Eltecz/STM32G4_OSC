/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\sdmmc\usr_sd_app.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-20 16:24:52
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-20 17:13:09
 */
#include "usr_sd_app.h"

void sd_init(){
    BSP_SD_CardInfo cards;
    u8 sd_status;
	sd_status=BSP_SD_Init();
    if(sd_status){
        printf("SD card Error");
        return ;
    }   
    printf("SD card init done\r\n");
    BSP_SD_GetCardInfo(&cards);
  	printf("Card ManufacturerID:%d\r\n",cards.Cid.ManufacturerID);	//制造商ID
	printf("Card Capacity:%d MB\r\n",(u32)(cards.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",cards.CardBlockSize);			//显示块大小    
    
}