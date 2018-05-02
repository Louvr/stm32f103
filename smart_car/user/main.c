#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "led.h"
#include "24l01.h"
#include "bsp_spi.h"

//int mode =0;


int main (void )
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        spi_Init();
        NRF24L01_Init();
        u8 tmp_buf[33];
        if(NRF24L01_Check())
        {
                 printf("error");
                 delay_ms(500);
        }
        else
        {
               printf("success");
        }
        while(1)
        {
                 
        }
        
        
        
}
//int main(void)
//{ 
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//        
//        systick_init();
//        usart_init();
//        
//        u16 t=0;			 
//        u8 tmp_buf[33];

//        NRF24L01_Init();    //��ʼ��NRF24L01 
//        while(NRF24L01_Check())//��ⲻ��24L01
//        {
//                delay_ms(500);
//                LED0=!LED0;//DS0��˸
//        }
//        while(1)//�ڸò���ȷ�������ĸ�ģʽ!
//        {

//        
//        if(mode==0)//RXģʽ
//        {
//                RX_Mode();		  
//                while(1)
//                {	  		    		    				 
//                        if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
//                        {
//                                tmp_buf[32]=0;//�����ַ���������
//                        }else delay_us(100);	   
//                        t++;
//                        if(t==10000)//��Լ1s�Ӹı�һ��״̬
//                        {
//                                t=0;
//                                LED0=!LED0;
//                        } 				    
//                };	
//        }else//TXģʽ
//        {							    
//                LCD_ShowString(60,150,"NRF24L01 TX_Mode");	
//                TX_Mode();
//                mode=' ';//�ӿո����ʼ  
//                while(1)
//                {	  		   				 
//                        if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
//                        {
//                                key=mode;
//                                for(t=0;t<32;t++)
//                                {
//                                        key++;
//                                        if(key>('~'))key=' ';
//                                        tmp_buf[t]=key;	
//                                }
//                                mode++; 
//                                if(mode>'~')mode=' ';  	  
//                                tmp_buf[32]=0;//���������		   
//                        }else
//                        {									 
//                        };
//                        LED0=!LED0;
//                        delay_ms(1500);				    
//                };
//        }     	  
//}
//        }

        
