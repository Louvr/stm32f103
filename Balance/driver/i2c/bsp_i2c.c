#include "bsp_i2c.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
 
/**
 * @brief ��ʼ��I2C��Ӧ�Ľӿ�����
 * @return null
 */
void IIC_Init(void)
{
        RCC->APB2ENR|=1<<3;         //��ʹ������IO PORTBʱ�� 
        GPIOB->CRH&=0XFFFFFF00;     //PB8/9 ������� 
        GPIOB->CRH|=0X00000033; 
}

/**
 * @brief ����IIC��ʼ�ź�
 * @return 1 �ɹ� 0 ʧ��
 */

int IIC_Start(void)
{
        SDA_OUT();     //sda�����
        IIC_SDA=1;
//        if(!READ_SDA)return 0; //IDR Ϊ 1
        IIC_SCL=1;
        delay_us(1);
        IIC_SDA=0;    //��ʼ�źţ�ʱ���߱��ָߵ�ƽ���䣬���������½��� 
//        if(READ_SDA)return 0; //IDR Ϊ 0
        delay_us(1);
        IIC_SCL=0;    //ǯסI2C���ߣ�׼�����ͻ�������� 
        return 1;
}

/**
 * @brief ����IICֹͣ�ź�
 */
void IIC_Stop(void)
{
        SDA_OUT();      //sda�����
        IIC_SCL=0;
        IIC_SDA=0;      //ֹͣ�źţ�ʱ���߸ߵ�ƽ���䣬��������������
        delay_us(1);
        IIC_SCL=1; 
        IIC_SDA=1;      //����I2C���߽����ź�
        delay_us(1); 
}

/**
 * @brief �ȴ�Ӧ���źŵ���
 * @return 0 ����Ӧ��ʧ�� 1 ����Ӧ��ɹ�
 */
int IIC_Wait_Ack(void)
{
        u8 ucErrTime=0;
        SDA_IN();      //SDA����Ϊ����  
        IIC_SDA=1;
        delay_us(1); 
        IIC_SCL=1;
        delay_us(1); 
        while(READ_SDA)
        {
                ucErrTime++;
                if(ucErrTime>50)
                {
                        IIC_Stop();
                        return 0;
                }
                delay_us(1);
        }
        IIC_SCL=0;//ʱ�����0  
        return 1;
} 

/**
 * @brief ����Ӧ���ź�
 */
void IIC_Ack(void)
{
        IIC_SCL=0;
        SDA_OUT();
        IIC_SDA=0;
        delay_us(1);
        IIC_SCL=1;
        delay_us(1);
        IIC_SCL=0;
}

/**
 * @brief ���� NACK Ӧ��
 */
void IIC_NAck(void)
{
        IIC_SCL=0;
        SDA_OUT();
        IIC_SDA=1;
        delay_us(1);
        IIC_SCL=1;
        delay_us(1);
        IIC_SCL=0;
}

/**
 * @brief ����һ���ֽ�
 * @param ���͵��ֽ�
 */
void IIC_Send_Byte(u8 txd)
{                        
        u8 t;   
        SDA_OUT(); 
        IIC_SCL=0;              //����ʱ�ӿ�ʼ���ݴ���
        for(t=0;t<8;t++)
        {              
                IIC_SDA=(txd&0x80)>>7;
                txd<<=1; 
                delay_us(1);   
                IIC_SCL=1;
                delay_us(1); 
                IIC_SCL=0; 
                delay_us(1);
        } 
} 
/**
 * @beief  i2c д����
 * @param addr �豸��ַ
 * @param reg �Ĵ�����ַ
 * @param len Ҫд��ĳ���
 * @param data Ҫд�������
 * @return 1 ʧ�� 0 �ɹ�
 */

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
        int i;
        if (!IIC_Start())
        return 1;
        IIC_Send_Byte(addr << 1 );
        if (!IIC_Wait_Ack()) 
        {
                IIC_Stop();
                return 1;
        }
        IIC_Send_Byte(reg);
        IIC_Wait_Ack();
        for (i = 0; i < len; i++)
        {
                IIC_Send_Byte(data[i]);
                if (!IIC_Wait_Ack())
                {
                        IIC_Stop();
                        return 0;
                }
        }
        IIC_Stop();
        return 0;
}

/**
 * @brief i2c ��ȡ����
 * @param addr �豸��ַ
 * @param reg �Ĵ�����ַ
 * @param len ��ȡ�ĳ���
 * @param buf �ݴ�����
 * @return 0 �ɹ� 1 ʧ��
 */
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
        if (!IIC_Start())
        return 1;
        IIC_Send_Byte(addr << 1);
        if (!IIC_Wait_Ack())
        {
                IIC_Stop();
                return 1;
        }
        IIC_Send_Byte(reg);
        IIC_Wait_Ack();
        IIC_Start();
        IIC_Send_Byte((addr << 1)+1);
        IIC_Wait_Ack();
        while (len)
        {
                if (len == 1)
                *buf = IIC_Read_Byte(0);
                else
                *buf = IIC_Read_Byte(1);
                buf++;
                len--;
        }
        IIC_Stop();
        return 0;
}


