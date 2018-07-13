#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = ADC_PIN;
	
	GPIO_Init(ADC_PORT,&GPIO_InitStruct);
}

static void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef	DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	ADC_APBxClock_FUN(ADC_CLK,ENABLE);
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC_x->DR ) );
	
	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������СΪ1���������Ĵ�СӦ�õ��ڴ洢���Ĵ�С
	DMA_InitStructure.DMA_BufferSize = 1;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	
	// ����ת��ģʽ 
	ADC_InitStruct.ADC_ContinuousConvMode =  ENABLE ;
	// ת������Ҷ���
	ADC_InitStruct.ADC_DataAlign =   ADC_DataAlign_Right ;
	// �����ⲿ����ת���������������
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ; 
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStruct.ADC_Mode =  ADC_Mode_Independent ; 
	// ת��ͨ��1��
	ADC_InitStruct.ADC_NbrOfChannel = 1 ;
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStruct.ADC_ScanConvMode =  DISABLE ;
	// ��ʼ��ADC
	ADC_Init(ADC_x, &ADC_InitStruct);
	
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, 
	                         ADC_SampleTime_55Cycles5);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC_x, ENABLE);
	
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC_x));
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}	



void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADC_Mode_Config();
}









