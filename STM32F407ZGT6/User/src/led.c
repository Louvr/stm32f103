#include "led.h"

/* �Ĵ�����ʼ��(LED4 -- PC0) */
//void LED_Init(void)
//{
//	RCC->AHB1ENR |= 1 << 2;
//	GPIOC->MODER &= ~(3 << (2 * 0));
//	GPIOC->MODER |= 1 << (2 * 0);
//	GPIOC->OTYPER &= ~(1 << 0);
//	GPIOC->OSPEEDR &= ~(3 << (2 * 0));
//	GPIOC->OSPEEDR |= 3 << (2 * 0);
//	GPIOC->PUPDR &= ~(3 << (2 * 0));
//	GPIOC->ODR |= 1 << 0;
//}


/* �⺯����ʼ��(LED4 -- PC0) */
void LED_Init(void)
{
        /* �⺯���ṹ����� */
        GPIO_InitTypeDef GPIO_InitStructure;                   //����GPIO�ڽṹ�����

        /* ����GPIO����ģʽ */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  //ʹ��GPIOCʱ��
        
        GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_OUT;         //ͨ�����ģʽ
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //�������
        GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;       //����ٶ�50M
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //����Ҫ��/����

        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //����GPIOC.0�˿�
        GPIO_Init(GPIOC,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;              //����GPIOC.0�˿�
        GPIO_Init(GPIOF,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //����GPIOC.0�˿�
        GPIO_Init(GPIOF,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //����GPIOC.0�˿�
        GPIO_Init(GPIOF,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        GPIO_SetBits(GPIOC,GPIO_Pin_0);
        GPIO_SetBits(GPIOF,GPIO_Pin_6);
        GPIO_SetBits(GPIOF,GPIO_Pin_9);
        GPIO_SetBits(GPIOF,GPIO_Pin_10);
        
        
} 
void KEY_Init(void)
{
        /* �⺯���ṹ����� */
        GPIO_InitTypeDef GPIO_InitStructure;                   //����GPIO�ڽṹ�����

        /* ����GPIO����ģʽ */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOCʱ��
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //ʹ��GPIOCʱ��
        
        GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;         //ͨ������ģʽ
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //�������
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //����Ҫ��/����

        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //����GPIOC.0�˿�
        GPIO_Init(GPIOA,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;              //����GPIOC.0�˿�
        GPIO_Init(GPIOE,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;              //����GPIOC.0�˿�
        GPIO_Init(GPIOE,&GPIO_InitStructure);                   //��ʼ��GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;              //����GPIOC.0�˿�
        GPIO_Init(GPIOE,&GPIO_InitStructure);                   //��ʼ��GPIOC
} 


















