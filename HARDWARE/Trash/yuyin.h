#ifndef _YUYIN_H
#define _YUYIN_H

#define CYLJ 		0x02 
#define KHSLJ		0x04
#define QTLJ		0x06
#define YHLJ		0x08
#define YUYINCMD	0x01
#define OTHERCMD	0x12


#include "stm32f4xx.h"
#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "string.h"

extern uint8_t YUYINRXDATA;

void USART3_SendString(uint8_t *DAT,uint8_t len);
void yuyinProc(void);
void Usart3_Init(unsigned int bound);


#endif