/**
 * @brief ��1���ֽ�
 * @param 1 ���� ASK 0 ���� NASK
 * @return ��ȡ���ֽ�
 */
u8 IIC_Read_Byte(unsigned char ack)
{
        unsigned char i,receive=0;
        SDA_IN();//SDA����Ϊ����
        for(i=0;i<8;i++ )
        {
                IIC_SCL=0; 
                delay_us(2);
                IIC_SCL=1;
                receive<<=1;
                if(READ_SDA)receive++;   
                delay_us(2); 
        } 
        if (ack)
        IIC_Ack(); //����ACK 
        else
        IIC_NAck();//����nACK  
        return receive;
}

/**
 * @brife ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
 * @param I2C_Addr i2c ���豸��ַ
 * @param addr �豸�Ĵ洢��ַ  �Ĵ�����ַ
 * @return ��ȡ����ֵ
 */ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
        unsigned char res=0;

        IIC_Start();    
        IIC_Send_Byte(I2C_Addr);        //����д����
        res++;
        IIC_Wait_Ack();
        IIC_Send_Byte(addr); res++;  //���͵�ַ
        IIC_Wait_Ack(); 
        //IIC_Stop();//����һ��ֹͣ���� 
        IIC_Start();
        IIC_Send_Byte(I2C_Addr+1); res++;    //�������ģʽ  
        IIC_Wait_Ack();
        res=IIC_Read_Byte(0);  
        IIC_Stop();//����һ��ֹͣ����

        return res;
}


/**
 * @brief ��ȡָ���豸 ָ���Ĵ����� length��ֵ
 * @param dev Ŀ���豸��ַ
 * @param reg �Ĵ�����ַ
 * @param length Ҫ�����ֽ���
 * @param *data  ���������ݽ�Ҫ��ŵ�ָ��
 * @return ���������ֽ�����
 */
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
        u8 count = 0;

        IIC_Start();
        IIC_Send_Byte(dev);     //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(reg);   //���͵�ַ
        IIC_Wait_Ack();   
        IIC_Start();
        IIC_Send_Byte(dev+1);  //�������ģʽ 
        IIC_Wait_Ack();

        for(count=0;count<length;count++)
        {

                if(count!=length-1)data[count]=IIC_Read_Byte(1);  //��ACK�Ķ�����
                else  data[count]=IIC_Read_Byte(0);             //���һ���ֽ�NACK
        }
        IIC_Stop();             //����һ��ֹͣ����
        return count;
}

/**
 * @brief ������ֽ�д��ָ���豸 ָ���Ĵ���
 * @param dev  Ŀ���豸��ַ
 * @param reg �Ĵ�����ַ
 * @param length Ҫд���ֽ���
 * @param *data  ��Ҫд�����ݵ��׵�ַ
 * @return 1 �ɹ� 0 ʧ��
 */

u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
{

        u8 count = 0;
        IIC_Start();
        IIC_Send_Byte(dev);     //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(reg);   //���͵�ַ
        IIC_Wait_Ack(); 
        for(count=0;count<length;count++)
        {
                IIC_Send_Byte(data[count]); 
                IIC_Wait_Ack(); 
        }
        IIC_Stop();     //����һ��ֹͣ����

        return 1;       //status == 0;
}

/**
 * @brief ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
 * @param dev  Ŀ���豸��ַ
 * @param reg �Ĵ�����ַ
 * @param *data  ���������ݽ�Ҫ��ŵĵ�ַ
 * @return 1 �ɹ�
 */
u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
        *data=I2C_ReadOneByte(dev, reg);
        return 1;
}

/**
 * @brief д��ָ���豸 ָ���Ĵ���һ���ֽ�
 * @param dev  Ŀ���豸��ַ
 * @param reg �Ĵ�����ַ
 * @param data  ��Ҫд����ֽ�
 * @return ��ȡ����ֵ
 */
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
        return IICwriteBytes(dev, reg, 1, &data);
}

/*
 * @brief �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
 * @param dev  Ŀ���豸��ַ
 * @param reg �Ĵ�����ַ
 * @param data  ��Ҫд����ֽ�
 * @param bitStart  Ŀ���ֽڵ���ʼλ
 * @param length   λ����
 * @return 1 �ɹ� 0 ʧ��
 */
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

        u8 b;
        if (IICreadByte(dev, reg, &b) != 0) 
        {
                u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
                data <<= (8 - length);
                data >>= (7 - bitStart);
                b &= mask;
                b |= data;
                return IICwriteByte(dev, reg, b);
        } 
        else 
        return 0;
        
}

/**
 * @brief �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
 * @param dev  Ŀ���豸��ַ
 * @param reg �Ĵ�����ַ
 * @param bitNum  Ҫ�޸�Ŀ���ֽڵ� bitNum λ
 * @param data  ��Ҫд����ֽ�
 * @return 1 �ɹ� 0 ʧ��
 */
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
        u8 b;
        IICreadByte(dev, reg, &b);
        b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
        return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
