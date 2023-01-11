/*
 * @Description:
 * @FilePath: \MDK-ARM\usr\osc.c
 * @Version: 3.0
 * @Autor: Eltecz
 * @Date: 2022-06-21 15:42:04
 * @LastEditors: Eltecz
 * @LastEditTime: 2022-10-27 10:58:25
 */
#include "osc.h"



extern u16 adc_delay[][2];
extern u16 pwm_lut[][2];

u16 waveform_db[2][adc_length];
u16 dualadc_buf[adc_dma_length];
#define sizewdb sizeof(waveform_db[0])
static u16 *wdb1_p = waveform_db[0];
static u16 *wdb2_p = waveform_db[1];
u16 runtime;
u8 un_trig=0;

__ALIGNED(4) static osc_dev oscdev = {
	8, 12, adc_length, 128,
	0, 
	1, (1 << 11), 5, 
	0,0,3,
	12,3.3,0,
	0,0,0,
	0,
	6,7,6,7,
	0,0
	};

static u16 key_seletion;
static u16 old_seletion;


char wave_sin_s[]=	"Sin";
char wave_dc_s[]=	"DC ";
char wave_off_s[]=	"Off";
char wave_tri_s[]=	"Tri";
char wave_sqr_s[]=	"Squ";
char wave_pul_s[]=	"Pul";
char wave_saw_s[]=	"Saw";


u32 d_abs(u32 a,u32 b,u32 c){
	if(c<a)return a;
	else if(c>b) return b;
	else return c;
}

void LCD_DrawLine_fast(u16 x1, u8 y1, u8 y2, u32 color)
{
	u16 t;
	if (y1 < y2)
	{
		LCD_Address_Set(x1,y1,x1,y2);
		for (t = y1; t < y2 + 1; t++)
			LCD_WR_DATA(color);
	}
	else
	{
		LCD_Address_Set(x1,y2,x1,y1);		
		for (t = y2; t < y1 + 1; t++)
			LCD_WR_DATA(color);
	}
}
void Gui_linemap(u16 *db, u16 x, u8 y, u8 h, u16 l, u16 color)
{
	u16 i;
	for (i = 0; i < l - 1; i++)
	{
		LCD_DrawLine_fast(i + x, *(db + i), *(db + i + 1), color);
	}
}
void Gui_duallinemap(u16 *db, u16 *db_old, u16 x, u8 y, u8 h, u16 l, u16 color)
{
	u16 i;
	u8 t;
	u8 p;
	u8 trig_p=d_abs(0,h,h-(oscdev.trig32*h>>oscdev.res));
	u16 *resize_p;
	u16 *resize_pold;
	if(oscdev.tdiv<3){
		switch (oscdev.tdiv)
		{
		case 0:t=l/10,p=10,resize_p=&db[adc_length_d2*9/10],resize_pold=&db_old[adc_length_d2*9/10];break;
		case 1:t=l/5,p=5,resize_p=&db[adc_length_d2*4/5],resize_pold=&db_old[adc_length_d2*4/5];break;
		case 2:t=l/2,p=2,resize_p=&db[adc_length_d2/2],resize_pold=&db_old[adc_length_d2/2];break;
		}
		for (i = 0; i < t; i++)
		{
				LCD_DrawLine(i*p + x, *(resize_pold + i), i*p + x+p, *(resize_pold + i + 1), BLACK);
				LCD_DrawLine(i*p + x, *(resize_p + i), i*p + x+p, *(resize_p + i + 1), color);
		}		
	}
	else
	{
		for (i = 1; i < l - 1; i++)
		{
			LCD_DrawLine_fast(i + x, *(db_old + i), *(db_old + i + 1), BLACK);
			LCD_DrawLine_fast(i + x, *(db + i), *(db + i + 1), color);
		}		
	}

	if(!oscdev.hsize)LCD_DrawLine(x,y+trig_p,x+l,y+trig_p,BROWN);
}

