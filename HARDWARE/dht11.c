#include "dht11.h"

DHT11_DATA_Struct DHT11_DATA;

//dht11的gpio初始化
void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DHT11_CLK, ENABLE); 
														   
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	  

	GPIO_SetBits(DHT11_PORT, DHT11_PIN);
}

//dht11引脚改为输入模式
void DHT11_MODE_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
			   
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

//dht11引脚改为输出模式
void DHT11_MODE_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
			   
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//模拟输入引脚
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}


static uint8_t Read_Byte(void) 
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	   
		while(DHT11_DATA_IN==0);

		delay_us(40); 	   	  

		if(DHT11_DATA_IN==Bit_SET)  
		{
			while(DHT11_DATA_IN==Bit_SET);
			temp|=(uint8_t)(0x01<<(7-i));  
		}
		else	 
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); 
		}
	}
	return temp;
}

uint8_t Read_DHT11(DHT11_DATA_Struct *DHT11_Data)
{  	
	DHT11_MODE_OUT();   
	
	DHT11_DATA_OUT=0;   
	delay_ms(18);      

	DHT11_DATA_OUT=1;  
	delay_us(30);        

	DHT11_MODE_IN();   
   
	if(DHT11_DATA_IN==Bit_RESET)    
	{  
		while(DHT11_DATA_IN==Bit_RESET);  
		while(DHT11_DATA_IN==Bit_SET);   

		  
		DHT11_Data->humi_int= Read_Byte();
		DHT11_Data->humi_deci= Read_Byte();
		DHT11_Data->temp_int= Read_Byte();
		DHT11_Data->temp_deci= Read_Byte();
		DHT11_Data->check_sum= Read_Byte();

		DHT11_MODE_OUT();   
		DHT11_DATA_OUT=1;   

		
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}

