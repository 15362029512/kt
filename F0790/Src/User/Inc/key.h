#ifndef _KEY_H_
#define _KEY_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"

/* Private define ------------------------------------------------------------*/
#define KEY_AD_CH  			    ADC_CH5
#define KEY_AD_PIN              P45F
#define KEY_AD_MOD              P45_ADC5_SETTING
sbit KEY_AD_GPIO = P4^5;


#define HARD_KEY_NUM	        7	   				        // ʵ�尴������ 
#define KEY_COUNT               (HARD_KEY_NUM + 1)          //ʵ���õ��İ��� = ʵ�尴������ + ˫���İ���
#define KEY_FIFO_SIZE	        15

/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME         5
#define KEY_LONG_TIME           200			/* ��λ5ms�� ����2�룬��Ϊ�����¼� */
#define KEY_REPEAT_TIME         20			/* ����ÿ200ms����һ�ΰ���ֵ */

#define KEY_NONE_AD_VAL         4095

#define KEY_12_24H_SET_AD_VAL           698// (KEY_12_24H_SET_AD_VAL + KEY_SNOOZE_AD_VAL)/2                     
#define KEY_SNOOZE_AD_VAL               1541// (KEY_SNOOZE_AD_VAL + KEY_UP_DOWM_DOUBLE_AD_VAL)/2       
#define KEY_UP_DOWM_DOUBLE_AD_VAL       1825// (KEY_UP_DOWM_DOUBLE_AD_VAL + KEY_UP_AD_VAL)/2           
#define KEY_UP_AD_VAL                   2204// (KEY_UP_AD_VAL + KEY_ALARM_SET_AD_VAL)/2         
#define KEY_ALARM_SET_AD_VAL            2636// (KEY_ALARM_SET_AD_VAL + KEY_ALARM1_AD_VAL)/2     
#define KEY_ALARM1_AD_VAL               3000//2978// (KEY_ALARM1_AD_VAL + KEY_DOWN_AD_VAL)/2          
#define KEY_DOWN_AD_VAL                 3377// (KEY_DOWN_AD_VAL + KEY_ALARM2_AD_VAL)/2          
#define KEY_ALARM2_AD_VAL               3854// (KEY_ALARM2_AD_VAL + KEY_NONE_AD_VAL)/2  
/* Exported macro ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/*
    ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	
*/
typedef struct
{
	uint8_t      Count;         /* �˲��������� */
    uint16_t     LongCount;     /* ���������� */
    uint16_t     LongTime;      /* �������³���ʱ��, 0��ʾ����ⳤ�� */
    uint8_t      State;         /* ������ǰ״̬�����»��ǵ��� */
    uint8_t      RepeatSpeed;   /* ������������ */
    uint8_t      RepeatCount;   /* �������������� */
}KEY_T;

typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
}KEY_FIFO_T;

/* Exported types ------------------------------------------------------------*/
/*����ID,���������Ӧ���±�*/
typedef enum
{
    KID_12_24H_SET = 0,
    KID_SNOOZE,
    KID_UP_DOWM_DOUBLE,
	KID_UP,
	KID_ALARM_SET,
	KID_ALARM1,
	KID_DOWN,
	KID_ALARM2,
}KEY_ID_E;

/*��������ֵ(��ͬ�İ���״̬)*/
typedef enum
{
    KEY_NONE = 0,                   /* 0 ��ʾ�����¼� */

    KEY_12_24H_SET_SHORT_PRESS,                /*����*/
    KEY_12_24H_SET_SHORT_UP,            /*�̰�̧��*/
    KEY_12_24H_SET_LONG_FIRST,          /*��һ�γ���*/
    KEY_12_24H_SET_LONG_REPEAT,         /*��������*/
    KEY_12_24H_SET_LONG_UP,             /*����̧��*/

    KEY_SNOOZE_SHORT_PRESS,                /*����*/
    KEY_SNOOZE_SHORT_UP,            /*�̰�̧��*/
    KEY_SNOOZE_LONG_FIRST,          /*��һ�γ���*/
    KEY_SNOOZE_LONG_REPEAT,         /*��������*/
    KEY_SNOOZE_LONG_UP,             /*����̧��*/

    KEY_UP_DOWM_DOUBLE_SHORT_PRESS,             /*����*/
    KEY_UP_DOWM_DOUBLE_SHORT_UP,         /*�̰�̧��*/
    KEY_UP_DOWM_DOUBLE_LONG_FIRST,       /*��һ�γ���*/
    KEY_UP_DOWM_DOUBLE_LONG_REPEAT,      /*��������*/
    KEY_UP_DOWM_DOUBLE_LONG_UP,          /*����̧��*/
    

    KEY_UP_SHORT_PRESS,             /*����*/
    KEY_UP_SHORT_UP,         /*�̰�̧��*/
    KEY_UP_LONG_FIRST,       /*��һ�γ���*/
    KEY_UP_LONG_REPEAT,      /*��������*/
    KEY_UP_LONG_UP,          /*����̧��*/

    KEY_ALARM_SET_SHORT_PRESS,                /*����*/
    KEY_ALARM_SET_SHORT_UP,            /*�̰�̧��*/
    KEY_ALARM_SET_LONG_FIRST,          /*��һ�γ���*/
    KEY_ALARM_SET_LONG_REPEAT,         /*��������*/
    KEY_ALARM_SET_LONG_UP,             /*����̧��*/

    KEY_ALARM1_SHORT_PRESS,                     /*����*/
    KEY_ALARM1_SHORT_UP,                /*�̰�̧��*/
    KEY_ALARM1_LONG_FIRST,              /*��һ�γ���*/
    KEY_ALARM1_LONG_REPEAT,             /*��������*/
    KEY_ALARM1_LONG_UP,                 /*����̧��*/ 

    KEY_DOWN_SHORT_PRESS,                  /*����*/
    KEY_DOWN_SHORT_UP,              /*�̰�̧��*/
    KEY_DOWN_LONG_FIRST,            /*��һ�γ���*/
    KEY_DOWN_LONG_REPEAT,           /*��������*/
    KEY_DOWN_LONG_UP,               /*����̧��*/  

    KEY_ALARM2_SHORT_PRESS,                  /*����*/
    KEY_ALARM2_SHORT_UP,              /*�̰�̧��*/
    KEY_ALARM2_LONG_FIRST,            /*��һ�γ���*/
    KEY_ALARM2_LONG_REPEAT,           /*��������*/
    KEY_ALARM2_LONG_UP,               /*����̧��*/  
}KEY_ENUM;


/* Exported variables --------------------------------------------------------*/  
extern uint8_t ucKeyCode;      /* ��������ֵ */


/* Private functions ---------------------------------------------------------*/
static void bsp_InitKeyVar(void);
static uint8_t KeyPinActive(uint8_t index);
static uint8_t IsKeyDownFunc(uint8_t _id);
static void bsp_PutKey(uint8_t _KeyCode);
static void bsp_DetectKey(uint8_t i);

/* Exported functions --------------------------------------------------------*/  
extern void key_ad_init(void);
extern uint8_t bsp_GetKey(void);
extern void bsp_KeyScan10ms(void);
extern void key_print(void);
extern void close_key_ad(void);



#endif
