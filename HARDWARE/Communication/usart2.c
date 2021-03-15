
/*-------------------------------------------------*/
/*                                                 */
/*          	       串口2                  	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4Xx.h"  //包含需要的头文件
#include "usart2.h"     //包含需要的头文件


char Usart2_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart2_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	


/*------------------------------------------------*/
/*函数名：初始化串口2发送功能                        */
/*参  数：bound：波特率                             */
/*返回值：无                                        */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
      
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
   
	//将PA2  PA3复用为USART2功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1	

	USART_ClearFlag(USART2, USART_FLAG_RXNE);	              //清除接收标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                          //设置串口2中断 

	USART_Cmd(USART2, ENABLE);                                //使能串口2
}

/*-------------------------------------------------*/
/*函数名：串口2 printf函数                          */
/*参  数：char* fmt,...  格式化输出字符串和参数      */
/*返回值：无                                        */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt, ...) 
{  
	unsigned int i, length;
	
	va_list ap;
	va_start(ap, fmt);
	vsprintf(USART2_TxBuff, fmt, ap);
	va_end(ap);	
	
	length=strlen((const char*)USART2_TxBuff);		
	while((USART2->SR&0X40) == 0);
	for(i = 0; i < length; i++)
	{			
		USART2->DR = USART2_TxBuff[i];
		while((USART2->SR&0X40) == 0);
	}	
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                    */
/*参  数：data：数据                                */
/*返回值：无                                        */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	int	i;	
	while((USART2->SR&0X40) == 0);
	for(i = 1; i <= (data[0] * 256 + data[1]); i++)
	{
		USART2->DR = data[i+1];
		while((USART2->SR&0X40) == 0);	
	}
}

