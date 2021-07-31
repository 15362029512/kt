#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "user_type.h"
#include "bsp.h"

/*********************************************************************************************************************/
sbit motor_a	= P3^3;
sbit motor_b 	= P3^7;

#define MOTOR_AIN_PIN					P33F
#define MOTOR_BIN_PIN					P37F

#define MOTOR_A_FUNC_EN				P33_PWM6_SETTING			//PWM6
#define MOTOR_A_FUNC_DIS			OUTPUT

#define MOTOR_B_FUNC					OUTPUT


#define MOTOR_PWM_CH					6									


#define SHAKE_TIME						50								//��ʱ��	��λ��10ms
#define STOP_TIME 						10								//ֹͣʱ��	��λ��10ms

#define MOTOR_FREQ						PLL								//���PWM2 ��ʱ��
#define MOTOR_CTR_PWM_FREQ		100000						//���Ƶ����pwm��Ƶ��  	��λ��Hz	


#define motor_strength_L			137				//62%			  
#define motor_strength_M			159				//72%
#define motor_strength_H			199				//90%

#define STRENGTH_L			0					//��ǿ�� ��
#define STRENGTH_M			1					//��ǿ�� ��
#define STRENGTH_H			2					//��ǿ�� ��

#define SHAKE_CYCLE_TIMES			3//8
/*********************************************************************************************************************/
typedef struct
{
//	u16 freq;					//��Ƶ��
	u8 strength;				//��ǿ��   ��pwm��ռ�ձȡ�
	u8 enable;
	u8 state;	
//	u8 shaketime;			//��ʱ��
//	u8 stoptime;			//ֹͣʱ��
	u8 time_cnt;
	
	u8 cycle;								//һ��ѭ�� �� �Ĵ�����Ϊ0��ʾһֱ����죬���޴Ρ�  ���� �Σ�  �� �� �Σ�   �� �� �� �Ρ�
	u8 cycle_cnt;		
	
	u8 finish;	
}_MOTOR;


extern _MOTOR motor;
extern u8 alarm_shake_seted_strength;
extern u8 alarm_shake_cycle;
extern u8 shake_strength[3];


extern void motor_init(void);


extern void motor_start(u8 _strength,u8 _cycle);
extern void motor_stop(void);

extern void motor_pro(void);
extern void alarm_shake(void);


extern void motor_enable(u8 _strength);		



#endif
