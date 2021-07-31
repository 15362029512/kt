#ifndef _RTC_H_
#define _RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "delay.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t st_year;
	uint8_t st_month;
	uint8_t st_day;
	uint8_t st_hour;					
	uint8_t st_minute;
	uint8_t st_second;
	uint8_t st_week;		
}_CALENDAR;


typedef struct
{
	uint8_t a_day;						//���� ����   														//���λ�� ���ӿ��ر�־ ��1�����ӿ�   0�����ӹء�
	uint8_t a_hour;					//���� ʱ				
	uint8_t a_minute;				//���� ��
	uint8_t a_second;				//���� ��
}_ALARM;



typedef enum
{
	day_1_1,						//��������
	day_1_5,						//����������
	day_1_7,						//ÿ������
	day_6_7,						//��ĩ����
}_ALARM_DAY;


/* Exported macro ------------------------------------------------------------*/
#define ALARM_CNT					4					//���ӵ�����
#define ALARM_1						0					//��һ������		
#define ALARM_2						1					//�ڶ�������
#define ALARM_SNOOZE			    2					//̰˯����
#define ALARM_TEMP				    3					//��������ʱ�� �м����

#define RTC_HF		(1<<1)


#define RTC_W_EN		RTCON |= 0x02														//RTCʱ�ӼĴ���дʹ��
#define RTC_W_DIS		RTCON &= 0xfd														//RTCʱ�ӼĴ���д��ֹ


#define F_12HOURS				0x80
#define MODE_12HOUR				1
#define MODE_24HOUR				0


#define ALARM_TIME        10    //��λ������




#define ALARM_TIME_MIN		1					//����ʱ����Сֵ����λ������
#define ALARM_TIME_MAX		30				//����ʱ�����ֵ����λ������

#define SNOOZE_TIME_MIN		1					//̰˯ʱ����Сֵ����λ������
#define SNOOZE_TIME_MAX		15//60				//̰˯ʱ�����ֵ����λ������

#define SNOOZE_HIDE_TIME		1				//̰˯������˸���ʱ�䣬��λ������ 500ms	
#define SNOOZE_SHOW_TIME		2

#define BAT_SHOW_TIME				1
#define BAT_HIDE_TIME				3


#define use_clock_dot								//����λ��˸ �� ̰˯������˸ �� flag_clockdot_show ������

													
#define DAY_OF_COMMONYEAR		365		//ƽ��һ�������
#define DAY_OF_LEAPYEAR			366		//����һ�������




/* Exported variables --------------------------------------------------------*/  
extern _CALENDAR    calendar;
extern _ALARM		rtc_alarm[ALARM_CNT];
extern code uint8_t day_of_month[12];


extern uint8_t		flag_current_alarmed;						//��ǰ���������һ������
extern uint8_t		flag_current_seting_alarm;			//��ǰ���õ�����һ������
extern uint8_t 		flag_24hours_en;
extern uint8_t 		flag_alarm1_en;
extern uint8_t 		flag_alarm2_en;


extern uint8_t 		flag_alarm_style_ring;
extern uint8_t 		flag_alarm_style_shake;


extern uint8_t		flag_show_which_alarm;

extern uint8_t 		flag_alarm_timeout;
extern uint16_t 		flag_enter_snooze;
extern uint8_t		flag_snooze_show;
extern uint8_t		flag_current_snooze;
extern uint8_t 		snooze_time;


extern uint8_t		flag_half_second;
extern uint8_t		flag_minute;
extern uint8_t		flag_second;

extern uint8_t		flag_current_seting_alarm;

extern uint16_t     alarmed_time;
extern uint8_t      alarm_ring_step;
extern uint8_t      alarm_shake_step;



/* Exported functions --------------------------------------------------------*/  
extern void rtc_init(void);
extern void get_calendar(void);
extern void set_rtc(uint16_t s_year,uint8_t s_month,uint8_t s_day,uint8_t s_hour,uint8_t s_minute,uint8_t s_second);
extern uint8_t is_over_max_days(uint16_t _year,uint8_t _month,uint8_t _day);
extern void set_max_days_of_month(uint16_t _year,uint8_t _month);
extern uint8_t is_leap_year(uint16_t _year);
extern void updata_calendar(void);
extern void enter_snooze(void);
extern void alarm_test(void);
extern void snooze_alarm_test(void);
extern void alarmed_time_test(void);
extern void nothing_action_test(void);
extern void rtc_alarm_opera(void);

#endif
