#include "bsp_motor.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/**
 * @brief ���� TIM3 ��ʱ����� PWM ��
 * @return null
 * @note TIM3 ���ĸ�ͨ��Ϊ PA6��PA7��PB0��PB1
 */
static void PWM_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 ;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief ���õ����ͨ GPIO ������Ҫ���Ƶ��������ת
 * @return null
 */
void MOTOR_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        PBout(12)=0;
        PBout(13)=0;
        PBout(14)=0;
        PBout(15)=0;

}
/**
 * @brief ���� PWM �������   ���ö�ʱ��
 * @return null
 */
static void PWM_Mode_Config(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
        
        TIM_TimeBaseStructure.TIM_Prescaler =  71;         //ʱ��Ԥ��Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;  //���ϼ���
        TIM_TimeBaseStructure.TIM_Period = 99;                    //�Զ���װֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //ʱ�ӷ�Ƶ1
        TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
        
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
        
        /*--------------------����ȽϽṹ���ʼ��-------------------*/	
        // ռ�ձ�����
        uint8_t CCR1_Val = 50;
        uint8_t CCR4_Val = 50;
        
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //ģʽһ
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //���ͨ����ƽ��������
        
        /*���ö�ʱ��ͨ��1*/
        TIM_OCInitStructure.TIM_Pulse = CCR1_Val;    //����CCR�Ĵ���      ռ�ձ� = ��CCR/ARR��
        TIM_OC1Init(TIM3,&TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
         /*���ö�ʱ��ͨ��4*/
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val;    //����CCR�Ĵ���      ռ�ձ� = ��CCR/ARR��
        TIM_OC4Init(TIM3,&TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
        
        TIM_Cmd(TIM3,ENABLE);   //ʹ��TIM3
}

/**
 * @brief PWM ��ʼ��
 * @return null 
 */
void TIM3_PWM_Init(void) 
{
        PWM_GPIO_Config();
        PWM_Mode_Config();
        MOTOR_GPIO_Config();
}


/**
 * @brief ���1��ת��
 * @param VALռ�ձȼ���ֵ�䷶Χ�ǣ�-100,100��
 *        ������ʾ��ת��������ʾ��ת
 * @return null
 * @note VAL�ľ���ֵԽ��ת��Խ��
 */
void M1_Revolve(int val)
{
        if (val>0)
        {
                PBout(14)=1;
                PBout(15)=0;
                TIM1->CCR1=val;
        }
        else
        {
                PBout(14)=0;
                PBout(15)=1;
                TIM1->CCR1=abs(val);
        }

}
/**
 * @brief ���2��ת��
 * @param VALռ�ձȼ���ֵ�䷶Χ�ǣ�-100,100��
 *        ������ʾ��ת��������ʾ��ת
 * @return null
 * @note VAL�ľ���ֵԽ��ת��Խ��
 */
void M2_Revolve(int val)
{
        if (val>0)
        {
                PBout(12)=1;
                PBout(13)=0;
                TIM3->CCR4=val;
        }
        else
        {
                PBout(12)=0;
                PBout(13)=1;
                TIM3->CCR4=abs(val);
        }

}
/**
 * @brief ����ֵ����
 * @param ��Ҫȡ����ֵ����
 * @return ����ֵ
 */

int abs (int val)
{
        int temp;
        if(val<0)
        temp=-val;
        else
        temp=val;
        return temp;
}




