#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"
#include <string.h>
#include "bsp_led.h"
#include "bsp_infrared_send.h"
#include "bsp_GeneralTim.h" 

volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };

extern u8 order;
extern char tem;
extern u8 on_off_flag;
extern u8 Code_Tem_26_off[];
extern u8 Code_Tem_26_on[];
extern u8 Code_Tem_26_up[];
extern u8 Code_Tem_26_down[];




/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
        uint8_t ucStatus;
        
        DHT11_Data_TypeDef   DHT11_Data;
        
        
        printf ( "\r\n�������� ESP8266 ......\r\n" );

        macESP8266_CH_ENABLE();  //ʹ��ģ�������

        ESP8266_AT_Test ();     //����ATָ��

        ESP8266_Net_Mode_Choose ( STA );  //ѡ����ģʽ
        //����WiFi     WiFi��������
        while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) ); 
        printf("WiFi������");
        //�����ö�����
        ESP8266_Enable_MultipleId ( DISABLE );
        //���ӷ�����       ������Э��     ������IP     �������˿�     ģ��IP
        while ( ! ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
        printf("������������");
        
        ESP8266_Cmd("AT+CIFSR","OK",NULL,500);

        //������͸��
        while ( ! ESP8266_UnvarnishSend () );

        printf ( "\r\n���� ESP8266 ���\r\n" );


        while ( 1 )
        {       
                char Str[]="{\"M\":\"checkin\",\"ID\":\"3115\",\"K\":\"52e38a188\"}\n";
                sprintf ( cStr,"{\"M\":\"update\",\"ID\":\"3115\",\"V\":{\"2948\":\"%d.%d\"}}\n",DHT11_Data.temp_int,DHT11_Data.temp_deci);
                
                
                
                ESP8266_SendString ( ENABLE, Str, strlen(Str), Single_ID_0 );               //��������
                ESP8266_SendString ( ENABLE, cStr, strlen(cStr),Single_ID_0 );
                
                
                /*����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
                if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
                {
                        printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
                        DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
                }
                
                else
                {
                        printf("Read DHT11 ERROR!\r\n");
                }
                
                
                switch(order)
                {
                        case 1 :   
                                printf("�Ʒ�ת\r\n");
                                LED2_TOGGLE;
                                order=0;
                                if(on_off_flag)
                                {
                                        Send_infrared_code(Code_Tem_26_off,CODE_SIZE);
                                        printf("�յ�off\r\n");
                                        on_off_flag=~on_off_flag;
                                }
                                else
                                {
                                        Send_infrared_code(Code_Tem_26_on,CODE_SIZE);
                                        printf("�յ�on\r\n");
                                        on_off_flag=~on_off_flag;
                                        
                                }
                                break;
                }
                
                
                
                delay_ms ( 1000 );
                
                
                
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


