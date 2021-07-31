/* Includes ------------------------------------------------------------------*/
#include "ring.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 


#define VOL_MAX_LEVEL 15 //(��������(F0~FF)�������һ����17��)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t current_ring_vol_AD_value = 0;     //��ǰ������ť��ֵ
static uint16_t before_ring_vol_AD_value = 0;     //�ϴ�������ť��ֵ
static uint16_t max_ring_vol_AD_value = 3269;     //���������ADֵ  


uint8_t spk_cmd_ing = 0;          //���ڷ�������
static uint8_t spk_cmd_step = 0;         //��������Ĳ���
static uint8_t spk_time_cnt = 0;         
static uint8_t cmd_bit_cnt = 0;


//��ͬ�������ȼ�����450����Ϊ15�Σ�ÿһ��վ��ͬ�ı���:90,90,40,40,35,35,35,25,20,5,5,5,5,5,15
static uint16_t vol_level_percent_array[VOL_MAX_LEVEL] = {90,180,220,260,295,330,365,390,410,415,420,425,430,435,450};



/* global variables ----------------------------------------------------------*/
uint8_t g_cmd = cmd_ring_stop;				//ͨ��ʱҪ���͵�����		
uint8_t need_replay = 0;
uint8_t g_alarm_ring_index	= 0x02;			//����ѡ�������	

uint8_t g_current_ring_vol_level = 0XF0;     //��ǰ�����ĵȼ�  

uint8_t alarm_ring_cycle = 2;								//һ��ѭ���� �� �Ĵ���


uint8_t ring_gap_time = 0;