void wave_gen(u16 *adcdb,u16 *bufdb, u8 y, u16 h, u16 l, u16 mw)
{
	u16 i;
	u16 *adcbuf_p = adcdb + mw - adc_length_d2 + 1;
	u32 vote_sum=0;
	u16 resize_v=(oscdev.hsize==0)?1:((oscdev.hsize==1)?2:(oscdev.hsize==2)?5:10);
	for (i = 0; i < l; i++)
	{
		bufdb[i] = (u16)(h-2-d_abs(0,h,(((*(adcbuf_p + i)) * h *resize_v >> 12)))+ y);
		vote_sum+=(*(adcbuf_p+i));
		if(*(adcbuf_p+i)>oscdev.vote_max)	oscdev.vote_max=*(adcbuf_p+i);
		if(*(adcbuf_p+i)<oscdev.vote_min)	oscdev.vote_min=*(adcbuf_p+i);		
	}
	oscdev.vote_avg=vote_sum/l;
}

void osc_init()
{
	Adc_DMA_Init(oscdev.tdiv,oscdev.adc_ch);
	PWM_TIM_INIT(oscdev.pwm1_freq,oscdev.pwm2_freq,oscdev.pwm1_duty,oscdev.pwm2_duty);
	runtime = 0;
	key_seletion=ADC_CHSEL;
	old_seletion=ADC_CHSEL;
	memset(wdb1_p, 0x00, sizewdb);
	memset(wdb2_p, 0x00, sizewdb);
}

void osc_reload()
{
	oscdev.mw = 0;
	oscdev.freq = 0;
	un_trig=0;
	oscdev.trig32=(oscdev.trig_level*(1<<oscdev.res))/10;
	oscdev.vote_max=oscdev.vote_avg=0;
	oscdev.vote_min=1<<oscdev.res;
	oscdev.tb=adc_delay[oscdev.tdiv][0]*adc_delay[oscdev.tdiv][1]*1000/MAIN_FOSC;
	if(mainfunc_coi(PWM_MEMU)){
		if(oscdev.pwm1_freq)oscdev.pwm1_tb=MAIN_FOSC*1000000/(pwm_lut[oscdev.pwm1_freq][0]*pwm_lut[oscdev.pwm1_freq][1]);
		if(oscdev.pwm2_freq)oscdev.pwm2_tb=MAIN_FOSC*1000000/(pwm_lut[oscdev.pwm2_freq][0]*pwm_lut[oscdev.pwm2_freq][1]);		
	}

	old_seletion=key_seletion;
}

void p_exchange()
{
	u16 *p;
	p=wdb1_p;
	wdb1_p=wdb2_p;
	wdb2_p=p;
}

void osc_showfreq(u16 x,u8 y){
	char s[10];
	u32 freq=oscdev.freq;
	if(freq>10000)	sprintf(s,"Freq=%3.1lfKhz",(float)freq/1000.0);
	else					sprintf(s,"Freq=%5d hz",freq);
	LCD_ShowString(x,y,s,WHITE,BLACK,0);	
}

void osc_showtdiv(u16 x,u8 y){
	char s[10];
	u32 tb=oscdev.tb;
	switch (oscdev.tdiv)
	{
		case 0:tb/=10;break;
		case 1:tb/=5;break;
		case 2:tb/=2;break;
	}
	sprintf(s,"AD%d:",oscdev.adc_ch+1);
	LCD_ShowString(x,y,s,func_wcolor(ADC_CHSEL,WHITE,OSC_COLOR,BLACK),\
	OSC_COLOR,!func_hit(ADC_CHSEL));	
	if(tb<999)	sprintf(s,"%4dns",tb);
	else				sprintf(s,"%4dus",tb/1000);
	LCD_ShowString(x+24,y,s,func_wcolor(ADC_TDIV,WHITE,OSC_COLOR,BLACK),\
	OSC_COLOR,!func_hit(ADC_TDIV));	
}

