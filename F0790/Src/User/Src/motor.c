#include "motor.h"

u8 motor_strength;				//��ǿ��   ��pwm��ռ�ձȡ�
bit motor_en;
bit motor_state;	
u8 motor_time_cnt;
u8 motor_cycle;								//һ��ѭ�� �� �Ĵ�����Ϊ0��ʾһֱ����죬���޴Ρ�  ���� �Σ�  �� �� �Σ�   �� �� �� �Ρ�
u8 motor_cycle_cnt;		

bit motor_finish;	



u8 shake_strength[] = {motor_strength_L,motor_strength_M,motor_strength_H};

u8 alarm_shake_seted_strength = 0;					//���õ��𶯵�ǿ��  ��3�����ؿ���
u8 alarm_shake_cycle = 2;										//һ��ѭ���� �� �Ĵ���
/*********************************************************************************************************************
��	�ܣ�������� ת��
��	����
����ֵ��
*********************************************************************************************************************/
void motor_enable(u8 _strength)		
{
	GPIO_Init(MOTOR_AIN_PIN,MOTOR_A_FUNC_EN);		//motor A ��������ΪPWM
	GPIO_Init(MOTOR_BIN_PIN,OUTPUT);						//motor B ��������Ϊ ���
	motor_b = 0;																//motor B ��� 0
	
	INDEX = MOTOR_PWM_CH;
    
	
//	PWMDIVH	= 0;
//	PWMDIVL	= 255;															//4M 14.4KHz	
	
//	PWMDIVH	= 0;
//	PWMDIVL	= 123;															//4M 30KHz
	
//	PWMDIVH	= 0;
//	PWMDIVL	= 73;																//4M 50KHz
	
//	PWMDIVH	= (u8)(442>>8);
//	PWMDIVL	= (u8)442;													//22.1184M 50KHz

	PWMDIVH	= (u8)(221>>8);
	PWMDIVL	= (u8)221;													//22.1184M 100KHz
	
	PWMDUTH = (u8)(_strength>>8);
	PWMDUTL = (u8)_strength;								//����PWM2 ��motor A��PWM�� ռ�ձȣ����������ǿ��
	PWMUPD |= 1 << MOTOR_PWM_CH;								//����PWM6����		
	while(PWMUPD);															//�ȴ��������
	PWMEN |= 1 << MOTOR_PWM_CH;									//����PWM6 ��motor A��PWM��		
}
/*********************************************************************************************************************
��	�ܣ���� �ر�
��	����
����ֵ��
*********************************************************************************************************************/
void motor_disable(void)
{
	PWMEN &= ~(1 << MOTOR_PWM_CH);					//�ر�PWM6 ��motor A��	
	GPIO_Init(MOTOR_AIN_PIN,	OUTPUT);		//motor A ��������Ϊ ���	
	GPIO_Init(MOTOR_BIN_PIN,	OUTPUT);		//motor B ��������Ϊ ���	
	motor_a = 1;
	motor_b = 1;													//�������Ŷ�����ߣ���ɲ��
	Delay_50us(20);//��ʱ1ms
    motor_a = 0;
	motor_b = 0;													//�������Ŷ�����ߣ���ɲ��
	
}


void motor_init(void)
{
    GPIO_Init(MOTOR_AIN_PIN,	OUTPUT);		//motor A ��������Ϊ ���	
    GPIO_Init(MOTOR_BIN_PIN,	OUTPUT);		//motor B ��������Ϊ ���	

    motor_a = 0;
    motor_b = 0;													//�������Ŷ�����ߣ���ɲ��
}

/*********************************************************************************************************************
��	�ܣ�
��	����
����ֵ��
*********************************************************************************************************************/
void motor_start(u8 _strength,u8 _cycle)
{
	motor_state = 0;
	motor_finish = 0;	
	motor_strength = _strength;
//	motor.shaketime = 20;					//
//	motor.stoptime = 8;
	motor_time_cnt = 0;
	motor_cycle = _cycle;
	motor_cycle_cnt = 0;
	
	motor_en = 1;	
	motor_enable(_strength);
}
/*********************************************************************************************************************
��	�ܣ�
��	����
����ֵ��
*********************************************************************************************************************/
void motor_stop(void)
{
	motor_en = 0;
	motor_disable();
}
/*********************************************************************************************************************
��	�ܣ�
��	����
����ֵ��
*********************************************************************************************************************/
void motor_pro(void)
{
	if(motor_en == 0)// || motor.stoptime == 0)
		return;
	
	if(motor_state == 0)
	{
//		if(motor.stoptime > 0)			//����ֹͣ��������һֱ�����������м������
		{
			if(++motor_time_cnt >= SHAKE_TIME/*motor.shaketime*/)
			{
				motor_state = 1;
				motor_time_cnt = 0;
				motor_disable();				
			}
		}		
	}
	else// if(motor_state == 1)
	{
		if(++motor_time_cnt >= STOP_TIME/*motor.stoptime*/ )
		{
			if(motor_cycle > 0)				//������𶯶�Σ�����һ�Σ���������һֱ�𶯣������м������   �� �� ��
			{
				if(++motor_cycle_cnt >= motor_cycle)		//�ﵽ�𶯵Ĵ�����
				{			
					motor_en = 0;
					motor_finish = 1;
					return ;															//ֱ���˳�
				}
			}			
			motor_state = 0;					//���޴���  ��  û�ﵽ�𶯵Ĵ��� ��
			motor_time_cnt = 0;
			motor_enable(motor_strength);							//�ٴ���
		}
	}
}
/*********************************************************************************************************************
��	�ܣ�
��	����
����ֵ��
*********************************************************************************************************************/
void alarm_shake(void)
{
	static u8 cycle_delay_time = 0;				//����ѭ��֮��ļ��ʱ�� 
	static u8 alarm_cycle_times = 0;			//����ѭ���ظ��Ĵ���	
	
	if(alarm_shake_step == 0)
	{
		cycle_delay_time = 0;
		alarm_cycle_times = 0;
		motor_start(shake_strength[alarm_shake_seted_strength],alarm_shake_cycle);
		alarm_shake_step = 1;	
	}
	else
	{
		if(motor_finish)										//�����һ�ε�����ѭ��
		{
			if(++ cycle_delay_time >= 50)			//��ʱ500ms
			{					
				cycle_delay_time = 0;
				motor_finish = 0;

				if(++ alarm_cycle_times >= SHAKE_CYCLE_TIMES)		//ÿ��ѭ�� �ﵽ�ظ����� ��
				{
					alarm_cycle_times = 0;
					alarm_shake_cycle += 1;		
				}
				
				if(alarm_shake_cycle > 4)												//���� �� �� �� �� �� �� ��			
				{					
					motor_start(shake_strength[alarm_shake_seted_strength],0);		
				}
				else
				{
					motor_start(shake_strength[alarm_shake_seted_strength],alarm_shake_cycle);				
				}
			}				
		}	
	}
}

