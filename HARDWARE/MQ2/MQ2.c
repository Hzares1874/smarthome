#include "mq2.h"

u16 MQ2_Value=0;

void MQ2_Init(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE );	  //使能ADC3通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	//PF6 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
	ADC_DeInit();
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC3,&ADC_InitStructure);
	  
	ADC_Cmd(ADC3,ENABLE);
		

}	


//获得ADC值
u16 Get_Adc(void)   
{
		//设置指定ADC的规则组通道，一个序列，采样时间
		ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_144Cycles);
		
		ADC_SoftwareStartConv(ADC3);
		 
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束

		return ADC_GetConversionValue(ADC3);	//返回最近一次ADC3规则组的转换结果
}

u16 Get_Adc_Average(u8 times)
{
		u32 temp_val=0;
		u8 t;
		//printf("init1\r\n");
		for(t=0;t<times;t++)
		{
			temp_val+=Get_Adc();
			delay_ms(5);
		}
		return temp_val/times;
} 	

u16 mq2Proc(void)
{		
	int temp = 0;
		temp = Get_Adc_Average(5);
	if(temp)
	{
		MQ2_Value = temp*3.66+5000;
		//printf("mq2:%d\r\n",MQ2_Value);
		return SUCCESS;
	}
	else
		return ERROR;
}