void osc_showtrig(u16 x,u8 y){
	u8 p='~'+1+(oscdev.trig_way<<1);
	u16 color_f,color_b;
	char s[20];
	LED_ON();
	if(func_sel(OSC_TWAY))	color_f=OSC_COLOR;
	else	if((runtime%2)&&(oscdev.trig_way)&&(un_trig)){
		color_f=BLACK;
		LED_OFF();
	}
	else	color_f=WHITE;
	LCD_ShowChar(x,y,p,color_f,OSC_COLOR,!func_hit(OSC_TWAY));	
	LCD_ShowChar(x+6,y,p+1,color_f,OSC_COLOR,!func_hit(OSC_TWAY));	
	sprintf(s,"Trig=%1.3lfV",oscdev.tvote*oscdev.trig_level/10);
	LCD_ShowString(x+24,y,s,func_wcolor(OSC_TVOTE,WHITE,OSC_COLOR,BLACK),\
	OSC_COLOR,!func_hit(OSC_TVOTE));
}

void osc_showpwm(u16 x,u8 y){
	char s[10];	
	LCD_ShowString(x,y,"PWM1:      |",WHITE,BLACK,0);	
	if(oscdev.pwm1_freq){
		if(oscdev.pwm1_tb>999999)	sprintf(s,"%3dMhz",oscdev.pwm1_tb/1000000);
		else if(oscdev.pwm1_tb>999)	sprintf(s,"%3dKhz",oscdev.pwm1_tb/1000);
		else	sprintf(s,"%3d hz",oscdev.pwm1_tb);
	}
	else	sprintf(s,"OFF   ");
	LCD_ShowString(x+30,y,s,func_wcolor(OSC_PWM1_FREQ,WHITE,PWM_COLOR,BLACK),\
	PWM_COLOR,!func_hit(OSC_PWM1_FREQ));
	if(oscdev.pwm1_freq){
		sprintf(s,"%3d%%",oscdev.pwm1_duty*10);
		LCD_ShowString(x+72,y,s,func_wcolor(OSC_PWM1_DUTY,WHITE,PWM_COLOR,BLACK),\
	PWM_COLOR,!func_hit(OSC_PWM1_DUTY));	
	}
	

	LCD_ShowString(x+102,y,"PWM2:      |",WHITE,BLACK,0);	
	if(oscdev.pwm2_freq){
		if(oscdev.pwm2_tb>999999)	sprintf(s,"%3dMhz",oscdev.pwm2_tb/1000000);
		else if(oscdev.pwm2_tb>999)	sprintf(s,"%3dKhz",oscdev.pwm2_tb/1000);
		else	sprintf(s,"%3d hz",oscdev.pwm2_tb);
	}
	else	sprintf(s,"OFF   ");
	LCD_ShowString(x+132,y,s,func_wcolor(OSC_PWM2_FREQ,WHITE,PWM_COLOR,BLACK),\
	PWM_COLOR,!func_hit(OSC_PWM2_FREQ));
	sprintf(s,"40%%");
	if(oscdev.pwm2_freq){
		sprintf(s,"%3d%%",oscdev.pwm2_duty*10);
		LCD_ShowString(x+174,y,s,func_wcolor(OSC_PWM2_DUTY,WHITE,PWM_COLOR,BLACK),\
	PWM_COLOR,!func_hit(OSC_PWM2_DUTY));	
	}		
}

void draw_memu(){
	u16 xs=200;
	LCD_ShowString(xs,0,"OSC",mainfunc_wcolor(OSC_MEMU,WHITE,OSC_COLOR,BLACK),\
	OSC_COLOR,!mainfunc_hit(OSC_MEMU));
	xs+=24;
	LCD_ShowString(xs,0,"PWM",mainfunc_wcolor(PWM_MEMU,WHITE,PWM_COLOR,BLACK),\
	PWM_COLOR,!mainfunc_hit(PWM_MEMU));
	xs+=24;
	LCD_ShowString(xs,0,"DAC",mainfunc_wcolor(DAC_MEMU,WHITE,DAC_COLOR,BLACK),\
	DAC_COLOR,!mainfunc_hit(DAC_MEMU));
}

