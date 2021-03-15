#ifndef _DHT11_H
#define _DHT11_H


#include "stm32f4Xx.h"
#include "delay.h"
#include "stdio.h"

#define HIGH  1
#define LOW   0

#define DHT11_CLK     	RCC_AHB1Periph_GPIOE
#define DHT11_PIN     	GPIO_Pin_9                  
#define DHT11_PORT		GPIOE 
#define DHT11_DATA_IN 	PEin(9)
#define DHT11_DATA_OUT 	PEout(9)
typedef struct
{
	uint8_t  humi_int;		//ʪ������λ
	uint8_t  humi_deci;	 	//ʪ��С��λ
	uint8_t  temp_int;	 	//�¶�����λ
	uint8_t  temp_deci;	 	//�¶�С��λ
	uint8_t  check_sum;	 	//У���
	
}DHT11_DATA_Struct;

extern DHT11_DATA_Struct DHT11_DATA;

void DHT11_Init(void);
uint8_t Read_DHT11(DHT11_DATA_Struct *DHT11_Data);

#endif

