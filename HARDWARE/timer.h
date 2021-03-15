#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx.h"
extern char timer4_flag;   //外部变量声明，1:定时器4中断，有数据到

void TIM4_Init(unsigned short int, unsigned short int);
void TIM2_ENABLE_10S(void);
void TIM3_ENABLE_30S(void);
void TIM3_ENABLE_2S(void);

#endif