void draw_dac_ui(u16 x,u8 y)
{
	char *ws;
	char s[10];
	u8 pl=y+12;
	u16 px=x;
	u32 freq;
	u32 pp;
	LCD_ShowString(px,y,"DAC1:",WHITE,BLACK,0);
	px+=30;
	switch (dac_ch1.wave_type)
	{
	case WAVE_SINE:		ws=wave_sin_s;break;
	case WAVE_PULSE:	ws=wave_pul_s;break;	
	case WAVE_DC:		ws=wave_dc_s;break;
	case WAVE_TRI:		ws=wave_tri_s;break;
	case WAVE_SQR:		ws=wave_sqr_s;break;
	case WAVE_SAW:		ws=wave_saw_s;break;
	default:	ws=wave_off_s;break;
	}
	LCD_ShowString(px,y,ws,func_wcolor(OSC_DAC1_TYPE,WHITE,DAC_COLOR,BLACK),\
	DAC_COLOR,!func_hit(OSC_DAC1_TYPE));
	px+=30;
	freq=MAIN_FOSC*1000000/(dac_trig_lut[dac_ch1.tdiv][0]*dac_trig_lut[dac_ch1.tdiv][1]);
	if(freq>999999)	sprintf(s,"%3dMhz",freq/1000000);
	else if(freq>999)	sprintf(s,"%3dKhz",freq/1000);
	else	sprintf(s,"%3d hz",freq);
	LCD_ShowString(px,y,s,func_wcolor(OSC_DAC1_FREQ,WHITE,DAC_COLOR,BLACK),\
	DAC_COLOR,!func_hit(OSC_DAC1_FREQ));
	px+=36;
	sprintf(s,"%4dpx",dac_point_lut[dac_ch1.dlength]);	
	LCD_ShowString(px,y,s,func_wcolor(OSC_DAC1_DOTS,WHITE,DAC_COLOR,BLACK),\
	DAC_COLOR,!func_hit(OSC_DAC1_DOTS));	
	px+=48;
	LCD_ShowString(px,y,"Vpp:",WHITE,BLACK,0);
	px+=24;
	sprintf(s,"%1.2fV",oscdev.tvote*dac_v_lut[dac_ch1.vpp_lut]/(1<<oscdev.res));
	LCD_ShowString(px,y,s,func_wcolor(OSC_DAC1_VPP,WHITE,DAC_COLOR,BLACK),\
	DAC_COLOR,!func_hit(OSC_DAC1_VPP));	
	px+=36;			
	LCD_ShowString(px,y,"Vdc:",WHITE,BLACK,0);
	px+=24;
	sprintf(s,"%1.2fV",oscdev.tvote*dac_v_lut[dac_ch1.vdc_lut]/(1<<oscdev.res));
	LCD_ShowString(px,y,s,func_wcolor(OSC_DAC1_VCC,WHITE,DAC_COLOR,BLACK),\
	DAC_COLOR,!func_hit(OSC_DAC1_VCC));	
}
void draw_fonts()
{
	u16 pos_down1 = oscdev.posy + oscdev.height+1;
	u16 pos_down2=pos_down1+12;
	char s[25];	
	LCD_ShowString(oscdev.posx+6, 0, "DSO By Eltecz", WHITE, BLACK, 0);
//	LCD_ShowIntNum(120,0,key_seletion,3,GREEN,BLACK);
	osc_showfreq(oscdev.posx,pos_down1);
	osc_showtdiv(oscdev.posx+84,pos_down1);
	osc_showtrig(oscdev.posx+180,pos_down1);	
	sprintf(s,"%2dX",(oscdev.hsize==0)?1:((oscdev.hsize==1)?2:(oscdev.hsize==2)?5:10));
	LCD_ShowString(oscdev.posx+150,pos_down1,s,func_wcolor(OSC_HDIV,WHITE,OSC_COLOR,BLACK),\
	func_bcolor(OSC_HDIV,OSC_COLOR,BLACK),0);			
	if(mainfunc_coi(OSC_MEMU)){
		sprintf(s,"Max:%1.2lfV Min:%1.2lfV Avg:%1.2lfV Duty:%3d%%",oscdev.vote_max*oscdev.tvote/(1<<oscdev.res),oscdev.vote_min*oscdev.tvote/(1<<oscdev.res),oscdev.vote_avg*oscdev.tvote/(1<<oscdev.res),(oscdev.vote_max-oscdev.vote_min)?d_abs(0,100,oscdev.vote_avg*1000/(oscdev.vote_max-oscdev.vote_min)/10):0);
		LCD_ShowString(oscdev.posx,pos_down2,s,WHITE,BLACK,0);	
	}
	else if(mainfunc_coi(DAC_MEMU)){
		draw_dac_ui(oscdev.posx,pos_down2);
	}		
	else{
		osc_showpwm(oscdev.posx,pos_down2);
	}		
	draw_memu();
}

