/* Includes ------------------------------------------------------------------*/
#include "bat.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* global variables ----------------------------------------------------------*/


uint8_t lowspeed = 0;                       //�͹������б�־
uint8_t time_to_enter_lowspeed = 0;			//�������ģʽ��ʱ�䣬Ϊ 0 ʱ��������ģʽ

uint8_t flag_dc_5v = 1;									//1��5v��Դ����




uint16_t low_power_test_time = 5000; //5s


static uint8_t dc_5v_timer = 0;

static uint8_t flag_low_vol = 0;								//��ص͵��־

static uint8_t low_vol_time_cnt = 0;
static uint8_t test_ok_first_time = 0;      //��ֹ�ظ��������� �� ��


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   test_5v_init
    @�������ܣ�   5v����������ų�ʼ��
    @��ڲ�����   ����Ϊ����ģʽ
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void test_5v_init(void)
{   
    GPIO_Init(DC_5V_PIN,   INPUT);

    #if 0
    GPIO_Init(DC_5V_PIN,					P42_ADC2_SETTING);
    ADCON = 0x3d;
    ADCFGL |= 0xc0;	 
    #endif
}


void test_5v_config_io_in(void)
{

}


void test_5v_config_ADC_in(void)
{
    
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bat_ADC_init
    @�������ܣ�   ����������ų�ʼ��
    @��ڲ�����   ����ΪADģʽ
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bat_ADC_init(void)
{
    GPIO_Init(BAT_ADC_PIN,				P41_ADC1_SETTING);		//ch1

    ADCON = 0x3d;
    ADCFGL |= 0xc0;	 
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   close_bat_adc
    @�������ܣ�   �͹���ģʽ�¹رյ������AD������Ϊ����
    @��ڲ�����   ����ΪADģʽ
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void close_bat_adc(void)
{
    GPIO_Init(BAT_ADC_PIN,				HIGH_Z);		//ch1
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   test_5v_in
    @�������ܣ�   ����5V�Ƿ����
    @��ڲ�����   ����Ϊ�ߵ�ƽ
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void test_5v_in(void)
{
	static uint8_t test_ng_first_time = 0;		//��ֹ�ظ����ý���͹���ʱ��
	static u16 usb_vol = 0;    

    if (lcd_mode != mode_9v_power_on_wait)
    {
        
        //usb_vol = GetChannelAdcValue(V_ADC_CH);

        if (DC_5V_GPIO) 
        //if(usb_vol >= 2500)	
    	{
    		if (++dc_5v_timer >= TEST_DC_5V_TIME) 
    		{
    			dc_5v_timer = TEST_DC_5V_TIME;
    			flag_dc_5v = 1;										//5vDC��Դ����			
    			test_ng_first_time = 0;
    			if(!test_ok_first_time)						//��һ�μ�鵽 ,˵����5V�ϵ�
    			{
    				test_ok_first_time = 1;

                    lcd_mode = mode_9v_power_on_wait;       //�����ϵ�ȴ�ģʽ
                    g_9v_power_on_wait_time = 500;          //�ȴ�500ms
    			}
    		}		
    	}	
    	/**************** ********************/
    	else
    	{	
    		dc_5v_timer = 0;
    		flag_dc_5v = 0;
    		test_ok_first_time = 0;	
    		bl_close();//�ر���
    		
    		if(!test_ng_first_time)
    		{
    			test_ng_first_time = 1;
                enter_low_speed_mode();
                return;
    		}		
    	}



    }


}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bat_vol_test
    @�������ܣ�   ��ص������
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bat_vol_test(void)
{
	if(GetChannelAdcValue(BAT_ADC_CH) <=  BAT_LOW_VOL)
	{
		if(++ low_vol_time_cnt >= 20)					//��ʱ�ж��Ƿ������ǵ͵�
		{
			low_vol_time_cnt = 20;
			flag_low_vol = 1;
		}
	}
	else
	{
		low_vol_time_cnt = 0;
		flag_low_vol = 0;
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   low_vol_opera
    @�������ܣ�   �͵�ʱ�Ĳ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   �͵�ʱ��˸���ͼ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void low_vol_opera(void)
{
	if(flag_low_vol)					//�͵�ѹʱ
	{
        lcd_symbol_deal(symbol_bat,flag_clockdot_show);		//�͵�ѹ���Ÿ���ʱ�������˸	
	}
	else											//�� �͵�ѹʱ
	{
		lcd_symbol_deal(symbol_bat,HIDE);										//���ص͵�ѹ����
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   test_5v_in_lowspeed
    @�������ܣ�   �͹���ʱ����5v�Ƿ���룬������룬����
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   �͵�ʱ��˸���ͼ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void test_5v_in_lowspeed(void)
{
    #ifdef TIME_TEST_LOWPOWER_WAKEUP
    if (!low_power_test_time)
    {
        quit_low_speed_mode();//�˳��͹���
        low_power_test_time = 5000;
        bl_pwm_mode = 0;
    }
    #endif

    #ifndef TIME_TEST_LOWPOWER_WAKEUP
	if(dc_5v_in)
	{
		quit_low_speed_mode();//�˳��͹���
		time_to_enter_lowspeed = ENTER_LOWSPEED_TIME;			//����͹��ĵ���ʱ 10s
		bl_pwm_mode = 0;
	}
	else
	{
		flag_dc_5v = 0;
	}
   #endif

}
/*********************************************************************************************************************


*********************************************************************************************************************/
void nothing_action_test(void)
{
    #ifdef TIME_TEST_LOWPOWER_WAKEUP
    if (!low_power_test_time)
    {
        enter_low_speed_mode();
    }
    #endif

    #ifndef TIME_TEST_LOWPOWER_WAKEUP
	if(!flag_dc_5v)					//ֻ�� ��ع��� ����»� ����͹���
	{
		if(time_to_enter_lowspeed == 0)
		{
			enter_low_speed_mode();
		}
	}
   #endif


}


void ring_amp_close(void)
{
    SCL_LOW;
	SDA_LOW;
    DAC_R_HIGH;
    V_AMP_CTL_PIN_LOW;
}




/*********************************************************************************************************************


*********************************************************************************************************************/
#if 0
void enter_low_speed_mode(void)
{

	GPIO_Init(MOTOR_AIN_PIN,HIGH_Z);        //������Źر�
	GPIO_Init(MOTOR_BIN_PIN,HIGH_Z);
    	
	lcd_clear();

    bl_close();//�ر���

    ring_close(); //������
    //ring_amp_close();
    close_switch_ADC_pin(); //���뿪������ΪIO������

    close_bat_adc();        //�ص�ص�ѹ�������

    close_key_ad();

    //GPIO_Init(DC_5V_PIN,					INPUT);

    TR0 = 0;
    ET0 = 0;

    TR1 = 0;
    ET1 = 0;
    
		
	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))
	PLLCON &= 0x7f;												//PLL�رգ�����ʹ��IRCHʱ��
#endif		
//--------����ģʽ--------------

	I2CCON = 0; 
    CKCON |= ILCKE;
    Delay_ms(1);
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCL;		//ϵͳʱ��ѡ�� �ڲ�����ʱ�� 131KHz		
	CKCON = 0;								//�ر�û��ѡ��ʹ�õ���������ʱ�� ( )
	PWCON &= 0xf7;							//����LDO����͹���ģʽ
    

//------------------------------
	lowspeed = 1;
test_ok_first_time = 0;

low_power_test_time = 50;

}
#endif





/*********************************************************************************************************************


*********************************************************************************************************************/
#if 0
void quit_low_speed_mode(void)
{	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))	
	PLLCON |= 0xa0;													//PLL��   6��Ƶ 3686400 * 6 = 22118400hZ
	while(!(PLLCON & 0x01));								//�ȴ�PLL�ȶ�
#endif	
	
	CKCON |= IHCKE;
	CKCON |= ILCKE;	//ʹ���ڲ�����ʱ�ӣ����ڱ���pwm										//IRCLʱ��ʹ��
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCH;			//�ڲ� IRCH 4M ʱ��
	
	#if (SYSCLK_SRC == PLL)
	CKSEL = (CKSEL & 0XF8) | 4;							//ϵͳʱ��ѡ�� PLL
	ADCFGL |= 0x60;													//ADCʱ�� 6 ��Ƶ
	#endif	

	PWCON |= 0x08;													//LDO���ûظ߹���ģʽ
	//------------------	

    TR0 = 1;
    ET0 = 1;
   
    //������ڹ��ָܻ�    
    GPIO_Init(BL_ADC_DETECT_PIN, BL_ADC_DETECT_MOD);
    //5V���
    //GPIO_Init(DC_5V_PIN,P42_ADC2_SETTING);
    //�������ָܻ�

    //SCL_LOW;
	//SDA_LOW;
    
    SCL_HIGH;
	SDA_HIGH;
    
    GPIO_Init(DAC_R_PIN,OUTPUT);
    DAC_R_LOW;
    V_AMP_CTL_PIN_HIGH;
    GPIO_Init(RING_VOL_CTL_PIN, RING_VOL_CTL_MOD);     
    GPIO_Init(BUSY_PIN,INPUT);

    //���뿪�ع��ָܻ�
    GPIO_Init(MOTOR_SWITCH_ADC_PIN,			MOTOR_SWITCH_PIN_ADC_MODE);
    GPIO_Init(MODE_SWITCH_ADC_PIN,			MODE_SWITCH_PIN_ADC_MODE);

    //��ص�����⹦�ָܻ�
    GPIO_Init(BAT_ADC_PIN,				P41_ADC1_SETTING);	
		
	lowspeed = 0;//
}
#endif



void enter_low_speed_mode(void)
{

	GPIO_Init(MOTOR_AIN_PIN,HIGH_Z);        //������Źر�
	GPIO_Init(MOTOR_BIN_PIN,HIGH_Z);
    	
	lcd_clear();

    bl_close();//�ر���

    ring_close(); //������
    close_switch_ADC_pin(); //���뿪������ΪIO������

    close_bat_adc();        //�ص�ص�ѹ�������

    close_key_ad();

    TR0 = 0;
    ET0 = 0;

    TR1 = 0;
    ET1 = 0;
    
		
	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))
	PLLCON &= 0x7f;												//PLL�رգ�����ʹ��IRCHʱ��
#endif		
//--------����ģʽ--------------
	I2CCON = 0; 
    CKCON |= ILCKE;
    Delay_ms(1);
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCL;		//ϵͳʱ��ѡ�� �ڲ�����ʱ�� 131KHz		
	CKCON = 0;								//�ر�û��ѡ��ʹ�õ���������ʱ�� ( )
	PWCON &= 0xf7;							//����LDO����͹���ģʽ
//------------------------------
	lowspeed = 1;
test_ok_first_time = 0;

low_power_test_time = 50;

}





void quit_low_speed_mode(void)
{	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))	
	PLLCON |= 0xa0;													//PLL��   6��Ƶ 3686400 * 6 = 22118400hZ
	while(!(PLLCON & 0x01));								//�ȴ�PLL�ȶ�
