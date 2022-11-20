/*
 * @Description: 
 * @FilePath: \MDK-ARM\usr\fatfs\source\fatfs_app.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-09-23 15:13:55
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-09-24 17:16:39
 */
#include "fatfs_app.h"
#include "bmp.h"
void fatfs_test()
{
  u8 res;
  my_mem_init(SRAM1);
  exfuns_init();
 	res=f_mount(fs[0],"0:",1);	//挂载FLASH.  
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		printf("File System Error, id = %d ,Flash Disk Formatting...\r\n",res);	//格式化FLASH
		res=f_mkfs("0:",FM_ANY,0,fatbuf,FF_MAX_SS);	//格式化FLASH,1:,盘符;FM_ANY,自动选择文件系统类型,0,自动选择簇大小
		if(res==0)
		{  
			f_setlabel((const TCHAR *)"0:ALIENTEK");//设置Flash磁盘的名字为：ALIENTEK
			printf("Flash Disk Format Finish\r\n");	//格式化完成
		}else printf("Flash Disk Format Error ,Error id = %d\r\n",res);	//格式化失败
		delay_ms(1000);
	}	 	
  else{
    printf("SPI FLASH filesystem test done!\r\n");
  }	  
}

void usb_disk_init(){
    my_mem_init(SRAM1);
    exfuns_init();
 	if(f_mount(fs[0],"0:",1)==0x0d){//挂载FLASH.    
		printf("File System Error\r\n");	//格式化FLASH
    }	 
    else{
        printf("Flash Mount.\r\n");
    }
}

void usb_disk_dir_list(){
    mf_scan_files("0:/IMAGE");
}

void fatfs_show_bmp(u8 delays,u8 modes){
	DIR picdir;
    char dirs[]="0:/IMAGE/00.BMP";    
    u8 image_cnt=0;
    u8 i=0;
	u8 ret,k;
    mf_scan_cnt_files("0:/IMAGE",&image_cnt);
    printf("find %d images\r\n",image_cnt);	
	exfunc.image_cnt=image_cnt;
    f_opendir(&picdir,"0:/");
	while(1){
		if(modes==0){
			while(1){
					sprintf(dirs,"0:/IMAGE/%02d.BMP",i);
					ret=stdbmp_decode(dirs);
					if(ret)
					{
						printf("image %s open error\r\n",dirs);
					}       
					else
					{
					printf("open image %s successed\r\n",dirs);                
					}  
					ret=0;
					while(ret==0){
						ret=getkey();
					}
					switch (ret)
					{
					case KL:i++;i%=image_cnt;break;
					case KR:i=(i)?(i-1):(image_cnt-1);break;
					case KM:goto K;		
					}
					delay_ms(100); 
			}		
		}
		else if(modes==1){
			while(1){
					sprintf(dirs,"0:/IMAGE/%02d.BMP",i);
					ret=stdbmp_decode(dirs);
					if(ret)
					{
						printf("image %s open error\r\n",dirs);
					}       
					else
					{
					printf("open image %s successed\r\n",dirs);                
					}  
					i++;
					k=0;
					i%=image_cnt; 
					for(k=0;k<delays;k++){
						if(getkey())	goto K; 
						delay_ms(100); 							
					}
				
			}			
		}		
	}
K:  return;


}

#define BMP_DBUF_SIZE 1024
FIL f_bfile;
u8 bmpreadbuf[BMP_DBUF_SIZE];
u8 stdbmp_decode(const u8 *filename) 
{
	FIL* f_bmp;
    u16 br=0;	   		   
	u8 res;
 	u32 readlen=BMP_DBUF_SIZE;//一次从SD卡读取的字节数长度
	f_bmp=&f_bfile;
	res=f_open(f_bmp,(const TCHAR*)filename,FA_READ);//打开文件	 						  
	if(res==0)//打开成功.
	{ 
        f_read(f_bmp,bmpreadbuf,sizeof(BITMAPINFO),(UINT*)&br);	//读出readlen个字节  
        LCD_Address_Set(0,0,321,171);        
		while(1)
		{	
			res=f_read(f_bmp,bmpreadbuf,readlen,(UINT *)&br);//读出readlen个字节            
			LCD_Fillcolor_pal(BMP_DBUF_SIZE,bmpreadbuf);
			if(br!=(readlen))readlen=br;	//最后一批数据		
			if(res||br==0)break;		//读取出错
		} 

		f_close(f_bmp);//关闭文件
    printf("image read done \r\n");        
	}  	
	else
	{
		printf("read error,code %d",res);
	}	
	
	return res;		//BMP显示结束.    					   
}	