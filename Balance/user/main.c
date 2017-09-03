#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_motor.h"
#include "bsp_control.h"
#include "DataScope_DP.h"
#include "bsp_i2c.h"
#include "bsp_mpu6050.h"

#include <math.h>

/***
 * �˹��̲�֪����ʲôԭ������ʽ��ΪGB2312ģʽ
 * �ȷ��ֵ�ʱ��д�˲���ע�ͣ��ֲ�Ը��д�ͱ���GBK�ı����ʽ
 */
float Acceleration_Z;                       //Z����ٶȼ�
float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������
int Temperature;     //��ʾ�¶�
float angle, angle_dot; 
extern float Accel_Angle;
int Motor1;
//int Target_position=11000;    //��ʼֵ��10000��Ŀ��ֵ��11000
int Encoder=10000,Position=1000;
float Menu_MODE=1,Menu_PID=1;  //PID��ر�־λ
char Flag_Show=0;

int main(void)
{
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        MOTOR_GPIO_Config();
        Encoder_Init_TIM4();
        Encoder_Init_TIM2();
        MiniBalance_PWM_Init(7199,0);
        Timer3_Init(99,7199);
        IIC_Init();
        MPU6050_Init();
        
        
        while(1)
        {
//                printf("[%s][%d]\r\n", __func__, __LINE__);
                test_mpu6050();
                
                
                
        }
} 
