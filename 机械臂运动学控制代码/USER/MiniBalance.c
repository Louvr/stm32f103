#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
u8 Flag_Way=0,Flag_Show=0;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Servo1,Servo2,Servo3,Servo4,Servo5;     //���PWM����
float Velocity1,Velocity2,Velocity3,Velocity4,Velocity5,Velocity6;     //���PWM����
float Position1=750,Position2=750,Position3=750,Position4=750,Position5=750,Position6=750;    
float Speed=10;                   //�˶��ٶ�����
int Voltage;                      //��ص�ѹ������صı���
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send; //��ʱ�͵��εȱ���
u8 ps2_mode;
float Balance_Kp=350,Balance_Kd=0,Velocity_Kp=70,Velocity_Ki=0.7;//PID����
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash������� 
float Target_X=750,Target_Y=750	,Target_Z=750;                     
float Target1=750,Target2=735,Target3=717,Target4=702,Target5=750,Target6=750;     //���Ŀ��ֵ
float TargetX=0.2,TargetY=0,Target_Beta=45,Target_Alpha=0;     //���Ŀ��ֵ
float	Position_KP=6,Position_KI=0,Position_KD=3;  //λ�ÿ���PID����
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
int main(void)
  { 
		delay_init();	    	            //=====��ʱ������ʼ��	
		uart_init(128000);	            //=====���ڳ�ʼ��Ϊ
		JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
		LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
//	  KEY_Init();                     //=====������ʼ��
		MY_NVIC_PriorityGroupConfig(2); //=====�жϷ���
		delay_ms(100);                  //=====��ʱ�ȴ�
	//	uart3_init(9600);               //=====����3��ʼ��
		Adc_Init();                     //=====adc��ʼ��
		delay_ms(100);                  //=====��ʱ�ȴ�
    OLED_Init();                    //=====OLED��ʼ��	
		Beep_Init();
    TIM1_Int_Init(99,7199);         //=====��ʱ��10ms�ж�
		TIM2_PWM_Init(9999,143);        //=====PWM��ʼ��
		TIM4_PWM_Init(9999,143);        //=====PWM��ʼ��
		PS2_Init();                     //=====PS2�ֱ���ʼ��
		PS2_SetInit();  //=====ps2���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
		Beep_Test();
    while(1)
	   {
			PS2_LX=PS2_AnologData(PSS_LX);    
			PS2_LY=PS2_AnologData(PSS_LY);
			PS2_RX=PS2_AnologData(PSS_RX);
			PS2_RY=PS2_AnologData(PSS_RY);
			PS2_KEY=PS2_DataKey();	
			ps2_mode=PS2_RedLight();
			 if(ps2_mode==0)
			 {
				Flag_Way=0;
			 }
			 else 
				{
					Flag_Way=1;
        }
			 	if(Flash_Send==1)        //д��PID������Flash,��app���Ƹ�ָ��
					{
          	Flash_Write();	
						Flash_Send=0;	
					}	
		 //   if(Flag_Show==0)           
			//		{
					//		APP_Show();	
				      oled_show();          //===��ʾ����
				//	}
				//	else                   
					//{
				      DataScope();      
					//}	
					delay_flag=1;	
					delay_50=0;
					while(delay_flag);	     	
	   } 
}