#endif	
	
	CKCON |= IHCKE;
	CKCON |= ILCKE;	//ʹ���ڲ�����ʱ�ӣ����ڱ���pwm										//IRCLʱ��ʹ��
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCH;			//�ڲ� IRCH 4M ʱ��
	
	#if (SYSCLK_SRC == PLL)
	CKSEL = (CKSEL & 0XF8) | 4;							//ϵͳʱ��ѡ�� PLL
	ADCFGL |= 0x60;													//ADCʱ�� 6 ��Ƶ
	#endif	

	PWCON |= 0x08;													//LDO���ûظ߹���ģʽ
	//------------------	

    TR0 = 1;
    ET0 = 1;
   
    //������ڹ��ָܻ�    
    GPIO_Init(BL_ADC_DETECT_PIN, BL_ADC_DETECT_MOD);
    
    //�������ָܻ�

    SCL_HIGH;
	SDA_HIGH;
    
    GPIO_Init(DAC_R_PIN,OUTPUT);
    DAC_R_LOW;
    //V_AMP_CTL_PIN_HIGH;
    V_AMP_CTL_PIN_LOW;
    GPIO_Init(RING_VOL_CTL_PIN, RING_VOL_CTL_MOD);     
    GPIO_Init(BUSY_PIN,INPUT);




    

    //���뿪�ع��ָܻ�
    GPIO_Init(MOTOR_SWITCH_ADC_PIN,			MOTOR_SWITCH_PIN_ADC_MODE);
    GPIO_Init(MODE_SWITCH_ADC_PIN,			MODE_SWITCH_PIN_ADC_MODE);

    //��ص�����⹦�ָܻ�
    GPIO_Init(BAT_ADC_PIN,				P41_ADC1_SETTING);	
		
	lowspeed = 0;//
}









