/* Includes ------------------------------------------------------------------*/
#include "interrupt.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* global variables ----------------------------------------------------------*/





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   timer0_isr
	@�������ܣ�   ��ʱ��0,1MS�ж�һ�Σ����ڻ�����ʱ
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void timer0_isr (void) interrupt 1 	
{
    static uint8_t cnt_10ms = 0;
    static uint8_t cnt_1ms = 0;
    static uint8_t cnt_20ms = 0;
    static uint8_t cnt_200ms = 0;
    static uint8_t cnt_2s = 0;
	/********* ����ģʽ *************/
	if(!lowspeed)
	{
		#if 	(SYSCLK_SRC == IRCH)
		TL0 = 0xcc;								
		TH0 = 0xfe;								// 1ms
		#elif	(SYSCLK_SRC == PLL)
		TL0 = 0xcc;								
		TH0 = 0xf8;								// 1ms
		#endif

        if (low_power_test_time)
		{
            low_power_test_time--;
        }

        if (g_9v_power_on_wait_time)
        {
            g_9v_power_on_wait_time--;
        }

        if(++cnt_1ms >= 10)
		{
			cnt_1ms = 0;
			//========================= 10ms =====
			cnt_20ms++;
            //========================= 20ms =====
            if (cnt_20ms > 2)
            {
                cnt_20ms = 0;
                flag_20ms = 1;	
            }
			flag_10ms = 1;	
			if(set_no_flash_gap > 0)
			{
				if(--set_no_flash_gap == 0)
					flag_set_flash_en = 1;					//����λ���¿�ʼ��˸
			}					
			
			//========================= 50ms =====
			if(++cnt_10ms >= 5)
			{
				cnt_10ms = 0;
				flag_50ms = 1;
				
				/***** ����	*****/				
				if(key_no_action_cnt)		key_no_action_cnt--;	
			}
            //========================= 200ms =====
            if (++cnt_200ms > 20)
            {
                cnt_200ms = 0;
                flag_200ms = 1;
            }

            
            if (++cnt_2s > 200)
            {
                cnt_2s = 0;
                flag_2s = 1;
            }

            if (g_timer0_2s_cnt)
            {
                g_timer0_2s_cnt--;
            }

            if (ring_gap_time)
            {
                ring_gap_time--;
            }
            
		}	

        /*
		if (g_timer0_5ms_cnt)
		{
			g_timer0_5ms_cnt--;
		}
        if (g_timer0_10ms_cnt)
        {
            g_timer0_10ms_cnt--;
        }		
        if (g_timer0_20ms_cnt)
        {
            g_timer0_20ms_cnt--;
        }
        
        if (g_timer0_200ms_cnt)
        {
            g_timer0_200ms_cnt--;
        }
        if (g_timer0_2s_cnt)
        {
            g_timer0_2s_cnt--;
        }
        */
	}
	/********* ����ģʽ *************/
	else						//����ģʽ�� 141KHz 
	{
		TL0 = 0xbc;								
		TH0 = 0xfb;		//100ms	

		if (low_power_test_time)
		{
            low_power_test_time--;
        }
	}
}






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   timer1_isr
	@�������ܣ�   ��ʱ��1�жϴ�������IC˫��ͨ��	
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void timer1_isr(void) interrupt 3			
{
	//if (!busy)
	//{
		ring_cmd_isr_handler();
	//}
		
	
   
}








/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   rtc_isr
	@�������ܣ�   rtc�жϴ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void rtc_isr (void) interrupt 13 	 
{
	static uint8_t test_flag = 0;
	//==================================================================
	if(RTCIF & RTC_HF)			//�����ж�
	{
		RTCIF = RTC_HF;	
//		flag_half_second = 1;	
		//ʱ�������˸
		flag_clockdot_show = !flag_clockdot_show;	
		
		//����� 
		if(flag_clockdot_show)
		{			
			flag_second = 1;
            
			if(time_to_enter_lowspeed) time_to_enter_lowspeed--;
		}
		if(RTCS == 0)			
		{
			test_flag = !test_flag;
			//�ָ���
			if(test_flag)
			{
				flag_minute = 1;
			}
		}		
	}	
}












