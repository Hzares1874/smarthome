#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f4xx.h"
#include "stdlib.h"
#include "delay.h"


#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý

#define OLED_CS_L()  GPIO_ResetBits( GPIOG,GPIO_Pin_6 ) //CS
#define OLED_CS_H()  GPIO_SetBits( GPIOB,GPIO_Pin_6)

#define OLED_RST_L() GPIO_ResetBits(GPIOG,GPIO_Pin_4)  //RST
#define OLED_RST_H() GPIO_SetBits(GPIOG,GPIO_Pin_4)

#define OLED_DC_L() GPIO_ResetBits( GPIOG,GPIO_Pin_5) //D/C
#define OLED_DC_H() GPIO_SetBits( GPIOG,GPIO_Pin_5)


#define OLED_SCLK_L() GPIO_ResetBits( GPIOG,GPIO_Pin_2 ) //CLK  D0
#define OLED_SCLK_H() GPIO_SetBits( GPIOG,GPIO_Pin_2)

#define OLED_DIN_L() GPIO_ResetBits( GPIOG,GPIO_Pin_3 ) //DIN  D1  MOSI
#define OLED_DIN_H() GPIO_SetBits( GPIOG,GPIO_Pin_3)

void OLEDIoInit(void);

void OLEDOpen(void);

void OLEDClose(void);

void OLED_DrawPoint( uint8_t x,uint8_t y,uint8_t dot );

void OLEDClear(void);

void OledFill( uint8_t x,uint8_t y,uint8_t W,uint8_t H ,uint8_t mode );

void OledInit(void);

void OledRefreshGram(void);

void OLED_ShowChar( uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode );

void OLEDDisNum( uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size );

void OLEDDrawLine( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );

void OLEDDrawRectangle( uint8_t x, uint8_t y, uint8_t W, uint8_t H );

void OLEDDisStr( uint8_t x,uint8_t y,uint8_t size,uint8_t *p );

void OLEDDisHZ( uint8_t x,uint8_t y,char *s );

#endif

