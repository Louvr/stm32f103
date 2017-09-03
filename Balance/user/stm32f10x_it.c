#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "bsp_control.h"
#include "bsp_motor.h"

extern int Motor1;
extern int Encoder;
extern int Target_position;

int TIM3_IRQHandler(void)  
{    
        if(TIM3->SR&0X0001)
        {   
                TIM3->SR&=~(1<<0);                           //===�����ʱ��1�жϱ�־λ  
                Encoder=Read_Encoder(4);                     //===��ȡ��������λ������ ��ʼֵ��10000
                Motor1=Incremental_PI(Encoder,11000);          //===λ��PID������
                Xianfu_Pwm();                                //===PWM�޷�
                M1_Revolve(Motor1);                          //===��ֵ��PWM�Ĵ���  
        }
        return 0; 
}

/**************************************************************************
�������ܣ�TIM4�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM4_IRQHandler(void)
{
        if(TIM4->SR&0X0001)//����ж�
        {
        }
        TIM4->SR&=~(1<<0);//����жϱ�־λ    
}
/**************************************************************************
�������ܣ�TIM2�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM2_IRQHandler(void)
{
        if(TIM2->SR&0X0001)//����ж�
        {
        }
        TIM2->SR&=~(1<<0);//����жϱ�־λ  
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
        /* Go to infinite loop when Hard Fault exception occurs */
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
          
        }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
        /* Go to infinite loop when Memory Manage exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
        /* Go to infinite loop when Bus Fault exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
        /* Go to infinite loop when Usage Fault exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
        
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
