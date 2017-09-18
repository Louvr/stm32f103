/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   WF-ESP8266 WiFiģ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_infrared_send.h"
#include "bsp_GeneralTim.h" 
extern u8 tem;


u8 Code_Tem_26_off[CODE_SIZE] = {0xb2,0x4d,0x7b,0x84,0xe0,0x1f};
u8 Code_Tem_26_on[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0xd0,0x2f};
u8 Code_Tem_26_up[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0xa0,0x5f};
u8 Code_Tem_26_down[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0x70,0x8f};

u8 on_off_flag = 0;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
        
        /* ��ʼ�� */
        USARTx_Config ();                   //��ʼ������1
        systick_init ();                    //���� SysTick Ϊ 1ms �ж�һ�� 
        ESP8266_Init ();                    //��ʼ��WiFiģ��ʹ�õĽӿں�����
        LED_GPIO_Config();
        /*��ʼ��DTT11������*/
        DHT11_Init ();
        //��ʼ����ʱ��
        GENERAL_TIM_Init();
        printf ( "\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n" );                          //��ӡ����������ʾ��Ϣ


        ESP8266_StaTcpClient_UnvarnishTest ();


        while ( 1 );


}


/*********************************************END OF FILE**********************/
