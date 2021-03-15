#include "LightLED.h"

char *LEDSTATUS = "LEDOFF";
char *sendfalg = "LEDOFF";

void LDR_Init(void)
{
	//����IO��ʼ�����ýṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//��PA�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	//���õ�IO��PE8
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;		
	//IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//����Ϊͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	
	//���ÿ⺯��GPIO_Init()����IO
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;		
	//IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//����Ϊͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	LDRLEDSTATUS = 1;
}

void ldr_work(void)
{
	//LDR_Init();
	if(LDR)
	{
		LDRLEDSTATUS = 0;
		LEDSTATUS = "LEDON";
	}
	else 
	{
		LDRLEDSTATUS = 1;
		LEDSTATUS = "LEDOFF";
	}
	//SendToServer();
	if(strcmp(sendfalg,LEDSTATUS))
	{
		SendToServer();
		//strcpy(sendfalg,LEDSTATUS);
		sendfalg = LEDSTATUS;
		//printf("%s\r\n",sendfalg);
	
	}
}

