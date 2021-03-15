#include "hcsr501.h"

void HcSr501_Init(void)
{
	//����IO��ʼ�����ýṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//��PF�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//���õ�IO��PF7
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;		
	//IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	
	//���ÿ⺯��GPIO_Init()����IO
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

