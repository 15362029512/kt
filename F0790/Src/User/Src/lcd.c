/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
/* Private typedef -----------------------------------------------------------*/



/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
code uint8_t num_table[NUM_TABLE_SIZE] = //b7 - b1 : _A - _G
{
	0xfc,		//0  
	0x60,		
	0xda,
	0xf2,
	0x66,
	0xb6,
	0xbe,
	0xe0,
	0xfe,
	0xf6,		//9	
	0x02,		//-
	0x6e,		//H
	0x0a,		//r
};


uint8_t  		lcd_mode = mode_9v_power_on_wait;									//��ǰLCD ����ʾģʽ
SET_LOCATION	set_location = 0;							//����ģʽ�� ��ǰҪ���õ�λ��
uint8_t			flag_set_flash_en = 0;				// 1������λ������˸
uint8_t			set_no_flash_gap = 0;					//����λ��������˸�ļ��ʱ��
uint8_t			flag_clockdot_show = 1;				// 1��ʱ�������ʾ				     0��ʱ����㲻��ʾ
uint8_t			show_in_bat_5sec = 0;					//�ڵ�ع���ʱ����ʾʱ�� 5 ��



/* Private functions ---------------------------------------------------------*/
static void smg1_deal(uint8_t deal,uint8_t num);
static void smg2_deal(uint8_t deal,uint8_t num);
static void smg3_deal(uint8_t deal,uint8_t num);
static void smg4_deal(uint8_t deal,uint8_t num);

