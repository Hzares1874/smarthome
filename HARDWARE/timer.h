#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx.h"
extern char timer4_flag;   //�ⲿ����������1:��ʱ��4�жϣ������ݵ�

void TIM4_Init(unsigned short int, unsigned short int);
void TIM2_ENABLE_10S(void);
void TIM3_ENABLE_30S(void);
void TIM3_ENABLE_2S(void);

#endif
