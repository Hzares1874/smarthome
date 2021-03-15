#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LightLED.h"
#include "mq2.h"
#include "hcsr501.h"
#include "exit.h"
#include "dht11.h"
#include "Commun.h"
#include "servo.h"
#include "yuyin.h"
#include "oled.h"

char *cmdLED_On = "LEDON";      //LED��
char *cmdLED_Off = "LEDOFF";    //LED�ر�

char *cmdDHT11_On = "DHTON";    //��ʪ�����ݴ����
char *cmdDHT11_Off = "DHTOFF";  //��ʪ�����ݴ���ر�

char *ledFlag = "LEDOFF";       //LED״̬
int   dhtFlag = 0;		        //��ʪ�����ݴ���״̬

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);					//��ʼ����ʱ����
	uart_init(115200);     				//��ʼ������
	//EXTI_Configuration();
	MQ2_Init();
	LDR_Init();
	DHT11_Init();
	
	Usart2_Init(115200);   //����2���ܳ�ʼ����������115200	
	TIM4_Init(500,7200);   //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	Servo_Init();
	Usart3_Init(9600);
	WiFi_ResetIO_Init();   //��ʼ��WiFi�ĸ�λIO
	IoT_Parameter_Init();  //��ʼ����IoTƽ̨MQTT�������Ĳ���
	
	OLED_Init();
	//USART3_SendString((u8*)"�յ�",strlen((char*)"�յ�"));
	//OLED_Menu_Display();
	while(1)
	{	
		mq2Proc();
		CommProc();	
		yuyinProc();
		ldr_work();
		OLED_Menu_Display();
	}
}






