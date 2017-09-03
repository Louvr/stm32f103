#include "bsp_mpu6050.h"
#include "bsp_i2c.h"
#include "bsp_usart.h"
#include <math.h>




extern char Flag_Show;
extern float Acceleration_Z;     //Z����ٶȼ�
extern float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������
extern int Temperature;     //��ʾ�¶�
extern float angle, angle_dot; 


uint8_t buffer[14];


/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   �Ĵ�����ַ
  * @param   ��Ҫд�������
  * @return  null
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
        IIC_Start();
        IIC_Send_Byte(MPU6050_SLAVE_ADDRESS);
        IIC_Wait_Ack();
        IIC_Send_Byte(reg_add);
        IIC_Wait_Ack();
        IIC_Send_Byte(reg_dat);
        IIC_Wait_Ack();
        IIC_Stop();
}



/**
 * @brief   ��ʼ��MPU6050оƬ
 * @param   null
 * @return  null
 */
void MPU6050_Init(void)
{
        int i=0,j=0;
        //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
        for(i=0;i<1000;i++)
        {
                for(j=0;j<1000;j++);
        }
        MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);       //�������״̬
        MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);      //�����ǲ����ʣ�1KHz
        MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);          //��ͨ�˲��������ã���ֹƵ����1K��������5K
        MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);    //���ü��ٶȴ�����������2Gģʽ�����Լ�
        MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);      //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   �Ĵ�����ַ
  * @param   �洢����ָ��
  * @param   ��ȡ���ֽ���
  * @return   null
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
        unsigned char i;

        IIC_Start();
        IIC_Send_Byte(MPU6050_SLAVE_ADDRESS);
        IIC_Wait_Ack();
        IIC_Send_Byte(reg_add);
        IIC_Wait_Ack();

        IIC_Start();
        IIC_Send_Byte(MPU6050_SLAVE_ADDRESS+1);
        IIC_Wait_Ack();

        for(i=0;i<(num-1);i++)
        {
                *Read=IIC_Read_Byte(1);
                Read++;
        }
        *Read=IIC_Read_Byte(0);
        IIC_Stop();
}

/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
        u8 buf[6];
        MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
        accData[0] = (buf[0] << 8) | buf[1];
        accData[1] = (buf[2] << 8) | buf[3];
        accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
        u8 buf[6];
        MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
        gyroData[0] = (buf[0] << 8) | buf[1];
        gyroData[1] = (buf[2] << 8) | buf[3];
        gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief ��ȡMPU6050�����¶ȴ���������
  * @return �����¶�
  */
int Read_Temperature(void)
{ 
        float Temp;
        Temp=(I2C_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_L);
        if(Temp>32768) Temp-=65536;
            Temp=(36.53+Temp/340)*10;
        return (int)Temp;
}




/**************************************************************************
�������ܣ����׿������˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
float K1 =0.02; 
//float angle, angle_dot; 
float Q_angle=0.001;// ����������Э����
float Q_gyro=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle=0.5;// ����������Э���� �Ȳ���ƫ��
float dt=0.005;//                 
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

void Kalman_Filter(float Accel,float Gyro) 
{
        angle+=(Gyro - Q_bias) * dt;            //�������
        Pdot[0]=Q_angle - PP[0][1] - PP[1][0];  // Pk-����������Э�����΢��

        Pdot[1]=-PP[1][1];
        Pdot[2]=-PP[1][1];
        Pdot[3]=Q_gyro;
        PP[0][0] += Pdot[0] * dt;                // Pk-����������Э����΢�ֵĻ���
        PP[0][1] += Pdot[1] * dt;                // =����������Э����
        PP[1][0] += Pdot[2] * dt;
        PP[1][1] += Pdot[3] * dt;

        Angle_err = Accel - angle;      //zk-�������

        PCt_0 = C_0 * PP[0][0];
        PCt_1 = C_0 * PP[1][0];

        E = R_angle + C_0 * PCt_0;

        K_0 = PCt_0 / E;
        K_1 = PCt_1 / E;

        t_0 = PCt_0;
        t_1 = C_0 * PP[0][1];

        PP[0][0] -= K_0 * t_0;  //����������Э����
        PP[0][1] -= K_0 * t_1;
        PP[1][0] -= K_1 * t_0;
        PP[1][1] -= K_1 * t_1;

        angle   += K_0 * Angle_err;     //�������
        Q_bias  += K_1 * Angle_err;     //�������
        angle_dot   = Gyro - Q_bias;    //���ֵ(�������)��΢��=���ٶ�
}


