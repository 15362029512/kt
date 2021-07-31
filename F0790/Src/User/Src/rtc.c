/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* global variables ----------------------------------------------------------*/

uint8_t flag_minute = 0;
uint8_t	flag_second = 0;

code uint8_t day_of_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};			
code uint8_t table_week[] 	 = {0,3,3,6,1,4,6,2,5,0,3,5}; 							//���������ݱ�	 

		
_CALENDAR  			calendar = {2018,1,1,12,0,0};										//2018-1-1 12:0:0	;												
#define year		calendar.st_year
#define month		calendar.st_month
#define day			calendar.st_day
#define hour		calendar.st_hour
#define minute	    calendar.st_minute
#define second	    calendar.st_second
#define week		calendar.st_week

_ALARM		rtc_alarm[ALARM_CNT] = 
						{
							day_1_7,7,0,0,
							day_1_7,7,0,0,
						};

uint8_t			flag_current_alarmed;				//��ǰ���������һ������
uint8_t			flag_current_seting_alarm;	//��ǰ���õ�����һ������

uint8_t 		flag_24hours_en = 1;				//1��ʱ��24Сʱ��
uint8_t 		flag_alarm1_en = 0;					//1������1����
uint8_t 		flag_alarm2_en = 0;					//1������2����						
						
uint8_t 		flag_alarm_style_ring = 0;	//1���������巽ʽ��������
uint8_t 		flag_alarm_style_shake = 0;	//1���������巽ʽ���𶯿�

#ifdef SHOW_ALARM_TIME_EN
uint8_t			flag_show_which_alarm;			//������ʾ�ĸ����ӵ�ʱ��5��
#endif

uint16_t 		flag_enter_snooze = 0;			//flag_sleep_again = 0;						//1������̰˯

uint8_t			flag_current_snooze = 0;		//��ǰ̰˯������һ������   
uint8_t 		snooze_time = 9;						//̰˯ʱ��		��λ������

uint8_t 		flag_alarm_timeout = 0;			//1������ʱ�䵽��������	

uint16_t        alarmed_time = 0;						//�������徭����ʱ��
uint8_t         alarm_ring_step = 0;				//
uint8_t         alarm_shake_step = 0;
/*********************************************************************************************************************/

