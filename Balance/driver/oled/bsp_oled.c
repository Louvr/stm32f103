#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_systick.h"
#include "codetab.h"
#include "bsp_usart.h"


int WriteCmd(u8 cmd)
{
        IIC_Start();
        IIC_Send_Byte(OLED_ADDRESS);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(0x00);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(cmd);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                IIC_Stop();
                return -1;
        }
        IIC_Stop();
        return 0;
}

int  WriteDat(u8 data )
{
        IIC_Start();
        IIC_Send_Byte(OLED_ADDRESS);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(0x40);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(data);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                IIC_Stop();
                return -1;
        }
        IIC_Stop();
        return 0;
}

/**
 * @brief OLED ��ʼ��
 */

void OLED_Init(void)
{
        delay_ms(10); //�������ʱ����Ҫ

        WriteCmd(0xAE); //�ر���ʾ  ����
        WriteCmd(0x20); //�����ڴ�Ѱַģʽ
        WriteCmd(0x10); //00,ˮƽѰַģʽ;01,��ֱѰַģʽ;10,ҳ��Ѱַģʽ;11,��Ч
        WriteCmd(0xb0); //Ϊҳ��Ѱַ����ҳ����ʼ��ַ Mode,0-7
        WriteCmd(0xc8); //����ɨ�跽��
        WriteCmd(0x00); //���õ��е�ַ
        WriteCmd(0x10); //���ø��е�ַ
        WriteCmd(0x40); //���ÿ�ʼ��
        WriteCmd(0x81); //���ÿ��ƼĴ���
        WriteCmd(0xff); //���ȵ��� 0x00~0xff
        WriteCmd(0xa1); //���϶�����ӳ��0��127
        WriteCmd(0xa6); //����������ʾ
        WriteCmd(0xa8); //���ö�·������(1��64)
        WriteCmd(0x3F); //
        WriteCmd(0xa4); //0xa4,��������ڴ������;0xa5,ˮƽ����
        WriteCmd(0xd3); //������ʾ����
        WriteCmd(0x00); //������
        WriteCmd(0xd5); //������ʾʱ�ӷ�Ƶ/����Ƶ��
        WriteCmd(0xf0); //���÷���
        WriteCmd(0xd9); //--set pre-charge period
        WriteCmd(0x22); //
        WriteCmd(0xda); //����com pinӲ������
        WriteCmd(0x12);
        WriteCmd(0xdb); //--set vcomh
        WriteCmd(0x20); //0x20,0.77xVcc
        WriteCmd(0x8d); //--set DC-DC enable
        WriteCmd(0x14); //
        WriteCmd(0xaf); //��oled���
}

/**
 * @brief ������ʼ������
 */
void OLED_SetPos(u8 x, u8 y)
{ 
        WriteCmd(0xb0+y);
        WriteCmd(((x&0xf0)>>4)|0x10);
        WriteCmd((x&0x0f)|0x01);
}

/**
 * @brief ȫ�����
 */

void OLED_Fill(unsigned char fill_Data)
{
        unsigned char m,n;
        for(m=0;m<8;m++)
        {
                WriteCmd(0xb0+m);       //page0-page1
                WriteCmd(0x00);         //low column start address
                WriteCmd(0x10);         //high column start address
                for(n=0;n<128;n++)
                {
                        WriteDat(fill_Data);
                }
        }
}

void OLED_CLS(void)     //����
{
        OLED_Fill(0x00);
}
/**
 * @brief �� OLED �������л���
 */

void OLED_ON(void)
{
        WriteCmd(0X8D);  //���õ�ɱ�
        WriteCmd(0X14);  //������ɱ�
        WriteCmd(0XAF);  //OLED����
}

/**
 * @brief OLED ��������ģʽ
 */
void OLED_OFF(void)
{
        WriteCmd(0X8D);  //���õ�ɱ�
        WriteCmd(0X10);  //�رյ�ɱ�
        WriteCmd(0XAE);  //OLED����
}
/**
 * @brief ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
 * @param x ��ʼ������  ��0-127��
 * @param y ��ʼ������  ��0-7��
 * @param ch[] Ҫ��ʾ���ַ���
 * @param TextSize �ַ���С(1:6*8 ; 2:8*16)
 */
void OLED_ShowStr(u8 x, u8 y, u8 ch[], u8 TextSize)
{
        unsigned char c = 0,i = 0,j = 0;
        switch(TextSize)
        {
                case 1:
                {
                        while(ch[j] != '\0')
                        {
                                c = ch[j] - 32;
                                if(x > 126)
                                {
                                        x = 0;
                                        y++;
                                }
                                OLED_SetPos(x,y);
                                for(i=0;i<6;i++)
                                        WriteDat(F6x8[c][i]);
                                x += 6;
                                j++;
                        }
                }break;
                case 2:
                {
                        while(ch[j] != '\0')
                        {
                                c = ch[j] - 32;
                                if(x > 120)
                                {
                                        x = 0;
                                        y++;
                                }
                                OLED_SetPos(x,y);
                                for(i=0;i<8;i++)
                                        WriteDat(F8X16[c*16+i]);
                                OLED_SetPos(x,y+1);
                                for(i=0;i<8;i++)
                                        WriteDat(F8X16[c*16+i+8]);
                                x += 8;
                                j++;
                        }
                }break;
        }
}

/**
 * @brief ��ʾcodetab.h�еĺ���,16*16����
 * @param x ��ʼ������ (0-127)
 * @param y ��ʼ������ (0-7)
 * @param N:������codetab.h�е�����
 */
void OLED_ShowCN(u8 x, u8 y, u8 N)
{
        unsigned char wm=0;
        unsigned int  adder=32*N;
        OLED_SetPos(x , y);
        for(wm = 0;wm < 16;wm++)
        {
                WriteDat(F16x16[adder]);
                adder += 1;
        }
        OLED_SetPos(x,y + 1);
        for(wm = 0;wm < 16;wm++)
        {
                WriteDat(F16x16[adder]);
                adder += 1;
        }
}



