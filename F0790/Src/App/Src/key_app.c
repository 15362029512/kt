#include "key_app.h"


/* Private variables ---------------------------------------------------------*/
static uint8_t  test_key2_cnt = 0;	

/* global variables ----------------------------------------------------------*/
uint8_t key_no_action_start = 0;
uint8_t key_no_action_cnt = 0;

uint8_t  test_step = 0;								//����ʱ�Ĳ���   0����ʾ����汾��ȫ��    1��ȫ������԰��� �� ���ȵ��
uint8_t  test_key_num = 0;							//���԰���ʱ������ֵ



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_process
    @�������ܣ�   ÿ�������Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void key_process(void)
{
    if (lcd_mode != mode_9v_power_on_wait)
    {
        bsp_KeyScan10ms();
    	ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
        switch (ucKeyCode)
        {
            case KEY_12_24H_SET_SHORT_UP:
                key_12_24h_short_up_handler();
                break;
            case KEY_12_24H_SET_LONG_FIRST:
                key_12_24h_long_press_handler();
                break;
                         
            case KEY_UP_SHORT_PRESS:
            case KEY_UP_LONG_FIRST:
            case KEY_UP_LONG_REPEAT:    
                key_up_handler();
                break;

            case KEY_DOWN_SHORT_PRESS:
            case KEY_DOWN_LONG_FIRST:
            case KEY_DOWN_LONG_REPEAT:
                key_down_handler();
                break;
            
            case KEY_ALARM1_SHORT_UP:
                key_al1_short_up_handler();
                break;
            case KEY_ALARM1_LONG_FIRST:
                key_al1_long_press_handler();
                break;
            
            case KEY_ALARM2_SHORT_UP:
                key_al2_short_up_handler();
                break;
            case KEY_ALARM2_LONG_FIRST:
                key_al2_long_press_handler();
                break;

            case KEY_SNOOZE_SHORT_UP:
                key_snooze_short_up_handler();
                break;
            case KEY_SNOOZE_LONG_FIRST:
                key_snooze_long_handler();
                break;
            
            case KEY_UP_DOWM_DOUBLE_LONG_FIRST:
	            key_up_down_double_handler();
                break;

            case KEY_ALARM_SET_SHORT_PRESS:             //�̰�ȡ�������̰˯
                key_alarm_set_short_press_handler();
                break;   
            case KEY_ALARM_SET_SHORT_UP:                //��������ģʽ�·���������
                key_alarm_set_short_up_handler();
                break;
            case KEY_ALARM_SET_LONG_FIRST:              //����������������ģʽ
                key_alarm_set_long_press_handler();
                break;
            
            default:break;
        }
    }
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_no_action_test
    @�������ܣ�   �����޲����Ƕ�Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void key_no_action_test(void)
{
	if(key_no_action_start)
	{
		if(!key_no_action_cnt) 																			//�����޶���ʱ�䵽
		{			
			key_no_action_start = 0;
			#ifdef SHOW_ALARM_TIME_EN
			if(lcd_mode == mode_set_clock || lcd_mode == mode_set_snooze_time || lcd_mode == mode_show_alarm_time)
			#else
			if(lcd_mode == mode_set_clock || lcd_mode == mode_set_snooze_time)
			#endif
			{
				lcd_mode = mode_clock;
			}	
			else if(lcd_mode == mode_set_alarm)
			{				
				/**** ���������õ��������� *****/
				//rtc_alarm[flag_current_seting_alarm].a_ringtime = rtc_alarm[ALARM_TEMP].a_ringtime;
				rtc_alarm[flag_current_seting_alarm].a_day 			= rtc_alarm[ALARM_TEMP].a_day;
				rtc_alarm[flag_current_seting_alarm].a_hour 		= rtc_alarm[ALARM_TEMP].a_hour;
				rtc_alarm[flag_current_seting_alarm].a_minute 	= rtc_alarm[ALARM_TEMP].a_minute;
				rtc_alarm[flag_current_seting_alarm].a_second 	= rtc_alarm[ALARM_TEMP].a_second;
				lcd_mode = mode_clock;
			}
			else if(lcd_mode == mode_test)
			{
				test_step = 1;
				test_key2_cnt = 0;
				test_key_num = 0;
				lcd_show_all();
			}
		}
	}		
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   test_time_set_step
    @�������ܣ�   ����ʱ��״̬�£��������õ�location �����գ�ʱ��
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ����ʱ��ģʽ�µ���
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_time_set_step(void)
{
	key_no_action_start = 0;
	lcd_clear();
	if(set_location == set_clock_day)								//���ա�������ɺ�����һ�ΰ���
	{									//��������ʱ��		
		lcd_mode = mode_clock;												//ֱ�ӽ��� ʱ��ģʽ
		return;
	}
	else 
	{		
		set_location ++;															//˳���л��� ��������		
	}			
	
	key_no_action_cnt = SETING_FLASH_TIMER;
	key_no_action_start = 1;												//�����޶�����ʱ��ʼ
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_12_24h_short_up_handler
    @�������ܣ�   �̰�key_12_24h�Ĺ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   mode_clockģʽ�£��л�12h24h��mode_set_clock�������õ�location
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_12_24h_short_up_handler(void)
{
    if (lcd_mode == mode_test)
    {
    	if (test_step == 1)
    	{
    		test_key2_cnt = 0;
    		lcd_clear();
    		test_key_num = 2;								
    	}
    }
    else if (lcd_mode == mode_clock)												//����� ʱ��ģʽ 
    {
        flag_24hours_en = !flag_24hours_en;                             //�л�24h��12h�ƶ�
    	alarm_stop_key_function(KEY_12_24H_SET_SHORT_PRESS);
    }
    else if (lcd_mode == mode_set_clock)								//����� ʱ������ģʽ
    {
    	test_time_set_step();							
    }
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_12_24h_long_press_handler
    @�������ܣ�   ����key_12_24h�Ĺ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   mode_clockģʽ�£���������mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_12_24h_long_press_handler(void)
{
    if(lcd_mode == mode_clock)												//�� ʱ��ģʽ �³�����
	{
		lcd_clear();
		lcd_mode = mode_set_clock;											//ת���� ʱ������ ģʽ
		flag_set_flash_en = 1;													//����λ������˸
		set_location = set_clock_hour;									//������ ʱ
		key_no_action_cnt = SETING_FLASH_TIMER;
		key_no_action_start = 1;												//�����޶�����ʱ��ʼ								
	}
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_up_handler
    @�������ܣ�   key_up�Ĺ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   mode_clockģʽ�£���������mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_up_handler(void)
{
    
	if(lcd_mode == mode_test)
	{
        if(test_step == 1)
        {
            test_key2_cnt = 0;
            lcd_clear();
			test_key_num = 5;
        }
	}
	else if(lcd_mode == mode_clock)												//����� ʱ��ģʽ
	{
		alarm_stop_key_function(KEY_UP_SHORT_PRESS);
	}
	else if(lcd_mode == mode_volum_adjust)
	{
        if (g_alarm_ring_index <= 3)
        {
             g_alarm_ring_index++;
        }
        if (g_alarm_ring_index > 3)
        {
            g_alarm_ring_index = 0;
        }
	}
   

	else	
	{
		#ifdef SHOW_ALARM_TIME_EN
		if(lcd_mode != mode_show_alarm_time)
		#endif
		{
			key_no_action_start = 0;	
			flag_set_flash_en = 0;												//����ʱ����������˸		
			set_no_flash_gap = SET_KEY_FLASH_GAP;					//��������˸��ʱ���� 600ms					
			switch(lcd_mode)
			{
				case mode_set_clock:		set_clock_add();break;
				case mode_set_alarm:		set_alarm_add();break;
				case mode_set_snooze_time:		
																if(++snooze_time > SNOOZE_TIME_MAX)
																	snooze_time = SNOOZE_TIME_MIN;
																break;
			}
			
			key_no_action_cnt = SETING_FLASH_TIMER;
			key_no_action_start = 1;											//�����޶�����ʱ��ʼ									
		}							
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_down_handler
    @�������ܣ�   key_dowm�Ĺ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   mode_clockģʽ�£���������mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_down_handler(void)
{
	if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 3;								
		}
	}
	else if(lcd_mode == mode_clock)												//����� ʱ��ģʽ
	{
		alarm_stop_key_function(KEY_DOWN_SHORT_PRESS);
	}

    else if(lcd_mode == mode_volum_adjust)
	{
        if (g_alarm_ring_index > 0)
        {
            g_alarm_ring_index--;
        }
        else
        {
            g_alarm_ring_index = 3;
        }
	}
    
	else
	{
		#ifdef SHOW_ALARM_TIME_EN
		if(lcd_mode != mode_show_alarm_time)
		#endif
		{
			key_no_action_start = 0;
			flag_set_flash_en = 0;												//����ʱ����������˸		
			set_no_flash_gap = SET_KEY_FLASH_GAP;					//��������˸��ʱ���� 600ms					
			switch(lcd_mode)
			{
				case mode_set_clock:		set_clock_sub();break;
				case mode_set_alarm:		set_alarm_sub();break;
				case mode_set_snooze_time:		
																if(--snooze_time < SNOOZE_TIME_MIN)
																	snooze_time = SNOOZE_TIME_MAX;
																break;								
			}				
			
			key_no_action_cnt = SETING_FLASH_TIMER;
			key_no_action_start = 1;													//�����޶�����ʱ��ʼ								
		}
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   test_alarm_set_step
    @�������ܣ�   ��������ģʽ�µ�����ʾ��location,�����õ�ǰ���ӵĲ���
    @��ڲ�����   _set_alarm��ǰ�������õ�����
    @���ڲ�����   ��
    @��ע˵����   mode_clockģʽ�£���������mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_alarm_set_step(uint8_t _set_alarm)
{
	if(flag_current_seting_alarm == _set_alarm)
	{
		key_no_action_start = 0;
		lcd_clear();	
		//if(set_location == set_alarm_ringtime)					//����ʱ��������ɺ�����һ�ΰ���
		if(set_location == set_alarm_day)					//����ʱ��������ɺ�����һ�ΰ���
		{
			lcd_mode = mode_clock;
			if(flag_current_seting_alarm == ALARM_1) 
				flag_alarm1_en = 1;
			else 
				flag_alarm2_en = 1;
		}																		
		else 
		{
			set_location ++;															//�л����¸�����λ
		}												
		key_no_action_cnt = SETING_FLASH_TIMER;	
		key_no_action_start = 1;												//�����޶�����ʱ��ʼ	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   enter_alarm_seting
    @�������ܣ�   ������������ģʽ
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   mode_clockģʽ�£���������mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void enter_alarm_seting(uint8_t _set_alarm)
{
	key_no_action_start = 0;
	/********* ����ԭ�����ӵ����ݵ��м���� *********/
	flag_current_seting_alarm = _set_alarm;
	//rtc_alarm[ALARM_TEMP].a_ringtime 	= rtc_alarm[flag_current_seting_alarm].a_ringtime;
	rtc_alarm[ALARM_TEMP].a_day 			= rtc_alarm[flag_current_seting_alarm].a_day;
	rtc_alarm[ALARM_TEMP].a_hour 			= rtc_alarm[flag_current_seting_alarm].a_hour;
	rtc_alarm[ALARM_TEMP].a_minute 		= rtc_alarm[flag_current_seting_alarm].a_minute;
	rtc_alarm[ALARM_TEMP].a_second 		= rtc_alarm[flag_current_seting_alarm].a_second;		
	/**************************************************/
	lcd_clear();	
	
	lcd_mode = mode_set_alarm;										 	//ת��ģʽ�� ��������ģʽ
//	lcd_symbol_deal(symbol_sec,SHOW);				
	flag_set_flash_en = 1;													//����Ϊ������˸
	set_location = set_alarm_hour;									//������Сʱ
	key_no_action_cnt = SETING_FLASH_TIMER;
	key_no_action_start = 1;												//�����޶�����ʱ��ʼ		
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   alarm_stop_key_function
    @�������ܣ�   ���Ӳ���ʱ�����������ֹͣ���ӣ������ǰ��̰˯ģʽ����ر�̰˯,����δ����ʱ����KEY_ALARM1 KEY_ALARM2�ر�����
    @��ڲ�����   _key:��ǰ���µ����ĸ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void alarm_stop_key_function(uint8_t _key)
{
	if(flag_alarm_timeout)
	{
        flag_alarm_timeout = !flag_alarm_timeout;
    }
		
	else if(flag_enter_snooze)
	{
        flag_enter_snooze = !flag_enter_snooze;
    }		
	else	
	{
		switch(_key)
		{		
			case KEY_ALARM1_SHORT_PRESS:	
							#ifdef SHOW_ALARM_TIME_EN	//---------��ʾ����ʱ��
							if(!flag_alarm1_en)													// ����û�п���
							{
								key_no_action_start = 0;
								flag_alarm1_en = 1;												//�� ��������
								lcd_mode = mode_show_alarm_time;					//ת�� ��ʾ����ʱ�� ģʽ
								flag_show_which_alarm = ALARM_1;					//ͬʱ����Ҫ��ʾ���� ����1
								key_no_action_cnt = SHOW_ALARM_TIMER;	
								key_no_action_start = 1;									//�����޶�����ʱ��ʼ									
							}
							else																				//���� �Ѿ�����
							{
								flag_alarm1_en = 0;												//�� �ر�����
							}
							#else											//---------ֻ�������ӵĿ���
							flag_alarm1_en = !flag_alarm1_en;						//�л�����1�Ŀ���״̬
							#endif
							break;				
			case KEY_ALARM2_SHORT_PRESS:	
							#ifdef SHOW_ALARM_TIME_EN					
							if(!flag_alarm2_en)
							{
								key_no_action_start = 0;
								flag_alarm2_en = 1;
								lcd_mode = mode_show_alarm_time;
								flag_show_which_alarm = ALARM_2;
								key_no_action_cnt = SHOW_ALARM_TIMER;	
								key_no_action_start = 1;					
							}
							else
							{
								flag_alarm2_en = 0;
							}
							#else													
							flag_alarm2_en = !flag_alarm2_en;
							#endif
							break;
            default:break;                			
		}	
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_al1_short_up_handler
    @�������ܣ�   key_al1�̰���Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al1_short_up_handler(void)
{
	if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 7;								
		}
	}
	else if(lcd_mode == mode_clock)												//����� ʱ��ģʽ
	{
		alarm_stop_key_function(KEY_ALARM1_SHORT_PRESS);
	}							
	else if(lcd_mode == mode_set_alarm)								//����� ��������ģʽ
	{
		test_alarm_set_step(ALARM_1);
	}
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_al1_long_press_handler
    @�������ܣ�   key_al1������Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al1_long_press_handler(void)
{
    enter_alarm_seting(ALARM_1);

}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_al2_short_up_handler
    @�������ܣ�   key_al2�̰���Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al2_short_up_handler(void)
{
	if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 6;								
		}
	}
	else if(lcd_mode == mode_clock)												//����� ʱ��ģʽ
	{
		alarm_stop_key_function(KEY_ALARM2_SHORT_PRESS);
	}
	else if(lcd_mode == mode_set_alarm)								//����� ��������ģʽ
	{
		test_alarm_set_step(ALARM_2);
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_al2_long_press_handler
    @�������ܣ�   key_al2������Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al2_long_press_handler(void)
{
    enter_alarm_seting(ALARM_2);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_snooze_short_up_handler
    @�������ܣ�   key_snooze�̰���Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_snooze_short_up_handler(void)
{
	if(lcd_mode == mode_test)
	{
        switch (test_step)
        {
            case 1:
                lcd_clear();
                test_key_num = 4;
                test_key2_cnt++;
                if(test_key2_cnt == 2)
                {
                    test_key2_cnt = 0;
                    test_step = 2;
                    lcd_clear();
                }	
                break;

            case 2:
                test_step = 0;			
                motor_stop();
                ring_stop();
                lcd_mode = mode_clock;
                break;
                
            default:break;    
                
            
            
        }
        #if 0
		if(test_step == 1)
		{
            lcd_clear();
            test_key_num = 4;
			test_key2_cnt++;
			if(test_key2_cnt == 2)
			{
                test_step = 2;
                lcd_clear();
            }											
		}

        
        else if (test_step == 2)
        {
            test_step = 0;			
			motor_stop();
			ring_stop();
            lcd_mode = mode_clock;
            /*
            if (g_cmd == cmd_ring_stop)
            {
                lcd_mode = mode_clock;
            }
            */
        }
        #endif
	}
	else if(lcd_mode == mode_clock)
	{
		if(flag_alarm_timeout)
		{
			enter_snooze();																	//����̰˯
		}							
	}
	else if(lcd_mode == mode_set_snooze_time)				//����̰˯ʱ��ʱ�ٰ�̰˯��
	{
		lcd_mode = mode_clock;												//����ʱ��ģʽ							
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_snooze_long_handler
    @�������ܣ�   key_snooze������Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_snooze_long_handler(void)
{
	if(lcd_mode == mode_clock)
	{
		key_no_action_start = 0;
		lcd_clear();
		flag_set_flash_en = 1;													//����λ������˸
		lcd_mode = mode_set_snooze_time;								//����̰˯ʱ������ģʽ
		key_no_action_cnt = SETING_FLASH_TIMER;
		key_no_action_start = 1;												//�����޶�����ʱ��ʼ								
	}	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_alarm_set_short_up_handler
    @�������ܣ�   key_alarm_set�̰���Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_alarm_set_short_up_handler(void)
{
    alarm_stop_key_function(KEY_ALARM_SET_SHORT_UP); //�̰�ȡ�������̰˯ģʽ
    if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 1;								
		}
	}
    else if (lcd_mode == mode_volum_adjust)
    {
        lcd_clear();
        lcd_mode = mode_clock;
    }
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_alarm_set_long_press_handler
    @�������ܣ�   key_alarm_set������Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ����������������ģʽ
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_alarm_set_long_press_handler(void)
{
    alarm_stop_key_function(KEY_ALARM_SET_LONG_FIRST); //ȡ�������̰˯ģʽ
    if (lcd_mode == mode_clock)
    {
        lcd_clear();
        lcd_mode = mode_volum_adjust;          //������������ģʽ
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_alarm_set_short_press_handler
    @�������ܣ�   key_alarm_set�̰���Ӧ�Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ����������������ģʽ
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_alarm_set_short_press_handler(void)
{
    alarm_stop_key_function(KEY_ALARM_SET_LONG_FIRST); //ȡ�������̰˯ģʽ
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_up_down_double_handler
    @�������ܣ�   ˫���������ģʽ
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

static void key_up_down_double_handler(void)
{
    if(lcd_mode != mode_test)
    {
    	lcd_mode = mode_test;
    	test_step = 0;
    	lcd_clear();
    	/*** ��ʾ�汾  4.0r***/
    	lcd_num_deal(location_num_2,SHOW,num_1);
    	lcd_symbol_deal(symbol_snooze,SHOW);
    	lcd_num_deal(location_num_3,SHOW,num_0);
    	lcd_num_deal(location_num_4,SHOW,num_r);				
    	key_no_action_cnt = KEY_NO_ACTION_2s;
    	key_no_action_start = 1;												//�����޶�����ʱ��ʼ							
    }

}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_add����ʱ��
    @�������ܣ�   ÿ�������Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_clock_add(void)
{
	if(set_location == set_clock_hour)
	{
		if(++calendar.st_hour == 24)	calendar.st_hour = 0;
	}					
	else if(set_location == set_clock_minute)
	{
		if(++calendar.st_minute == 60)	calendar.st_minute = 0;
	}
	else if(set_location == set_clock_12_24)
	{
		flag_24hours_en = !flag_24hours_en;					//�л�Сʱ�ĸ�ʽ	
	}	
	else if(set_location == set_clock_year)
	{
		if(++calendar.st_year == 2100)	calendar.st_year = 2099;
	}
	else if(set_location == set_clock_month)
	{
		/* ���ж� ��ǰ������ �Ƿ� ��������һ��Ҫ�����µ� �������ֵ�� ������ ��ǰ���ú��������ֵ*/
		if(is_over_max_days(calendar.st_year,((calendar.st_month + 1 == 13) ? 1 : (calendar.st_month + 1)),calendar.st_day))
		{
			set_max_days_of_month(calendar.st_year,((calendar.st_month + 1 == 13) ? 1 : (calendar.st_month + 1)));
		}		
		if(++calendar.st_month == 13)	calendar.st_month = 1;					
	}
	else if(set_location == set_clock_day)
	{
		calendar.st_day++;
		/*** �����·ݵ�������� ***/
		if( ((calendar.st_month != 2) && (calendar.st_day > day_of_month[calendar.st_month - 1])) ||
				((calendar.st_month == 2) && (calendar.st_day == 29 + (u8)is_leap_year(calendar.st_year))) )
		{
			calendar.st_day = 1;
//			if(++calendar.st_month == 13)	calendar.st_month = 1;	
		}							
	}
	calendar.st_second = 0;							//ÿ�θ� �� ����	
	updata_calendar();
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_down����ʱ��
    @�������ܣ�   ÿ�������Ĺ���
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_clock_sub(void)
{
	if(set_location == set_clock_hour)
	{
		if(--calendar.st_hour == 255)	calendar.st_hour = 23;
	}					
	else if(set_location == set_clock_minute)
	{
		if(--calendar.st_minute == 255)	calendar.st_minute = 59;		
	}	
	else if(set_location == set_clock_12_24)
	{
		flag_24hours_en = !flag_24hours_en;					//�л�Сʱ�ĸ�ʽ			
	}	
	else if(set_location == set_clock_year)
	{
		if(--calendar.st_year == 1969)	calendar.st_year = 1970;
	}
	else if(set_location == set_clock_month)
	{
		if(is_over_max_days(calendar.st_year,((calendar.st_month - 1 == 0) ? 12 : (calendar.st_month - 1)),calendar.st_day))
		{
			set_max_days_of_month(calendar.st_year,((calendar.st_month - 1 == 0) ? 12 : (calendar.st_month - 1)));
		}			
		if(--calendar.st_month == 0)	calendar.st_month = 12;					
	}
	else if(set_location == set_clock_day)
	{
		if(--calendar.st_day == 0)
		{
//			if(--calendar.st_month == 0) calendar.st_month = 12;		
			if(calendar.st_month != 2)
			{
				calendar.st_day = day_of_month[calendar.st_month - 1];
			}
			else
			{
				calendar.st_day = 28 + (u8)is_leap_year(calendar.st_year);
			}		
		}				
	}
	calendar.st_second = 0;							//ÿ�θı� �� ����
	updata_calendar();	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_up��������
    @�������ܣ�   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_alarm_add(void)
{
	if(set_location == set_alarm_hour)
	{
		if(++rtc_alarm[flag_current_seting_alarm].a_hour == 24)
			rtc_alarm[flag_current_seting_alarm].a_hour = 0;
	}					
	else if(set_location == set_alarm_minute)
	{
		if(++rtc_alarm[flag_current_seting_alarm].a_minute == 60)
			rtc_alarm[flag_current_seting_alarm].a_minute = 0;
	}
	else if(set_location == set_alarm_day)
	{
		if(++rtc_alarm[flag_current_seting_alarm].a_day == 4)
			rtc_alarm[flag_current_seting_alarm].a_day = 0;			
	}	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   key_down��������
    @�������ܣ�   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_alarm_sub(void)
{
	if(set_location == set_alarm_hour)
	{
		if(--rtc_alarm[flag_current_seting_alarm].a_hour == 255)
			rtc_alarm[flag_current_seting_alarm].a_hour = 23;
	}					
	else if(set_location == set_alarm_minute)
	{
		if(--rtc_alarm[flag_current_seting_alarm].a_minute == 255)
			rtc_alarm[flag_current_seting_alarm].a_minute = 59;
	}
	else if(set_location == set_alarm_day)
	{
		if(--rtc_alarm[flag_current_seting_alarm].a_day == 255)
			rtc_alarm[flag_current_seting_alarm].a_day = 3;	
	}	
}



