#ifndef _MQ2_H
#define _MQ2_H


#include "stm32f4Xx.h"
#include "delay.h"
#include "stdio.h"

extern uint16_t MQ2_Value;

void MQ2_Init(void);
uint16_t Get_Adc(void); 
uint16_t Get_Adc_Average(uint8_t times); 
uint16_t mq2Proc(void);

#endif

