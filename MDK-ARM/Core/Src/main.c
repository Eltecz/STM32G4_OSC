/*
 * @Description: 
 * @FilePath: \MDK-ARM\Core\Src\main.c
 * @Version: 2.0
 * @Autor: Eltecz
 * @Date: 2022-08-22 11:40:42
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 09:28:32
 */
/*

*/
#include "main.h"
#include "per_conf.h"
#include "pic1.h"
u8 i=0;
char j=0;
char s[20];
u32 k=0;
u32 l;


int main(void)
{
  Stm32_Clock_Init(MAIN_FOSC/2,2,2,2);
  delay_init(MAIN_FOSC);
  gpio_init();
  LCD_Init();
//  uart1_init(MAIN_FOSC,921600);
//  printf("done!\r\n");
  osc_init();  
  LCD_Clear(WHITE);	
  LCD_ShowString(30,30,"STM32G431_OSC",BLACK,WHITE,1);
  LCD_ShowString(30,42,"Eltecz",BLACK,WHITE,1);  
  LCD_ShowString(30,54,"Non-Commercial",RED,WHITE,1);  
  delay_ms(1000);
  LCD_Clear(BLACK);
  dac_init();
  while (1)
  {
    osc_app();
  }
    while (0)
    {
      for(k=0;i<320*172;k++){
        LCD_DrawPoint(k%320,k/320,(gImage_pic1[k<<1])+(gImage_pic1[(k<<1)+1]<<8));
      }
    }	
}
