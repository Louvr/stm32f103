#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_ApSsid              "FlyingEmbeddedStudio"    //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd               "dianxin132"              //Ҫ���ӵ��ȵ����Կ

#define      macUser_ESP8266_TcpServer_IP                 "121.42.180.30"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "8181"               //Ҫ���ӵķ������Ķ˿�



/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );



#endif

