#ifndef _MAIN_C_
#define _MAIN_C_
/* Includes ------------------------------------------------------------------*/
#include "app.h"
#include "bsp.h"
#include "uart.h"
#include "delay.h"
/*-------------------------------------------------------------------------
*MCU:CA51F253L3
*FOSC:22118400hZ
*����ʱ��Ҫ����
*-------------------------------------------------------------------------
*/


void main(void)
{

    #if (SYSCLK_SRC == IRCH)
		Sys_Clk_Set_IRCH();
	#elif (SYSCLK_SRC == PLL)
        Sys_Clk_Set_PLL(PLL_Multiple);
    #endif
	CKCON |= ILCKE;	//ʹ���ڲ�����ʱ�ӣ����ڱ���pwm										//IRCLʱ��ʹ��

    GPIO_Init(P72F,P72_XOSCL_IN_SETTING);
	GPIO_Init(P71F,P71_XOSCL_OUT_SETTING);

	CKCON |= XLCKE;									//�ⲿ����ʱ��ʹ��
	while(!(CKCON & XLSTA));				//�ȴ�ʱ���ȶ�	

    
	#ifdef PRINT_ENABLE
	Uart0_Initial(115200);
	#endif
	
	timer_init();//��ʼ��������ʱ��
	rtc_init();

    //adc_user_init(); 
    
    lcd_init();
    bl_init();
    ring_init();
    
    switch_ADC_init();

    test_5v_init();
    bat_ADC_init();
    
    switch_led_mod_init();

    motor_init();
	
	no_use_io_init();
    
	EA = 1;
    		
	while (1)
	{
        if(!lowspeed)
        {
            if (flag_10ms)
            {
                flag_10ms = 0; 
                rtc_alarm_opera();
                motor_pro();
                test_5v_in(); 
                key_process();
                lcd_display();
                key_no_action_test();
                low_vol_opera();  //�͵���ʱ���ͼ����˸
            }


            if (lcd_mode != mode_9v_power_on_wait)
            {
                if (flag_20ms)
                {
        			flag_20ms = 0;
                    bl_handler();
                    ring_vol_adjust_handler();
                    g_timer0_20ms_cnt = TIMER0_20MS_CNT;
                }
    			
    			
                if(flag_50ms)
        		{
        			flag_50ms = 0;								
                    bat_vol_test();													//������ص�ѹ
                }

                if (flag_200ms)
                {
                    flag_200ms = 0;
                    get_volume_and_mode();									//�������𶯵ȼ� �� ����ģʽ����
                    led_mod_detect();
                    //uart_printf("lcd_mode = %d\n",lcd_mode); 
                    //uart_printf("flag_alarm_timeout = %d\n",flag_alarm_timeout); 
                }
    			
    			
            	if(flag_second)										//----- 1 ��
        		{
        			flag_second = 0;					
        			snooze_alarm_test();										//̰˯���Ӳ���
        			alarmed_time_test();										//��������ʱ�����
        			nothing_action_test();	
        		}

               
            	if(flag_minute)										//----- 1 ��
        		{
        			flag_minute = 0;			
        			get_calendar(); 												//��������ʱ��		
        			alarm_test();														//���Ӳ���
            	}

            }

        }

        else
        {
            test_5v_in_lowspeed();
        }
        
	}
}
#endif
