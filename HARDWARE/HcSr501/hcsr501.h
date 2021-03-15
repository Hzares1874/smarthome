#ifndef _HCSR501_H
#define _HCSR501_H

#define HCSR501STATUS PFin(7)

#include "stm32f4Xx.h"
#include "delay.h"
#include "stdio.h"



void HcSr501_Init(void);
void HcSr501Proc(void);

#endif

