#include "hcsr501.h"

void HcSr501_Init(void)
{
	//定义IO初始化配置结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	//打开PF端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//配置的IO是PF7
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;		
	//IO口速度为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	
	//调用库函数GPIO_Init()配置IO
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void HcSr501Proc(void)
{
	HcSr501_Init();
	if(1==HCSR501STATUS)
	{
		printf("HCSRHIGH\r\n");
	}
	else 
		printf("HCSRLOW\r\n");
}

