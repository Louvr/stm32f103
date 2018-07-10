#include "stm32f10x.h"   
#include "bsp_usart.h"   
#include "bsp_lcd.h"
#include "fonts.h"
#include "bsp_spi.h"
#include <stdio.h>
#include "bsp_touch.h"
#include "bsp_led.h"
#include "bsp_rtc.h"

struct rtc_time systmtime;

int main(void)
{                
//        uint16_t test=0;
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        RTC_NVIC_Config();
        
        USART_Config();
        
        RTC_CheckAndConfig(&systmtime);
        
        LCD_Init();
        
        USART_Config ();
        
        LED_GPIO_Config();
        
        BEEP_GPIO_Config();
        
        LCD_SetColors(RED,WHITE);
        LCD_Clear(0,0,240,320);
//        LCD_DispString_CH(88,50,"��������");
//        LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"��������",0);
         LCD_DisplayStringEx(72,30,32,32,(uint8_t *)"����ʿ",0);
        /*�Ƿ�ǿ��У׼����flash���в�����У׼
        ����һΪɨ�跽ʽ������1Ϊǿ��У׼0��ǿ��У׼*/
        
//        LCD_DrawLine(30,100,30,50);
        
//        LCD_Touch_Calibrate(3);//��ĻУ׼
        while(1)
        {
                Time_Show(&systmtime);
                LCD_Time_Show(&systmtime);
        }
        
}


