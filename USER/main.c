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

char *cmdLED_On = "LEDON";      //LED打开
char *cmdLED_Off = "LEDOFF";    //LED关闭

char *cmdDHT11_On = "DHTON";    //温湿度数据传输打开
char *cmdDHT11_Off = "DHTOFF";  //温湿度数据传输关闭

char *ledFlag = "LEDOFF";       //LED状态
int   dhtFlag = 0;		        //温湿度数据传输状态

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	//EXTI_Configuration();
	MQ2_Init();
	LDR_Init();
	DHT11_Init();
	
	Usart2_Init(115200);   //串口2功能初始化，波特率115200	
	TIM4_Init(500,7200);   //TIM4初始化，定时时间 500*7200*1000/72000000 = 50ms
	Servo_Init();
	Usart3_Init(9600);
	WiFi_ResetIO_Init();   //初始化WiFi的复位IO
	IoT_Parameter_Init();  //初始化云IoT平台MQTT服务器的参数
	
	OLED_Init();
	//USART3_SendString((u8*)"收到",strlen((char*)"收到"));
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






