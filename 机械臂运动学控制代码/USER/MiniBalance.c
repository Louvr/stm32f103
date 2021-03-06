#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
u8 Flag_Way=0,Flag_Show=0;                 //停止标志位和 显示标志位 默认停止 显示打开
int Servo1,Servo2,Servo3,Servo4,Servo5;     //电机PWM变量
float Velocity1,Velocity2,Velocity3,Velocity4,Velocity5,Velocity6;     //电机PWM变量
float Position1=750,Position2=750,Position3=750,Position4=750,Position5=750,Position6=750;    
float Speed=10;                   //运动速度设置
int Voltage;                      //电池电压采样相关的变量
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send; //延时和调参等变量
u8 ps2_mode;
float Balance_Kp=350,Balance_Kd=0,Velocity_Kp=70,Velocity_Ki=0.7;//PID参数
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash相关数组 
float Target_X=750,Target_Y=750	,Target_Z=750;                     
float Target1=750,Target2=735,Target3=717,Target4=702,Target5=750,Target6=750;     //电机目标值
float TargetX=0.2,TargetY=0,Target_Beta=45,Target_Alpha=0;     //电机目标值
float	Position_KP=6,Position_KI=0,Position_KD=3;  //位置控制PID参数
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
int main(void)
  { 
		delay_init();	    	            //=====延时函数初始化	
		uart_init(128000);	            //=====串口初始化为
		JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
		LED_Init();                     //=====初始化与 LED 连接的硬件接口
//	  KEY_Init();                     //=====按键初始化
		MY_NVIC_PriorityGroupConfig(2); //=====中断分组
		delay_ms(100);                  //=====延时等待
	//	uart3_init(9600);               //=====串口3初始化
		Adc_Init();                     //=====adc初始化
		delay_ms(100);                  //=====延时等待
    OLED_Init();                    //=====OLED初始化	
		Beep_Init();
    TIM1_Int_Init(99,7199);         //=====定时器10ms中断
		TIM2_PWM_Init(9999,143);        //=====PWM初始化
		TIM4_PWM_Init(9999,143);        //=====PWM初始化
		PS2_Init();                     //=====PS2手柄初始化
		PS2_SetInit();  //=====ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
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
			 	if(Flash_Send==1)        //写入PID参数到Flash,由app控制该指令
					{
          	Flash_Write();	
						Flash_Send=0;	
					}	
		 //   if(Flag_Show==0)           
			//		{
					//		APP_Show();	
				      oled_show();          //===显示屏打开
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