void osc_findmw(u16 *buf)
{
	u16 i, lens, ff1, ff2;
	ff1 = ff2 = lens = 0;
	i = adc_length_d2;
	if (oscdev.trig_way)
	{
		while ((i < (adc_dma_length - adc_length_d2)))
		{
			if ((((oscdev.trig_way - 1) ? (buf[i] >= (oscdev.trig32) && buf[i - 1] < (oscdev.trig32)) : ((buf[i] <= oscdev.trig32) && buf[i - 1] > (oscdev.trig32)))) == 1)
			{

				if ((i > (ff1 + adc_length)))
				{
					break;
				}
				else if (lens)
				{
					ff2 = i;
				}
				else
				{
					oscdev.mw = i;
					ff1 = i;
				}
				lens++;
			}
			i++;
		}
		lens--;
	}
	if (oscdev.mw == 0)
	{
		oscdev.mw = ff1 = adc_length;
		un_trig=1;
	}
	else if ((ff2 == ff1))
	{
		oscdev.freq = 500000000/oscdev.tb* lens  ;

	}
	else
	{
		oscdev.freq = (500000000/oscdev.tb* lens / (ff2 - ff1)) ;
	}
}

void osc_window()
{
	u16 trig_p=oscdev.posy+d_abs(0,oscdev.height,oscdev.height-(oscdev.trig32*oscdev.height>>oscdev.res));
	u16 *ch_buf_p;
	u16 *fft_p;
	u16 i;
	if(oscdev.adc_ch)	ch_buf_p=adc2_buf;
	else	ch_buf_p=adc1_buf;
	osc_findmw(ch_buf_p);
	wave_gen(ch_buf_p,wdb1_p, oscdev.posy + 1, oscdev.height, oscdev.length, oscdev.mw);
	Gui_duallinemap(wdb1_p,wdb2_p, oscdev.posx, oscdev.posy, oscdev.height, oscdev.length, GREEN);
	LCD_DrawRectangle(oscdev.posx, oscdev.posy, oscdev.posx + oscdev.length, oscdev.posy + oscdev.height, BLUE);
	draw_fonts();
	p_exchange();
}
void key_search(){
	u8 ki=getkey();
	if(ki){
		delay_ms(100);
		if(key_seletion<0X10){
			switch (ki)
			{
			case KL:key_seletion=(key_seletion==MAIN_MEMU_MAX-1)?OSC_MEMU:key_seletion+1;break;
			case KM:key_seletion=(key_seletion<<4);break;		
			case KR:key_seletion=(key_seletion==OSC_MEMU)?MAIN_MEMU_MAX-1:key_seletion-1;break;			
			}
		}
		else if((key_seletion>0XF)&&(key_seletion<0X100)){
			switch (key_seletion>>4)
			{
				case OSC_MEMU:
					switch (ki)
					{
					case KL:key_seletion=(key_seletion==(OSC_MAX-1))?OSC_ADC_START:key_seletion+1;break;
					case KM:key_seletion+=(key_seletion%0X10)?0X100:-0XF;break;		
					case KR:key_seletion=(key_seletion==OSC_ADC_START)?(OSC_MAX-1):key_seletion-1;break;	
					}
					break;
				case PWM_MEMU:
					switch (ki)
					{
					case KL:key_seletion=(key_seletion==(PWM_MAX-1))?OSC_PWM_START:key_seletion+1;break;
					case KM:key_seletion+=(key_seletion%0X10)?0X100:-0X1E;break;				
					case KR:key_seletion=(key_seletion==OSC_PWM_START)?(PWM_MAX-1):key_seletion-1;break;	
					}
					break;
				case DAC_MEMU:
					switch (ki)
					{
					case KL:key_seletion=(key_seletion==(DAC_MAX-1))?OSC_DAC_START:key_seletion+1;break;
					case KM:key_seletion+=(key_seletion%0X10)?0X100:-0X2D;break;				
					case KR:key_seletion=(key_seletion==OSC_DAC_START)?(DAC_MAX-1):key_seletion-1;break;	
					}
					break;
			}
		}
		else{
			switch ((u8)key_seletion)
			{
			case ADC_TDIV:
				switch (ki)
				{
				case KL:oscdev.tdiv=(oscdev.tdiv==12)?0:oscdev.tdiv+1;break;
				case KM:key_seletion=ADC_TDIV;break;		
				case KR:oscdev.tdiv=(oscdev.tdiv==0)?12:oscdev.tdiv-1;break;	
				}
				break;
			case OSC_TWAY:
				switch (ki)
				{
				case KL:oscdev.trig_way=(oscdev.trig_way==2)?0:oscdev.trig_way+1;break;
				case KM:key_seletion=OSC_TWAY;break;		
				case KR:oscdev.trig_way=(oscdev.trig_way==0)?2:oscdev.trig_way-1;break;	
				}
				break;
			case OSC_TVOTE:
				switch (ki)
				{
				case KL:oscdev.trig_level=(oscdev.trig_level==9)?1:oscdev.trig_level+1;break;
				case KM:key_seletion=OSC_TVOTE;break;		
				case KR:oscdev.trig_level=(oscdev.trig_level==1)?9:oscdev.trig_level-1;break;
				}
				break;
			case OSC_HDIV:
				switch (ki)
				{
				case KL:oscdev.hsize=(oscdev.hsize==3)?0:oscdev.hsize+1;break;
				case KM:key_seletion=OSC_HDIV;break;		
				case KR:oscdev.hsize=(oscdev.hsize==0)?3:oscdev.hsize-1;break;	
				}	
				break;	
			case ADC_CHSEL:	
				switch (ki)
				{
				case KL:
				case KR:oscdev.adc_ch=oscdev.adc_ch?0:1;break;
				case KM:key_seletion=ADC_CHSEL;break;			
				}	
				break;
			case OSC_PWM1_FREQ:
				switch (ki)
				{
				case KL:oscdev.pwm1_freq=(oscdev.pwm1_freq==15)?0:oscdev.pwm1_freq+1;break;
				case KM:key_seletion=OSC_PWM1_FREQ;break;		
				case KR:oscdev.pwm1_freq=(oscdev.pwm1_freq==0)?15:oscdev.pwm1_freq-1;break;	
				}	
				break;
			case OSC_PWM1_DUTY:
				switch (ki)
				{
				case KL:oscdev.pwm1_duty=(oscdev.pwm1_duty==10)?0:oscdev.pwm1_duty+1;break;
				case KM:key_seletion=OSC_PWM1_DUTY;break;		
				case KR:oscdev.pwm1_duty=(oscdev.pwm1_duty==0)?10:oscdev.pwm1_duty-1;break;	
				}	
				break;	
			case OSC_PWM2_FREQ:
				switch (ki)
				{
				case KL:oscdev.pwm2_freq=(oscdev.pwm2_freq==15)?0:oscdev.pwm2_freq+1;break;
				case KM:key_seletion=OSC_PWM2_FREQ;break;		
				case KR:oscdev.pwm2_freq=(oscdev.pwm2_freq==0)?15:oscdev.pwm2_freq-1;break;	
				}	
				break;
			case OSC_PWM2_DUTY:
				switch (ki)
				{
				case KL:oscdev.pwm2_duty=(oscdev.pwm2_duty==10)?0:oscdev.pwm2_duty+1;break;
				case KM:key_seletion=OSC_PWM2_DUTY;break;		
				case KR:oscdev.pwm2_duty=(oscdev.pwm2_duty==0)?10:oscdev.pwm2_duty-1;break;	
				}	
				break;	
			case OSC_DAC1_TYPE:
				switch (ki)
				{
				case KL:dac_ch1.wave_type=(dac_ch1.wave_type==WAVE_OFF)?WAVE_SINE:dac_ch1.wave_type+1;break;
				case KM:key_seletion=OSC_DAC1_TYPE;break;		
				case KR:dac_ch1.wave_type=(dac_ch1.wave_type==WAVE_SINE)?WAVE_OFF:dac_ch1.wave_type-1;break;	
				}					
				break;	
			case OSC_DAC1_FREQ:
				switch (ki)
				{
				case KL:dac_ch1.tdiv=(dac_ch1.tdiv==14)?0:dac_ch1.tdiv+1;break;
				case KM:key_seletion=OSC_DAC1_FREQ;break;		
				case KR:dac_ch1.tdiv=(dac_ch1.tdiv==0)?14:dac_ch1.tdiv-1;break;	
				}					
				break;	
			case OSC_DAC1_DOTS:
				switch (ki)
				{
				case KL:dac_ch1.dlength=(dac_ch1.dlength==9)?0:dac_ch1.dlength+1;break;
				case KM:key_seletion=OSC_DAC1_DOTS;break;		
				case KR:dac_ch1.dlength=(dac_ch1.dlength==0)?9:dac_ch1.dlength-1;break;	
				}					
				break;	
			case OSC_DAC1_VCC:
				switch (ki)
				{
				case KL:dac_ch1.vdc_lut=(dac_ch1.vdc_lut==11)?0:dac_ch1.vdc_lut+1;break;
				case KM:key_seletion=OSC_DAC1_VCC;break;		
				case KR:dac_ch1.vdc_lut=(dac_ch1.vdc_lut==0)?11:dac_ch1.vdc_lut-1;break;	
				}					
				break;	
			case OSC_DAC1_VPP:
				switch (ki)
				{
				case KL:dac_ch1.vpp_lut=(dac_ch1.vpp_lut==11)?0:dac_ch1.vpp_lut+1;break;
				case KM:key_seletion=OSC_DAC1_VPP;break;		
				case KR:dac_ch1.vpp_lut=(dac_ch1.vpp_lut==0)?11:dac_ch1.vpp_lut-1;break;	
				}					
				break;	
			}
		}
	}
}
void osc_app()
{
	runtime++;
	if(key_seletion!=old_seletion){
		Adc_DMA_Init(oscdev.tdiv,oscdev.adc_ch);
		LCD_Clear(BLACK);
		PWM_TIM_INIT(oscdev.pwm1_freq,oscdev.pwm2_freq,oscdev.pwm1_duty,oscdev.pwm2_duty);
		dac_init();
	}
	osc_reload();
	Dual_ADC_DMA_START(adc_dma_length);
	osc_window();
	key_search();
	
}