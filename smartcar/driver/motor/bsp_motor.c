#include "bsp_motor.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/**
 * @brief ���� TIM8 ��ʱ����� PWM ��
 * @return null
 * @note TIM3 ���ĸ�ͨ��Ϊ PC6��PC7��PC8��PC9
 */
static void PWM_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(MOTOR_PWM_CLOCK, ENABLE);

        GPIO_InitStructure.GPIO_Pin =  MOTOR1_PWM_PIN | MOTOR2_PWM_PIN|Steer_PWM_PIN ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        
}

/**
 * @brief ���õ����ͨ GPIO ������Ҫ���Ƶ��������ת
 * @return null
 */
void MOTOR_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(MOTOR_DIR_CLOCK, ENABLE);  
        
        GPIO_InitStructure.GPIO_Pin =  MOTOR1_DIR_PIN1 | MOTOR1_DIR_PIN2 | MOTOR2_DIR_PIN1 | MOTOR2_DIR_PIN2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        
        MOTOR1_PIN1 = 0 ;
        MOTOR1_PIN2 = 0 ;
        MOTOR2_PIN1 = 0 ; 
        MOTOR2_PIN2 = 0 ;
                      
}
/**
 * @brief ���� PWM �������   ���ö�ʱ��
 * @return null
 */
static void PWM_Mode_Config(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
        
        TIM_TimeBaseStructure.TIM_Prescaler =  7199;         //ʱ��Ԥ��Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;  //���ϼ���
        TIM_TimeBaseStructure.TIM_Period = 200;                    //�Զ���װֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //ʱ�ӷ�Ƶ1
        TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
        
        TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
        
        /*--------------------����ȽϽṹ���ʼ��-------------------*/	
        // ռ�ձ�����
        uint8_t CCR1_Val = 0;
        uint8_t CCR2_Val = 0;
        uint8_t CCR4_Val = 0;
        
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //ģʽһ
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //���ͨ����ƽ��������
//        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;    // ���ͨ�����е�ƽ��������   ����ɲ��
//        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        
        /*���ö�ʱ��ͨ��1*/
        TIM_OCInitStructure.TIM_Pulse = CCR1_Val;    //����CCR�Ĵ���      ռ�ձ� = ��CCR/ARR��
        TIM_OC1Init(TIM8,&TIM_OCInitStructure);     
        TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
        TIM_OCInitStructure.TIM_Pulse = CCR2_Val;    //����CCR�Ĵ���      ռ�ձ� = ��CCR/ARR��
        TIM_OC2Init(TIM8,&TIM_OCInitStructure);     
        TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
         /*���ö�ʱ��ͨ��4*/
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val;    //����CCR�Ĵ���      ռ�ձ� = ��CCR/ARR��
        TIM_OC4Init(TIM8,&TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
        
        
        /*------------------- ��-------------------*/
        // �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������
        TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
        TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
        TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
        TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
        // ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
        // �������õ�����ʱ��Ϊ152ns
        TIM_BDTRInitStructure.TIM_DeadTime = 11;
        TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
        // ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
        TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
        TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
        TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);
        
        
        TIM_CtrlPWMOutputs(TIM8, ENABLE);
        TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
        
        TIM_Cmd(TIM8,ENABLE);   //ʹ��TIM3
       
}

/**
 * @brief PWM ��ʼ��
 * @return null 
 */
void TIM8_PWM_Init(void) 
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
                MOTOR1_PIN1=1;
                MOTOR1_PIN2=0;
                TIM8->CCR2=val;
        }
        else
        {
                MOTOR1_PIN1=0;
                MOTOR1_PIN2=1;
                TIM8->CCR2=abs(val);
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
                MOTOR2_PIN1=1;
                MOTOR2_PIN2=0;
                TIM8->CCR4=val;
                
        }
        else
        {
                MOTOR2_PIN1=0;
                MOTOR2_PIN2=1;
                TIM8->CCR4=abs(val);
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