/* global variables ----------------------------------------------------------*/




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_num_deal
    @�������ܣ�   7�����ִ�����ʾ �� ���ؾ�������� 
    @��ڲ�����   where��Ҫ�����λ�ã�ȡֵ��Χ NUMBER_LOCATION    
                   deal������ķ�ʽ����ʾ������  SHOW / HIDE  
                   num ��Ҫ��ʾ����ֵ��ȡֵ��Χ  NUMBER
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_num_deal(NUMBER_LOCATION which,uint8_t deal,uint8_t num)
{
	switch(which)
	{
		case location_num_1:	smg1_deal(deal,num_table[num]); break;
		case location_num_2:	smg2_deal(deal,num_table[num]); break;
		case location_num_3:	smg3_deal(deal,num_table[num]); break;
		case location_num_4:	smg4_deal(deal,num_table[num]); break;
        default:break;
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_symbol_deal
    @�������ܣ�   ͼ�괦�� ����ʾ �� ���أ�
    @��ڲ�����   which��Ҫ������һ�����ţ�ȡֵ��Χ SYMBOL
                   deal��	SHOW(1) ����ʾ   HIDE(0) ������ʾ
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_symbol_deal(SYMBOL which,uint8_t deal)
{
	switch(which)
	{
		case symbol_am:	        SYMB_AM_S(deal);break;
		case symbol_bat:		SYMB_BAT(deal);break;
		case symbol_pm:			SYMB_PM(deal);break;
		case symbol_al1:		SYMB_AL1(deal);break;
		case symbol_al2:		SYMB_AL2(deal);break;
		case symbol_snooze:		SYMB_SNOOZE(deal);break;
		case symbol_sec1:		SYMB_SEC1(deal);break;
        case symbol_sec2:       SYMB_SEC2(deal);break;
        case symbol_alarm_out:  LED_SW_CTL(deal);break;
        default:break;
	}        
}







/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_init
    @�������ܣ�   lcd_init
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   Ҫ����lcd����ʹ�ܱ���
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_init(void)
{
    GPIO_Init(SMG1_A_PIN,OUTPUT);			
	GPIO_Init(SMG1_B_PIN,OUTPUT);	
	GPIO_Init(SMG1_C_PIN,OUTPUT);	
	GPIO_Init(SMG1_D_PIN,OUTPUT);	
	GPIO_Init(SMG1_E_PIN,OUTPUT);	
	GPIO_Init(SMG1_F_PIN,OUTPUT);	
	GPIO_Init(SMG1_G_PIN,OUTPUT);	
	
	GPIO_Init(SMG2_A_PIN,OUTPUT);			
	GPIO_Init(SMG2_B_PIN,OUTPUT);	
	GPIO_Init(SMG2_C_PIN,OUTPUT);	
	GPIO_Init(SMG2_D_PIN,OUTPUT);	
	GPIO_Init(SMG2_E_PIN,OUTPUT);	
	GPIO_Init(SMG2_F_PIN,OUTPUT);	
	GPIO_Init(SMG2_G_PIN,OUTPUT);

	GPIO_Init(SMG3_A_PIN,OUTPUT);			
	GPIO_Init(SMG3_B_PIN,OUTPUT);	
	GPIO_Init(SMG3_C_PIN,OUTPUT);	
	GPIO_Init(SMG3_D_PIN,OUTPUT);	
	GPIO_Init(SMG3_E_PIN,OUTPUT);	
	GPIO_Init(SMG3_F_PIN,OUTPUT);	
	GPIO_Init(SMG3_G_PIN,OUTPUT);	

	GPIO_Init(SMG4_A_PIN,OUTPUT);			
	GPIO_Init(SMG4_B_PIN,OUTPUT);	
	GPIO_Init(SMG4_C_PIN,OUTPUT);	
	GPIO_Init(SMG4_D_PIN,OUTPUT);	
	GPIO_Init(SMG4_E_PIN,OUTPUT);	
	GPIO_Init(SMG4_F_PIN,OUTPUT);	
	GPIO_Init(SMG4_G_PIN,OUTPUT);	
	
	GPIO_Init(SYMB_AM_PIN,OUTPUT);
	GPIO_Init(SYMB_BAT_PIN,OUTPUT);
	GPIO_Init(SYMB_PM_PIN,OUTPUT);
	GPIO_Init(SYMB_AL1_PIN,OUTPUT);
	GPIO_Init(SYMB_AL2_PIN,OUTPUT);
	GPIO_Init(SYMB_SNOOZE_PIN,OUTPUT);
	GPIO_Init(SYMB_SEC1_PIN,OUTPUT);
    GPIO_Init(SYMB_SEC2_PIN,OUTPUT);

    GPIO_Init(LED_SW_CTL_PIN,OUTPUT);    
    lcd_clear();
    
}






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_clear
    @�������ܣ�   lcd_clear
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_clear(void)
{
	smg1_deal(HIDE,num_table[num_8]);
	smg2_deal(HIDE,num_table[num_8]);
	smg3_deal(HIDE,num_table[num_8]);
	smg4_deal(HIDE,num_table[num_8]);

    SYMB_AM_S(HIDE);	
    SYMB_BAT(HIDE);  
    SYMB_PM(HIDE);	  
    SYMB_AL1(HIDE);	  
    SYMB_AL2(HIDE);	  
    SYMB_SNOOZE(HIDE);
    SYMB_SEC1(HIDE);	
    SYMB_SEC2(HIDE);	

    LED_SW_CTL(HIDE);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_show_all
    @�������ܣ�   lcd_show_all
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_show_all(void)
{
	smg1_deal(SHOW,num_table[num_8]);
	smg2_deal(SHOW,num_table[num_8]);
	smg3_deal(SHOW,num_table[num_8]);
	smg4_deal(SHOW,num_table[num_8]);

    SYMB_AM_S(SHOW);	
    SYMB_BAT(SHOW);	  
    SYMB_PM(SHOW);	  
    SYMB_AL1(SHOW);	  
    SYMB_AL2(SHOW);	  
    SYMB_SNOOZE(SHOW);
    SYMB_SEC1(SHOW);	
    SYMB_SEC2(SHOW);	

    LED_SW_CTL(HIDE);

}








/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   smg1_deal
    @�������ܣ�   ��ʾ �� ���� smg
    @��ڲ�����   deal��ȡֵ SHOW(��ʾ)  HIDE(����)  num��	ȡֵ num_table[]����ʾʱ��Ҫ��ʾ������	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg1_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//�������ʾ
	{		
		SMG1_A(num & 0x80);			//���� num ��ʾ �� ����ʾ ÿ���ʶ�
		SMG1_B(num & 0x40);
		SMG1_C(num & 0x20);
		SMG1_D(num & 0x10);
		SMG1_E(num & 0x08);
		SMG1_F(num & 0x04);
		SMG1_G(num & 0x02);	
	}
	else// if(deal == HIDE)		//����ǲ���ʾ
	{
		SMG1_A(HIDE);									//ֱ�� ����ʾ ���бʶ�
		SMG1_B(HIDE);
		SMG1_C(HIDE);
		SMG1_D(HIDE);
		SMG1_E(HIDE);
		SMG1_F(HIDE);
		SMG1_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   smg2_deal
    @�������ܣ�   ��ʾ �� ���� smg
    @��ڲ�����   deal��ȡֵ SHOW(��ʾ)  HIDE(����)  num��	ȡֵ num_table[]����ʾʱ��Ҫ��ʾ������	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg2_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//�������ʾ
	{
		SMG2_A(num & 0x80);			//���� num ��ʾ �� ����ʾ ÿ���ʶ�
		SMG2_B(num & 0x40);
		SMG2_C(num & 0x20);
		SMG2_D(num & 0x10);
		SMG2_E(num & 0x08);
		SMG2_F(num & 0x04);
		SMG2_G(num & 0x02);	
	}
	else										//����ǲ���ʾ
	{
		SMG2_A(HIDE);									//ֱ�� ����ʾ ���бʶ�
		SMG2_B(HIDE);
		SMG2_C(HIDE);
		SMG2_D(HIDE);
		SMG2_E(HIDE);
		SMG2_F(HIDE);
		SMG2_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   smg3_deal
    @�������ܣ�   ��ʾ �� ���� smg
    @��ڲ�����   deal��ȡֵ SHOW(��ʾ)  HIDE(����)  num��	ȡֵ num_table[]����ʾʱ��Ҫ��ʾ������	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg3_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//�������ʾ
	{
		SMG3_A(num & 0x80);			//���� num ��ʾ �� ����ʾ ÿ���ʶ�
		SMG3_B(num & 0x40);
		SMG3_C(num & 0x20);
		SMG3_D(num & 0x10);
		SMG3_E(num & 0x08);
		SMG3_F(num & 0x04);
		SMG3_G(num & 0x02);	
	}
	else										//����ǲ���ʾ
	{
		SMG3_A(HIDE);									//ֱ�� ����ʾ ���бʶ�
		SMG3_B(HIDE);
		SMG3_C(HIDE);
		SMG3_D(HIDE);
		SMG3_E(HIDE);
		SMG3_F(HIDE);
		SMG3_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   smg4_deal
    @�������ܣ�   ��ʾ �� ���� smg
    @��ڲ�����   deal��ȡֵ SHOW(��ʾ)  HIDE(����)  num��	ȡֵ num_table[]����ʾʱ��Ҫ��ʾ������	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg4_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//�������ʾ
	{
		SMG4_A(num & 0x80);			//���� num ��ʾ �� ����ʾ ÿ���ʶ�
		SMG4_B(num & 0x40);
		SMG4_C(num & 0x20);
		SMG4_D(num & 0x10);
		SMG4_E(num & 0x08);
		SMG4_F(num & 0x04);
		SMG4_G(num & 0x02);	
	}
	else										//����ǲ���ʾ
	{
		SMG4_A(HIDE);									//ֱ�� ����ʾ ���бʶ�
		SMG4_B(HIDE);
		SMG4_C(HIDE);
		SMG4_D(HIDE);
		SMG4_E(HIDE);
		SMG4_F(HIDE);
		SMG4_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   show_year
    @�������ܣ�   ��ʾ��
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void show_year(void)
{
	lcd_num_deal(location_num_1,SHOW,calendar.st_year / 1000);
	lcd_num_deal(location_num_2,SHOW,calendar.st_year % 1000 / 100);
	lcd_num_deal(location_num_3,SHOW,calendar.st_year % 100 / 10);
	lcd_num_deal(location_num_4,SHOW,calendar.st_year % 10);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   show_month_day
    @�������ܣ�   ��ʾ����
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void show_month_day(void)
{
	uint8_t temp;

	lcd_symbol_deal(symbol_sec1,SHOW);		
	/**************************************************/
	temp = calendar.st_month / 10;
	lcd_num_deal(location_num_1,(bit)temp,temp);
	lcd_num_deal(location_num_2,SHOW,calendar.st_month % 10);	
	/**************************************************/
	temp = calendar.st_day / 10;
	lcd_num_deal(location_num_3,(bit)temp,temp);
	lcd_num_deal(location_num_4,SHOW,calendar.st_day % 10);	
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   show_hour_minute
    @�������ܣ�   ��ʾʱ��
    @��ڲ�����   time_from ��Ҫ��ʾ�����ݴ�������		TIME_FROM	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void show_hour_minute(TIME_FROM time_from)
{
	uint8_t hour_temp = 0;
	uint8_t minute_temp = 0;	
	uint8_t hour_temp_temp = 0;
	
	uint8_t temp = 0;
	static uint8_t pm = 0;
	static uint8_t am = 0;
	switch(time_from)		
	{
		case from_calendar: 			hour_temp = calendar.st_hour;			
															minute_temp = calendar.st_minute;
															break;
		case from_alarm_1: 				hour_temp = rtc_alarm[ALARM_1].a_hour;
															minute_temp = rtc_alarm[ALARM_1].a_minute;	
															break;
		case from_alarm_2: 				hour_temp	= rtc_alarm[ALARM_2].a_hour;	
															minute_temp = rtc_alarm[ALARM_2].a_minute;
															break;
	}
	/********************* ��ʾ ʱ ******************************/
	if(flag_24hours_en)											//�����24Сʱ��
	{
		hour_temp_temp = hour_temp;
        lcd_symbol_deal(symbol_am,HIDE);							//�ر� am
		lcd_symbol_deal(symbol_pm,HIDE);							//�ر� pm		
	}		
	else																		//�����12Сʱ��
	{
		if((hour_temp >= 0) && (hour_temp < 12)) 			//am				
		{
			pm = 0;
            am = 1;
			hour_temp_temp = hour_temp;
		}
		else																					//pm
		{
			pm = 1;
            am = 0;
			hour_temp_temp = hour_temp - 12;			
		}
        lcd_symbol_deal(symbol_am,am);								//��ʾ am
		lcd_symbol_deal(symbol_pm,pm);								//��ʾ pm
		if(hour_temp_temp == 0) hour_temp_temp = 12;	
	}
	temp = hour_temp_temp / 10;
	lcd_num_deal(location_num_1,(bit)temp,temp);
	lcd_num_deal(location_num_2,SHOW,hour_temp_temp % 10);
	/********************* ��ʾ �� ******************************/
	lcd_num_deal(location_num_3,SHOW,minute_temp / 10);
	lcd_num_deal(location_num_4,SHOW,minute_temp % 10);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   show_12_24hours
    @�������ܣ�   ��ʾ24Сʱ�ƻ�12Сʱ��
    @��ڲ�����   time_from ��Ҫ��ʾ�����ݴ�������		TIME_FROM	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void show_12_24hours(void)
{
	if(flag_24hours_en)
	{
		lcd_num_deal(location_num_1,SHOW,num_2);
		lcd_num_deal(location_num_2,SHOW,num_4);		
	}
	else
	{
		lcd_num_deal(location_num_1,SHOW,num_1);
		lcd_num_deal(location_num_2,SHOW,num_2);		
	}
	lcd_num_deal(location_num_3,SHOW,num_H);
	lcd_num_deal(location_num_4,SHOW,num_r);			
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   show_alarm_day_style
    @�������ܣ�   ��ʾ������ظ�ʱ��
    @��ڲ�����   ��	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void show_alarm_day_style(void)
{
	lcd_num_deal(location_num_2,SHOW,num_);	//��2������ ��ʾ ��-��
	/**************************************************/
	switch(rtc_alarm[flag_current_seting_alarm].a_day)
	{
		case day_1_1:
							lcd_num_deal(location_num_1,SHOW,num_1);	//��1������ ��ʾ ��1��
							lcd_num_deal(location_num_3,SHOW,num_1);	//��3������ ��ʾ ��1��
							break;
		case day_1_5:
							lcd_num_deal(location_num_1,SHOW,num_1);	//��1������ ��ʾ ��1��
							lcd_num_deal(location_num_3,SHOW,num_5);	//��3������ ��ʾ ��5��
							break;
		case day_1_7:
							lcd_num_deal(location_num_1,SHOW,num_1);	//��1������ ��ʾ ��1��
							lcd_num_deal(location_num_3,SHOW,num_7);	//��3������ ��ʾ ��7��
							break;
		case day_6_7:
							lcd_num_deal(location_num_1,SHOW,num_6);	//��1������ ��ʾ ��6��
							lcd_num_deal(location_num_3,SHOW,num_7);	//��3������ ��ʾ ��7��
							break;
        default:break;                    
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_clock
    @�������ܣ�   ��ʱ����
    @��ڲ�����   ��	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_clock(void)
{
	show_hour_minute(from_calendar);										//��ʾʱ��						
	lcd_symbol_deal(symbol_sec1,SHOW);			                            //�����˸
	lcd_symbol_deal(symbol_sec2,SHOW);			                            //�����˸
	/************************************************************/
	if(flag_alarm_timeout || flag_enter_snooze)
	{
		if(flag_current_alarmed == ALARM_1)
		{			
			lcd_symbol_deal(symbol_al1,flag_clockdot_show);	//--- ��Ӧ���ӷ�����˸ ---
		}
		else
		{			
			lcd_symbol_deal(symbol_al2,flag_clockdot_show);	
		}
	}
    else
    {
        lcd_symbol_deal(symbol_al1,flag_alarm1_en);					            //��1�����ӷ��Ŵ���
        lcd_symbol_deal(symbol_al2,flag_alarm2_en);					            //��2�����ӷ��Ŵ���
    }

    if (g_led_alarm_enable)
    {
        lcd_symbol_deal(symbol_alarm_out,(flag_clockdot_show && flag_alarm_timeout)); //flag_alarm_timeout == 1�������˸
    }
    else
    {
        lcd_symbol_deal(symbol_alarm_out,HIDE);
    }
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_set_clock
    @�������ܣ�   ʱ�����ý���
    @��ڲ�����   ��	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_set_clock(void)
{
	if(flag_set_flash_en && !flag_clockdot_show)						//����������λ��˸ �� ʱ����㲻��ʾ ʱ������λҲ����ʾ		
	{
		switch(set_location)
		{
			case set_clock_hour:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//���� ʱ 		
				break;
			case set_clock_minute: 
				lcd_num_deal(location_num_3,HIDE,0);	
				lcd_num_deal(location_num_4,HIDE,0);							//���� ��
				break;			
			case set_clock_year: 
			case set_clock_12_24:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);
				lcd_num_deal(location_num_3,HIDE,0);
				lcd_num_deal(location_num_4,HIDE,0);							//���� ��		
				break;
			case set_clock_month: 
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//���� ��	
				break;	
			case set_clock_day: 
				lcd_num_deal(location_num_3,HIDE,0);
				lcd_num_deal(location_num_4,HIDE,0);							//���� ��	
				break;							
		}	
	}
	else
	{
		if(set_location < set_clock_12_24)										//��������� ʱ��
		{
			lcd_symbol_deal(symbol_sec1,SHOW);
            lcd_symbol_deal(symbol_sec2,SHOW);
            show_hour_minute(from_calendar);										//��ʾ ʱ��
		}
		else if(set_location == set_clock_12_24)
		{
			show_12_24hours();
		}
		else if(set_location == set_clock_year)								//��������� ��
		{
			show_year();
		}	
		else																									//��������� ����
		{
			show_month_day();
		}	
	}
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_set_alarm
    @�������ܣ�   �������ӽ���
    @��ڲ�����   ��	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_set_alarm(void)
{	
	if(flag_set_flash_en && !flag_clockdot_show)						//����������λ��˸ �� ʱ����㲻��ʾ ʱ������λҲ����ʾ	
	{
		switch(set_location)
		{
			case set_alarm_hour:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//���� ʱ 	
				break;
			case set_alarm_minute:
				lcd_num_deal(location_num_3,HIDE,0);	
				lcd_num_deal(location_num_4,HIDE,0);							//���� ��
				break;
			case set_alarm_day:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//���� 
				lcd_num_deal(location_num_3,HIDE,0);	
				break;
//			case set_alarm_ringtime:
//				lcd_num_deal(location_num_2,HIDE,0);							//���� 
//				lcd_num_deal(location_num_3,HIDE,0);	
//				break;
            default:break;
		}		
	}
	else
	{
		if(set_location < set_alarm_day)											//---���������ʱ��
		{
			lcd_symbol_deal(symbol_sec1,SHOW);
            lcd_symbol_deal(symbol_sec2,SHOW);
//			if(flag_current_seting_alarm == ALARM_1)
//				show_hour_minute(from_alarm_1);										//��ʾ ʱ��
//			else
//				show_hour_minute(from_alarm_2);										//��ʾ ʱ��			
			show_hour_minute((uint8_t)flag_current_seting_alarm + 1);
		}
		else if(set_location == set_alarm_day)
		{
			show_alarm_day_style();
		}
//		else
//		{
//			show_alarm_ringtime();
//		}	
	}
	/******* ����ָʾ���Ų��� ����λ ��˸ ���Լ�������˸ *******/
	if(flag_current_seting_alarm == ALARM_1)
	{
        lcd_symbol_deal(symbol_al1,flag_clockdot_show);
    }
		
	else
	{
        lcd_symbol_deal(symbol_al2,flag_clockdot_show);
    }
		
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_set_snooze_time
    @�������ܣ�   ����̰˯ʱ������
    @��ڲ�����   ��	
    @���ڲ�����   ��
    @��ע˵����   �� 
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_set_snooze_time(void)
{
	u8 temp = 0;

	if(flag_set_flash_en && !flag_clockdot_show)						//����������λ��˸ �� ʱ����㲻��ʾ ʱ������λҲ����ʾ
	{
		lcd_num_deal(location_num_2,HIDE,0);
		lcd_num_deal(location_num_3,HIDE,0);		
	}
	else
	{
		temp = snooze_time / 10;
		lcd_num_deal(location_num_2,(bit)temp,temp);
		lcd_num_deal(location_num_3,SHOW,snooze_time % 10);		
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   lcd_test
    @�������ܣ�   lcd����ģʽ
    @��ڲ�����   ��	
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_test(void)
{
	if(test_step == 1 && test_key_num != 0)
	{
		lcd_num_deal(location_num_2,SHOW,num_H);
		lcd_num_deal(location_num_3,SHOW,test_key_num);	
	}
	else if(test_step == 2)
	{
		lcd_num_deal(location_num_1,SHOW,g_current_motor_level);
		lcd_symbol_deal(symbol_sec1,SHOW);
        lcd_symbol_deal(symbol_sec2,SHOW);
		lcd_num_deal(location_num_4,SHOW,g_current_alarm_mode_level);
	}


    if (test_step == 2)
    {
        if (flag_alarm_style_shake)                      //�𶯵Ĳ���
        {
            motor_start(shake_strength[alarm_shake_seted_strength],0);
        }

        if (!flag_alarm_style_shake)
        {
            motor_stop();
        }


        if (flag_alarm_style_ring)							//****����ʱ�䵽ʱ �����Ĳ���
		{
			alarm_tone();												//���ڵ���
		}  

        if (!flag_alarm_style_ring)
        {
            if (g_cmd != cmd_ring_stop)
            {
                ring_stop();
            }           
        }
    }  
}




uint16_t g_9v_power_on_wait_time = 500; //500ms




void lcd_clock_9v_power_on_wait(void)
{
    if (!g_9v_power_on_wait_time)
    {
#if (BL_CTRL_MOD == BL_PWM_CTRL)
        bl_brightness_set(bl_PWM_duty);    //�����5V�ϵ��򰴵�ǰ��ռ�ձȵ�������
#elif (BL_CTRL_MOD == BL_IO_CTRL)
        GPIO_Init(BL_CTL_PIN,OUTPUT);
        BL_CTL_GPIO = 1;
#endif
        key_ad_init();
        get_calendar();                    //��ȡ���� 
        lcd_mode = mode_clock;
    }
}   


void lcd_volume_adjust(void)
{
    lcd_num_deal(location_num_3,SHOW,g_alarm_ring_index);	
}


/*********************************************************************************************************************
��	�ܣ�
��	����
����ֵ��
*********************************************************************************************************************/
void lcd_display(void)
{
    if(flag_dc_5v)	
    {
        switch(lcd_mode)
		{
            case mode_9v_power_on_wait:     lcd_clock_9v_power_on_wait();break;
			case mode_clock: 				lcd_clock();break;
			case mode_set_alarm: 		    lcd_set_alarm();break;
			case mode_set_clock:		    lcd_set_clock();break;
			case mode_set_snooze_time:	    lcd_set_snooze_time();break;           
            case mode_test:					lcd_test();break;
			case mode_volum_adjust:			lcd_volume_adjust();break;											
            default:break;                                                    
		}
    }
}