/* Private functions ---------------------------------------------------------*/
static uint8_t get_current_vol_level(void);
static void ring_vol_printf(void);
static void ring_vol_ad_init(void);



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   ring_vol_ad_init
    @�������ܣ�   ��ʼ�������������ΪADCH3
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void ring_vol_ad_init(void)
{
    GPIO_Init(RING_VOL_CTL_PIN, RING_VOL_CTL_MOD);      //ch3,�������ڼ��
	ADCON = 0x3d;
    //ADCFGL |= 0X60;				//ADCʱ��6��Ƶ 
    ADCFGL |= 0xc0;	
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   ring_init
    @�������ܣ�   ��ʼ������������ţ�����IC��������⣬������ʼ������
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void ring_init(void)
{
	GPIO_Init(BUSY_PIN,INPUT);
	GPIO_Init(SCL_PIN,OUTPUT);
	GPIO_Init(SDA_PIN,OUTPUT);
    GPIO_Init(DAC_R_PIN,OUTPUT);
    GPIO_Init(V_AMP_CTL_PIN,OUTPUT);

    /*
	SCL_LOW;
    SDA_LOW;
    */

	SCL_HIGH;
    SDA_HIGH;

    DAC_R_LOW;
    V_AMP_CTL_PIN_LOW;
   

    ring_vol_ad_init();
    current_ring_vol_AD_value = get_adjust_AD_value(RING_VOL_CTL_ADCCH);
	if (current_ring_vol_AD_value > max_ring_vol_AD_value)
	{
		max_ring_vol_AD_value = current_ring_vol_AD_value;
	}
    before_ring_vol_AD_value = current_ring_vol_AD_value;
	
    g_current_ring_vol_level = get_current_vol_level();    //��ȡ��ʼ�����ȼ�
	//g_current_ring_vol_level = 0xF0;
}


void ring_close(void)
{
    
    GPIO_Init(BUSY_PIN,HIGH_Z);
    SCL_LOW;
	SDA_LOW;
    GPIO_Init(DAC_R_PIN,HIGH_Z);
    GPIO_Init(RING_VOL_CTL_PIN, HIGH_Z);      //�������ڼ��ر�
    DAC_R_HIGH;
    V_AMP_CTL_PIN_LOW;
    
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   get_current_vol_level
    @�������ܣ�   ��ȡ��ǰ�����ȼ�
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   һ��15���ȼ� ����0xf0~0xff,0xefΪ�ر�����
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t get_current_vol_level(void)
{
    float temp_value = 0;
	float temp_level = 0;
	uint16_t _level = 0;
    uint8_t i = 0;
    uint16_t dvalue = 0;
    uint16_t dvalue_min = 450;
    uint8_t dvalue_min_index = 0;

	if (current_ring_vol_AD_value != 0)
	{
		temp_value = (float)max_ring_vol_AD_value/(float)(VOL_MAX_LEVEL * 30); //����ǰ������̷�Ϊ450��
		temp_value = (uint16_t)(temp_value + 0.5);                   //�����ǰ��AD��Լռ���ٷ�
		temp_level = (float)current_ring_vol_AD_value/temp_value; 
		_level = (uint16_t)(temp_level + 0.5);                   //�����ǰ��AD��Լռ���ٷ�

        //�������ݣ�����뵱ǰ������ӽ���
        for (i = 0; i < 15; i++)
        {
            dvalue = get_dvalue_u16(_level,vol_level_percent_array[i]);
            if (dvalue < dvalue_min)
            {
                dvalue_min_index = i;
                dvalue_min = dvalue;
            }
        }
        dvalue_min_index++;
		dvalue_min_index += 0xF0;
	}		
    return dvalue_min_index;
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   ring_vol_adjust_handler
    @�������ܣ�   �����ȼ����ڴ���
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ������20ms����һ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void ring_vol_adjust_handler(void)
{
    
    uint16_t ring_vol_AD_differ_value = 0;       //ǰһ������һ��ADֵ�Ĳ�ֵ

    #if (AD_FILTER == 0)
        current_ring_vol_AD_value = GetChannelAdcValue(RING_VOL_CTL_ADCCH);  //ֱ�ӻ�ȡADֵ
    #elif (AD_FILTER == 1)
        current_ring_vol_AD_value = get_adjust_AD_value(RING_VOL_CTL_ADCCH); //��ȡ�˲����ADֵ
    #endif

    if (current_ring_vol_AD_value == 0)
	{
		g_current_ring_vol_level = 0xF0;   //��С����
	}

    else 
    {        
        ring_vol_AD_differ_value = get_dvalue_u16(current_ring_vol_AD_value,before_ring_vol_AD_value); //�Աȵ�ǰֵ����һ�ε�ֵ�����ֵ

        if (ring_vol_AD_differ_value >= 5) //����������10������Ϊ���ֶ�����
        {
            if (current_ring_vol_AD_value <= max_ring_vol_AD_value)
        	{
        		//do nothing
        	}
        	else
        	{
        		if ((current_ring_vol_AD_value - max_ring_vol_AD_value ) > 20)
        		{
        			 if (current_ring_vol_AD_value != 0xFFF)
        			 {
        				max_ring_vol_AD_value = current_ring_vol_AD_value;
        			 }      			 
        		}
        	}
        	g_current_ring_vol_level = get_current_vol_level();
        	before_ring_vol_AD_value = current_ring_vol_AD_value;
        	//ring_vol_printf();
        }
    }
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   ring_vol_printf
    @�������ܣ�   ��ӡ��ǰ��������Ϣ
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void ring_vol_printf(void)
{
	#ifdef PRINT_ENABLE
    uart_printf("current_ring_vol_AD_value = %d\n",current_ring_vol_AD_value);
    uart_printf("g_current_ring_vol_level = %d\n",g_current_ring_vol_level);
	#endif
}

uint8_t addr = 0;


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   send_ring_cmd
	@�������ܣ�   ������������	
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   ��
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void send_ring_cmd(uint8_t CMD)
{	
	if (!spk_cmd_ing)
	{
        V_AMP_CTL_PIN_HIGH;    
        SCL_HIGH;
		g_cmd = CMD;
		need_replay = 1;
		cmd_bit_cnt = 0;
		//SCL_HIGH;                 //���� ʱ���� 400us
		spk_time_cnt = 4;
	  
		spk_cmd_ing = 1;
		spk_cmd_step = 0;
		

	#if (SYSCLK_SRC == IRCH)  //  3686400/12 = 307200
		TH1 = 0xe1;
		TL1 = 0xe1;     //(0xff - 0xe1) = 30 / 307200 = 1/10000s = 100us
	#elif (SYSCLK_SRC == PLL) //  22118400 / 12 = 1843200
		TH1 = 0x47;
		TL1 = 0x47;     //(0xff - 0x47) = 184 / 1843200 = 1/10000s = 100us
	#endif
		TR1 = 1;
		ET1 = 1;
		if (g_cmd != cmd_ring_stop)
        {
            send_ring_cmd_step++;
        }		
	}
}



uint8_t sig_cnt = 0;



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@�������ƣ�   ring_cmd_isr_handler
	@�������ܣ�   ���������жϴ������	
    @��ڲ�����   ��
    @���ڲ�����   ��
    @��ע˵����   timer1 100us����
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void ring_cmd_isr_handler(void)
{
    if (spk_cmd_ing)
	{
		if (spk_time_cnt)
		{
			spk_time_cnt--;
		}
		
		switch(spk_cmd_step)
		{
			case 0:
			{
				if (!spk_time_cnt) //���ʱ����������400us
				{
					spk_cmd_step = 1;         //�ж������������߻�������
				}
			}
			break;

			case 1:
			{
                if (!spk_time_cnt)
                {
                    if(g_cmd & (0x80 >> cmd_bit_cnt))   //�жϸ�λ
    				{
    					SDA_HIGH;
    				}
    				else
    				{
    					SDA_LOW;
    				}
                    SCL_LOW;
                    spk_cmd_step = 2;
                    spk_time_cnt = 4;                    
                }

			}
			break;

			case 2:
			{
				if (!spk_time_cnt)
				{
					cmd_bit_cnt++;
					spk_cmd_step = 3; 
					spk_time_cnt = 4;
                    SCL_HIGH;

				}

			}
			break;
			
			case 3:
			{
				if (!spk_time_cnt)
				{
					if (cmd_bit_cnt > 7)
					{
						cmd_bit_cnt = 0;
						spk_cmd_ing = 0;
						spk_cmd_step = 0;

                        
                        if (g_cmd == cmd_ring_stop)
                        {
                            V_AMP_CTL_PIN_LOW;
                            //SCL_LOW;
                            
                        }/*
                        if (g_cmd != cmd_ring_stop)
                        {
                            send_ring_cmd_step++;
                        }
                        if ((send_ring_cmd_step >= 2)||(g_cmd == cmd_ring_stop))
                        {
                            send_ring_cmd_step = 0;
                        }
                        */
					}
					else
					{
						spk_cmd_step = 1;
					}
				}
			}     
			break;            
            default:break;
		}	
	}
}






void turn_on_tone(void)
{

}



void send_byte(uint8_t addr)
{
    uint8_t i;
    SCL_HIGH;
    Delay_50us(8);
    for(i=0;i<8;i++)
    {
        if(addr&0x80)
        {
            SDA_HIGH;
        }
        
        else
        {
             SDA_LOW;
        }     
        SCL_LOW;
        Delay_50us(8);
        SCL_HIGH;
        Delay_50us(8);
        addr<<=1;
    } 
}

















uint8_t send_once_flg = 0;
uint8_t send_ring_cmd_step = 0;  // step = 0,��������  step = 1 ������������  step = 2����ʱ




void alarm_tone(void)
{
    if (!busy)
    {
        if (!ring_gap_time)
        {
			if (send_ring_cmd_step == 0)
			{
				send_ring_cmd(g_current_ring_vol_level);
			}
            if (send_ring_cmd_step == 1)
			{
				 send_ring_cmd(g_alarm_ring_index);
			}
			if (send_ring_cmd_step >= 2)
			{
				ring_gap_time = 200;
			   send_ring_cmd_step = 0;
			}
        }

    }
}


uint8_t ring_stop_flg = 0;

void ring_stop(void)
{
   
    if (g_cmd != cmd_ring_stop)
    {
		#if 0
		SCL_HIGH;                 //���� ʱ���� 400us
		SDA_HIGH;
		#endif
		
		
		
        if (!busy)
    	{       
    		send_ring_cmd(cmd_ring_stop);
            ring_gap_time = 0;
    		ring_stop_flg = 0;
    	}
		
    }
}

