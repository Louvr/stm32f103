#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"
#include <string.h>


volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };



/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
        uint8_t ucStatus;

        //char cStr [ 100 ] = { 0 };

        printf ( "\r\n�������� ESP8266 ......\r\n" );

        macESP8266_CH_ENABLE();  //ʹ��ģ�������

        ESP8266_AT_Test ();     //����ATָ��

        ESP8266_Net_Mode_Choose ( STA );  //ѡ����ģʽ
        //����WiFi     WiFi��������
        while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) ); 
        printf("WiFi����[%s][%d]", __func__, __LINE__);
        //�����ö�����
        ESP8266_Enable_MultipleId ( DISABLE );
        //���ӷ�����       ������Э��     ������IP     �������˿�     ģ��IP
        while ( ! ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
        printf("����������[%s][%d]", __func__, __LINE__);
        ESP8266_Cmd("AT+CIFSR","OK",NULL,500);
        printf("[%s][%d]", __func__, __LINE__);
        //������͸��
        while ( ! ESP8266_UnvarnishSend () );

        printf ( "\r\n���� ESP8266 ���\r\n" );
 

        while ( 1 )
        {       
                char Str[]="{\"M\":\"checkin\",\"ID\":\"3115\",\"K\":\"cde8de9b3\"}\n";
                char shuju[]="{\"M\":\"update\",\"ID\":\"3115\",\"V\":{\"2948\":\"26\"}}\n";
                        
                ESP8266_SendString ( ENABLE, Str, strlen(Str), Single_ID_0 );               //��������
                ESP8266_SendString ( ENABLE, shuju, strlen(shuju),Single_ID_0 );
                
                
                
                
                
                
                
                Delay_ms ( 1000 );
                
                
                
                if ( ucTcpClosedFlag )              //����Ƿ�ʧȥ����
                {
                        ESP8266_ExitUnvarnishSend ();            //�˳�͸��ģʽ
                        
                        do ucStatus = ESP8266_Get_LinkStatus ();     //��ȡ����״̬
                        while ( ! ucStatus );
                        
                        if ( ucStatus == 4 )                 //ȷ��ʧȥ���Ӻ�����
                        {
                                printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
                                
                                while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
                                
                                while ( ! ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
                                
                                printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );

                        }
                        
                        while ( ! ESP8266_UnvarnishSend () );   
                        
                }
        }

                
}