/*********************************************************************************************************************/



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   updata_calendar
    @�������ܣ�   ��������
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void updata_calendar(void)
{
	set_rtc(calendar.st_year,
					calendar.st_month,
					calendar.st_day,
					calendar.st_hour,
					calendar.st_minute,
					calendar.st_second);
	get_calendar();
}

				
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   rtc_init
    @�������ܣ�   RTC��ʼ��
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void rtc_init(void)
{

	
	
	RTCON = 0xa2;										//RTCʱ��ʹ�ܣ������жϹرգ������ж�ʹ�ܣ�ʱ��дʹ��
//	RTCIF = 0XFF;
//	RTMSS = 0;											//3.9ms  Ĭ��
	Delay_50us(10);									//RTCʹ�ܺ������ʱ300us��д��ʱ�䣬 ����д��ʱ�������Ч��

	updata_calendar();							
	INT8EN = 1;											//����RTC�ж�		
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   is_leap_year
    @�������ܣ�   �ж���ƽ�껹������
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

uint8_t is_leap_year(uint16_t _year)
{ 
	if (_year % 4 == 0) //�����ܱ�4����
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
            {
                return 1;//�����00��β,��Ҫ�ܱ�400����  
            }   
			else 
            {
                return 0;   
            }
		}
        else 
        {
            return 1;  
        } 
	}
    else
    {
        return 0;  
    } 
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   is_over_max_days
    @�������ܣ�   �ж������Ƿ񳬳��·ݵ��������ֵ
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint8_t is_over_max_days(uint16_t _year,uint8_t _month,uint8_t _day)
{
	if (_month == 4 || _month == 6 || _month == 9 || _month == 11)
	{
		if (_day > 30) 
        {
            return 1;
        }
		else
        {
            return 0;
        }
	} 
	else if (_month == 2)
	{
		if (is_leap_year(_year))
		{
			if (_day > 29) 
            {
                return 1;
            }
			else 
            {
                return 0;
            }
	    }
		else
		{
			if (_day > 28)
            {
                return 1;
            }
			else 
            {
                return 0;
            }
		}
	}
	else
	{
		if (_day > 31) 
        {
            return 1;
        }
		else 
        {
            return 0;
        }
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   set_max_days_of_month
    @�������ܣ�   ����ĳһ���µ��������ֵ 
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void set_max_days_of_month(uint16_t _year,uint8_t _month)
{
	if (_month == 4 || _month == 6 || _month == 9 || _month == 11)
	{
		calendar.st_day = 30;
	}
	else if (_month == 2)
	{
		if (is_leap_year(_year))
		{
			calendar.st_day = 29;
		}
		else
		{
			calendar.st_day = 28;
		}
	}
//	else
//	{
//		calendar.st_day = 31;
//	}	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_calendar
    @�������ܣ�   ��ȡ������Ϣ(��ȡRTC�Ĵ��� �� ����) ���� ������ ʱ���� ����  
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void get_calendar(void)
{
	uint16_t day_remain = 0;
	uint16_t get_year = 1970;
	uint8_t  get_month = 0;
	
	second = RTCS;		
	minute = RTCM;
	hour = RTCH & 0x1f;														//RTCH��5λ��ʾСʱ
	week = RTCH >> 5;															//RTCH��3λ��ʾ����
	day_remain = RTCDH ;
	day_remain = (day_remain << 8) | RTCDL;				//��ȡRTCD�Ĵ����������ֵ
	
	/***************ѭ���жϵõ����********************/
	while(day_remain >= 365)			
	{
		if(is_leap_year(get_year))	//���������
		{
			if(day_remain >= 366) 		//����������һ��
			{
                day_remain -= 366;
            }				
			else 
            {
                break;                              //����δ����һ�ֱ꣬���˳�
            }
		}
		else 
        {
            day_remain -= 365;          //�����ƽ���ҳ���һ��
        }		
		get_year ++;
	}	
	year = get_year;							//�õ���								
	
	/***************ѭ���жϵõ� �� ��******************/
	while(day_remain >= 28)
	{
		if(is_leap_year(year) && get_month == 1)
		{
			if(day_remain >= 29) 
            {
                day_remain -= 29;
            }
			else 
            {
                break;
            }
		}
		else
		{
			if(day_remain >= day_of_month[get_month]) 
            {
                day_remain -= day_of_month[get_month];
            }
			else 
            {
                break;
            }
		}
		get_month ++;
	}
	month = get_month + 1;			//�õ���
	day = day_remain + 1;				//�õ���
}

																
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_week
    @�������ܣ�   ���빫�����ڵõ�����(ֻ����1901-2099��) 
    @��ڲ�����   year,month,day������������ 
    @���ڲ�����   ���ں�	0 ~ 6   ->  ���� ~ ����
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t get_week(uint16_t _year,uint8_t _month,uint8_t _day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH =_year/100;	yearL = _year%100; 
	
	if (yearH > 19) 
    {
        yearL += 100;// ���Ϊ21����,�������100  
    }
	// ����������ֻ��1900��֮���  
	temp2 = yearL + yearL / 4;
	temp2 = temp2 % 7; 
	temp2 = temp2 + _day + table_week[_month - 1];
	if (yearL % 4 == 0 && _month < 3) 
    {
        temp2--;
    }
	return(temp2 % 7);
}	


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   set_rtc
    @�������ܣ�   ����������Ϣ���������е�ֵд��Ĵ�����
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void set_rtc(uint16_t s_year,uint8_t s_month,uint8_t s_day,uint8_t s_hour,uint8_t s_minute,uint8_t s_second)
{
	uint16_t rtc_day = 0;		
	uint16_t year_number = 1970;
	uint8_t  month_number = 0;
	uint8_t  s_week = 0;
	/*ͨ��Ҫ���õ� ������ ����Ҫд�� RTCD �Ĵ�����ֵ*/
	for (; year_number < s_year; year_number++)
	{
		if(is_leap_year(year_number)) 
        {
            rtc_day += 366;
        }
		else 
        {
            rtc_day += 365;
        }     
	}	
//	s_month -= 1;
	for (; month_number < s_month - 1; month_number++)	
	{
		rtc_day += day_of_month[month_number];		
		if(is_leap_year(s_year) && (month_number == 1))	//��������꣬��2����
		{
            rtc_day += 1;											  					//���һ��
        }	
	}
	rtc_day += s_day - 1 ;														//�õ��ܵ�����	
	s_week = get_week(s_year,s_month,s_day);					//�õ����ں�
	if (s_week == 0) 
    {
        s_week = 7;
    }
	/************************************************/
	RTC_W_EN;								//RTCʱ�ӼĴ���дʹ��					
	RTCS = s_second;
	RTCM = s_minute;
	RTCH = (s_week << 5) | s_hour;							//week�ĵ�3λ�洢��RTCH�ĸ�3λ
																							//hour�洢��RTCH�ĵ�5λ	
	RTCDH = (uint8_t)(rtc_day >> 8);
	RTCDL = (uint8_t)rtc_day;

	Delay_50us(1);
	RTC_W_DIS;							//RTCʱ�ӼĴ���д��ֹ
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   alarm_timeout_opera
    @�������ܣ�   ����������Ϣ���������е�ֵд��Ĵ�����
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void alarm_timeout_opera(uint8_t _alarm)
{
	flag_current_alarmed = _alarm;		//
	flag_alarm_timeout = 1;						//����ʱ�䵽
	alarmed_time = 0;	
	
	alarm_ring_step = 0;							
	alarm_ring_cycle = 2;

	alarm_shake_step = 0;							
	alarm_shake_cycle = 2;		
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   enter_snooze
    @�������ܣ�   ����̰˯
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void enter_snooze(void)
{
	uint8_t snooze_minute_temp = 0, snooze_hour_temp = 0;
	
	flag_alarm_timeout = !flag_alarm_timeout;   		//�ر�����
	flag_enter_snooze = 1;													//����̰˯
	flag_current_snooze = flag_current_alarmed;			//��¼��ǰ˯�ߵ�����һ������
//	rtc_alarm[ALARM_SNOOZE].a_day = rtc_alarm[flag_current_alarmed].a_day;		//��¼��ǰ˯�ߵ����ӵĹ���������
	//����̰˯���ӣ�����ʱ�� = ��ǰʱ�� + ̰˯ʱ�� (9����)							
	snooze_minute_temp = RTCM + snooze_time;
	snooze_hour_temp = (RTCH & 0x1f);
	if (snooze_minute_temp >= 60)
	{
		snooze_minute_temp -= 60;
		snooze_hour_temp += 1; 
	}
	rtc_alarm[ALARM_SNOOZE].a_hour = snooze_hour_temp;
	rtc_alarm[ALARM_SNOOZE].a_minute = snooze_minute_temp;
	rtc_alarm[ALARM_SNOOZE].a_second = RTCS;	
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   snooze_alarm_test
    @�������ܣ�   ����̰˯���ӣ�ÿ�����һ��
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void snooze_alarm_test(void)
{
	if(flag_enter_snooze)							//������뵽 ̰˯
	{
		if((RTCH & 0x1f) == rtc_alarm[ALARM_SNOOZE].a_hour && RTCM == rtc_alarm[ALARM_SNOOZE].a_minute && RTCS == rtc_alarm[ALARM_SNOOZE].a_second)
		{
			alarm_timeout_opera(flag_current_snooze);
		}
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   alarm_test
    @�������ܣ�   ���Ӳ��ԣ�ÿ�ֲ���һ��
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void alarm_test(void)
{
	if(lcd_mode == mode_clock)
	{
		//------------------ ����1���� ------------			
		if(flag_alarm1_en)
		{
			if((RTCH & 0x1f) == rtc_alarm[ALARM_1].a_hour && RTCM == rtc_alarm[ALARM_1].a_minute)
			{
                //alarm_timeout_opera(ALARM_1);

				switch(rtc_alarm[ALARM_1].a_day)
				{
					case day_1_1:																
									flag_alarm1_en = !flag_alarm1_en;		//�ر�����
									alarm_timeout_opera(ALARM_1);
									break;
					case day_1_5:	
									if((RTCH >> 5 != 6) && (RTCH >> 5 != 7))
									{
										alarm_timeout_opera(ALARM_1);									
									}
									break;
					case day_1_7:
									alarm_timeout_opera(ALARM_1);		
									break;
					case day_6_7:	
									if((RTCH >> 5 == 6) || (RTCH >> 5 == 7))
									{
										alarm_timeout_opera(ALARM_1);									
									}
									break;
                    default:break;                
				}	
			}
		}	
		//------------------ ����2���� ------------			
		if(flag_alarm2_en)
		{
			if((RTCH & 0x1f) == rtc_alarm[ALARM_2].a_hour && RTCM == rtc_alarm[ALARM_2].a_minute)
			{
			  //alarm_timeout_opera(ALARM_2);
				switch(rtc_alarm[ALARM_2].a_day)
				{
					case day_1_1:
									flag_alarm2_en = !flag_alarm2_en;		//�ر�����
									alarm_timeout_opera(ALARM_2);
									break;
					case day_1_5:	
									if((RTCH >> 5 != 6) && (RTCH >> 5 != 7))
									{
										alarm_timeout_opera(ALARM_2);								
									}
									break;
					case day_1_7:
									alarm_timeout_opera(ALARM_2);
									break;
					case day_6_7:	
									if((RTCH >> 5 == 6) || (RTCH >> 5 == 7))
									{
										alarm_timeout_opera(ALARM_2);								
									}
									break;
                    default:break;                 
				}	
			}
		}	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   alarmed_time_test
    @�������ܣ�   �����������𾭹���ʱ�䣬ÿ�����һ��
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void alarmed_time_test(void)
{
	//------------------��������ʱ�䳤�ȴ���------------
	if (flag_alarm_timeout)
	{
		if (++alarmed_time >= /*rtc_alarm[flag_current_alarmed].a_ringtime*/ALARM_TIME * 60)
		{
			flag_alarm_timeout = 0;
            if(flag_enter_snooze)
        	{
                flag_enter_snooze = !flag_enter_snooze;
            }	
		}
	}
//	else
//	{
//		alarmed_time = 0;
//	}
}




/*********************************************************************************************************************
��	�ܣ�   10ms���ڵ���
��	����
����ֵ��
*********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   rtc_alarm_opera
    @�������ܣ�   10ms���ڵ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void rtc_alarm_opera(void) 
{	
	static uint8_t changed = 0;
    if (lcd_mode != mode_test)
    {
        if (flag_dc_5v)
	    {
    		if (flag_alarm_timeout)		
    		{
    			flag_enter_snooze = 0;								//����̰˯����
    			if (!changed)													//-------��һ�ν�������
    			{
    				changed = 1;												//���ñ�־
    			}	
    			if (flag_alarm_style_shake)						//****����ʱ�䵽ʱ �𶯵Ĳ���
    			{
    				alarm_shake();											//���ڵ���		
    			}
                
    			if (flag_alarm_style_ring)							//****����ʱ�䵽ʱ �����Ĳ���
    			{
    				alarm_tone();												//���ڵ���
    			}
    		}
    		else /*************** ������� ********************/
    		{	
    			if (changed)
    			{
    				changed = 0;
    				motor_stop();    				
    			}           
                ring_stop();
    		}	
        }
    }
}

#if 0
void rtc_alarm_opera(void) 
{	
	static uint8_t changed = 0;
    if (lcd_mode != mode_test)
    {
        spk_cmd_ing = 0;
        alarm_tone();	
        //send_ring_cmd(g_current_ring_vol_level);
        //alarm_tone();	

        /*
        if (flag_dc_5v)
	    {
            alarm_tone();	    			
        }
        */
    }
}
#endif

#if 0
void rtc_alarm_opera(void) 
{	
	static uint8_t changed = 0;
	
	if (lcd_mode != mode_test)
    {
        if (flag_dc_5v)
	    {
	        if (flag_alarm_timeout)		
	        {
                flag_enter_snooze = 0;								//����̰˯����
    			if (!changed)													//-------��һ�ν�������
    			{
    				changed = 1;												//���ñ�־
    			}
                if (flag_alarm_style_shake)						//****����ʱ�䵽ʱ �𶯵Ĳ���
    			{
    				 alarm_tone();		
    			}
            }
            if (!flag_alarm_timeout)
            {
                if (changed)
    			{
    				changed = 0;
    				motor_stop();    				
    			}
				
                ring_stop();
            }
		}
	}
}
#endif




/*********************************************************************************************************************
��	�ܣ�
��	����
����ֵ��
*********************************************************************************************************************/
//void factory_set(void)
//{
//	u8 i;
//	flag_24hours_en = 1;						//24Сʱ��
//	set_rtc(factory.st_year,				//���ó���ʱ��
//					factory.st_month,
//					factory.st_day,
//					factory.st_hour,
//					factory.st_minute,
//					factory.st_second);			
//	get_calendar();	
//	
//	for(i = 0; i < 2; i++)					//�������Ӳ���
//	{
//		rtc_alarm[i].a_ringtime = 10;			//����ʱ����10����
//		rtc_alarm[i].a_day = day_1_7;			//�������ڣ�ÿ��
//		rtc_alarm[i].a_hour = 7;					//����ʱ�� 7
//		rtc_alarm[i].a_minute = 0;
//		rtc_alarm[i].a_second = 0;	
//	}
//	
//	snooze_time = 9;								//̰˯ʱ���� 9����	
//}




