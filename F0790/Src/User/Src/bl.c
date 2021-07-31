/* Includes ------------------------------------------------------------------*/
#include "bl.h"
#include "uart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/









/* Private functions ---------------------------------------------------------*/
static void bl_printf(void);
static void get_bl_PWM_duty_value(uint16_t * bl_PWM_duty);
static void bl_param_init(void);
static void set_bl_param(adjust_param_type * bl_param,uint16_t _max_bl_AD_value);
static void bl_adjust_AD_init(void);



/* Private variables ---------------------------------------------------------*/
static uint16_t current_bl_AD_value = 0;   //��ǰ������ť��ֵ
static uint16_t before_bl_AD_value = 0;    //�ϴα�����ť��ֵ


static uint16_t max_bl_AD_value = 3269;    //��ǰ������ڵ����ADֵ
static adjust_param_type bl_param = {0,0};



/* global variables ----------------------------------------------------------*/
uint16_t AD_value_buffer[AD_BUFF_LEN] = {0};
uint16_t bl_PWM_duty = 0;         //����ռ�ձ�




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bl_handler
    @�������ܣ�   ������ڴ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��ֹ��С����ʱ��˸����ADֵС��5ʱ��ֱ����Ϊ����0�ˣ�����0�����������5�����Ƶ���ʽ��������������0������С���ȴ���
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bl_handler(void)
{
    uint8_t set_bl_duty_flg = 0; //�Ƿ���Ҫ���ñ���ռ�ձ�   
    uint16_t bl_AD_differ_value = 0;       //ǰһ������һ��ADֵ�Ĳ�ֵ
    #if (AD_FILTER == 0)
        current_bl_AD_value = GetChannelAdcValue(BL_ADC_DETECT_ADCCH);  //ֱ�ӻ�ȡADֵ
    #elif (AD_FILTER == 1)
        current_bl_AD_value = get_adjust_AD_value(BL_ADC_DETECT_ADCCH); //��ȡ�˲����ADֵ
    #endif
    
    bl_AD_differ_value = get_dvalue_u16(current_bl_AD_value,before_bl_AD_value); //�Աȵ�ǰֵ����һ�ε�ֵ�����ֵ

    if (current_bl_AD_value < 5)  //���ADֵС��5ֱ�ӹرձ���
    {
        current_bl_AD_value = 0;
        if (!set_bl_duty_flg)
        {
            bl_brightness_set(0);
            set_bl_duty_flg = 1;
        }
        
    }

    if (bl_AD_differ_value >= 5) //���ǰһ�����һ�εĲ�ֵ����5������Ϊ���ֶ�����
    {
        set_bl_duty_flg = 0;
        if (current_bl_AD_value <= max_bl_AD_value) //�����ǰֵС�ڵ������ADֵ������֮ǰ�Ķ�Ӧ��ϵ��������
        {
			//do noting
        }
        else
        {
            if ((current_bl_AD_value - max_bl_AD_value) > 20) //�����ǰֵ�������ֵ20��D
            {
				 if (current_bl_AD_value != 0xFFF)
				 {
					 max_bl_AD_value = current_bl_AD_value; //�����趨���ֵ
					 set_bl_param(&bl_param,max_bl_AD_value); //�Ե�ǰ�����ֵ�趨kb����
				 }
            }
        }
        
        get_bl_PWM_duty_value(&bl_PWM_duty); //����kb�趨����
        bl_brightness_set(bl_PWM_duty);    //�趨���ȵ��Ĵ���
        before_bl_AD_value = current_bl_AD_value;
        
    }
	//bl_printf();
}
    
   



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bl_printf
    @�������ܣ�   ������ӡ������Ϣ
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bl_printf(void)
{
	#ifdef PRINT_ENABLE
    uart_printf("current_bl_AD_value = %d\n",current_bl_AD_value);
    uart_printf("bl_PWM_duty = %d\n",bl_PWM_duty);
	#endif
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_bl_PWM_duty_value
    @�������ܣ�   ��ȡ����ռ�ձȵ�ֵ
    @��ڲ�����   ��
    @���ڲ�����   bl_PWM_duty
    @��ע˵����   ����ADֵ�� bl_param �Ĳ�������bl_PWM_duty,
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void get_bl_PWM_duty_value(uint16_t * bl_PWM_duty)
{

    #if (PWM_DUTY_ADJUST_MODE == LINEAR_ADJUST)  
        *bl_PWM_duty = (uint16_t)(bl_param.k * current_bl_AD_value + bl_param.b);
        if (*bl_PWM_duty > BL_BRIGHTNESS_MAX)
        {
            *bl_PWM_duty = BL_BRIGHTNESS_MAX;
        }
    #elif (PWM_DUTY_ADJUST_MODE == CURVILINEAR_ADJUST)
        double temp_max_bl_AD_value = (double)current_bl_AD_value/100;
        double temp_max_bl_duty = 0;
        temp_max_bl_duty = (temp_max_bl_AD_value * temp_max_bl_AD_value * temp_max_bl_AD_value) * bl_param.k;
        *bl_PWM_duty = (uint16_t)(temp_max_bl_duty * 100);
    #endif
    if (*bl_PWM_duty == 0)
    {
        *bl_PWM_duty = 1;
    }
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bl_param_init
    @�������ܣ�   �����adֵ3269�����㣬K,B
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bl_param_init(void)
{
    set_bl_param(&bl_param,max_bl_AD_value);
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   set_bl_param
    @�������ܣ�   ���ݵ�ǰ������ڵ����ֵ����������������
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_bl_param(adjust_param_type * bl_param,uint16_t _max_bl_AD_value)
{   
    #if (PWM_DUTY_ADJUST_MODE == LINEAR_ADJUST)    
    
        bl_param->k = (float)BL_BRIGHTNESS_MAX/(float)_max_bl_AD_value;
        bl_param->b = 0;
    
    #elif (PWM_DUTY_ADJUST_MODE == CURVILINEAR_ADJUST)

        double temp_max_bl_AD_value = 0;
        double temp_max_bl_duty = 0;
        
        temp_max_bl_AD_value = (double)_max_bl_AD_value/100;
        temp_max_bl_duty = (double)(BL_BRIGHTNESS_MAX);
        temp_max_bl_duty = (temp_max_bl_duty/100);
        bl_param->k = temp_max_bl_duty/(temp_max_bl_AD_value * temp_max_bl_AD_value * temp_max_bl_AD_value);
    #endif
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bl_init
    @�������ܣ�   bl_init(PWM4,100HZ)
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   Ҫ����lcd����ʹ�ܱ��⣬PWMʱ��Դʹ�õ���IRCL������ǰ����ʹ��IRCL
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bl_init(void)
{
	#if (BL_CTRL_MOD == BL_IO_CTRL)
        GPIO_Init(BL_CTL_PIN,OUTPUT);
        BL_CTL_GPIO = 1;

    #elif (BL_CTRL_MOD == BL_PWM_CTRL)
        GPIO_Init(BL_CTL_PIN,BL_MOD);

        INDEX = BL_PWMCH;	
        PWMCON = 0x02;					  //PWM4 ʱ��:	IRCH = 131khz,PWM4 100HZ
        PWMDIVH	= (uint8_t)(BL_PWM_DivDat >> 8);
        PWMDIVL	= (uint8_t)BL_PWM_DivDat;
        PWMDUTL = 0;
        PWMDUTH = 0;

        PWMUPD |= (1 << BL_PWMCH);   //���� PWM4 ����	
        while(PWMUPD);              //�ȴ��������
        PWMEN |= (1 << BL_PWMCH);  //ʹ�� PWM4

        bl_adjust_AD_init();   //��ʼ������AD�ɼ�

        current_bl_AD_value = get_adjust_AD_value(BL_ADC_DETECT_ADCCH); //��ȡADֵ
        before_bl_AD_value = current_bl_AD_value;
        if (current_bl_AD_value > max_bl_AD_value)
        {
            if (current_bl_AD_value != 0xFFF)
            {
                 max_bl_AD_value = current_bl_AD_value;
            }
           
        }
        bl_param_init(); //�����ֵ�趨����
        get_bl_PWM_duty_value(&bl_PWM_duty); //���ݲ����趨��ʼ������
        //bl_brightness_set(bl_PWM_duty);    //LCD�� DC ����,�趨���ȵ��Ĵ���
    #endif

}

uint8_t bl_pwm_mode;


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bl_brightness_set
    @�������ܣ�   ���ñ�������
    @��ڲ�����   bl_duty:����ռ�ձ�
    @���ڲ�����   ��
    @��ע˵����   ��Χ0~1300
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bl_brightness_set(uint16_t bl_duty)
{
    if (!bl_pwm_mode)
    {
        GPIO_Init(BL_CTL_PIN,BL_MOD);
        bl_pwm_mode = 1;
    }
    INDEX = BL_PWMCH;
    PWMDUTH = (uint8_t)(bl_duty >> 8);
    PWMDUTL = (uint8_t)bl_duty;
    
    PWMUPD |= (1 << BL_PWMCH);   //���� PWM4 ����	
    while(PWMUPD);              //�ȴ��������
    PWMEN |= (1 << BL_PWMCH);  //ʹ�� PWM4
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   bl_adjust_AD_init
    @�������ܣ�   ��ʼ�����������ť,
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   P44 ADC4 ��ʼ��ǰ��������ADC
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bl_adjust_AD_init(void)
{    
    GPIO_Init(BL_ADC_DETECT_PIN, BL_ADC_DETECT_MOD);      //ch5,������ڼ��
    ADCON = 0x3d;
    //ADCFGL |= 0X60;				//ADCʱ��6��Ƶ 
    ADCFGL |= 0xc0;	
}



void bl_close(void)
{
    GPIO_Init(BL_CTL_PIN,OUTPUT);
    BL_CTL = 0;

    GPIO_Init(BL_ADC_DETECT_PIN,HIGH_Z);
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_adjust_AD_value
    @�������ܣ�   ��ȡ���������ťֵ
    @��ڲ�����   ADC_Channel_TypeDef ��Ӧ��ADͨ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint16_t get_adjust_AD_value(ADC_Channel_TypeDef adc_chx)
{
    uint8_t i = 0;
    uint32_t sum = 0;
    for (i = 0; i < AD_BUFF_LEN; i++)
    {
        AD_value_buffer[i] = GetChannelAdcValue(adc_chx);
        sum += AD_value_buffer[i];		
    }
    sum /= AD_BUFF_LEN;
    return (uint16_t)sum;
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_dvalue_u16
	@�������ܣ�   ��ȡ�������Ĳ�ֵ
	@��ڲ�����   ��  
	@���ڲ�����   differ_value��ֵ�����value1 > value2 ����max_flg = 1
	@��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint16_t get_dvalue_u16(uint16_t value1,uint16_t value2)
{
    if (value1 > value2)
    {
        return (value1 - value2);
    }
    return (value2 - value1);
}






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_dvalue_u8
	@�������ܣ�   ��ȡ�������Ĳ�ֵ
	@��ڲ�����   ��  
	@���ڲ�����   differ_value��ֵ�����value1 > value2 ����max_flg = 1
	@��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint8_t get_dvalue_u8(uint8_t value1,uint8_t value2)
{
    if (value1 > value2)
    {
        return (value1 - value2);
    }
    return (value2 - value1);
}















