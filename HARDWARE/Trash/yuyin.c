#include "yuyin.h"

u8 YUYINRXDATA = 0;

void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
      
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能串口2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10
   
	//将PA2  PA3复用为USART2功能
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口1	

	USART_ClearFlag(USART3, USART_FLAG_RXNE);	              //清除接收标志位
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                          //设置串口2中断 

	USART_Cmd(USART3, ENABLE);                                //使能串口2
}


void yuyinProc(void)
{
	//printf("111\r\n");
	switch(YUYINRXDATA)
	{
		case YUYINCMD: 
				USART3_SendString((u8*)"收到",strlen((char*)"收到"));
				printf("收到\r\n");
				YUYINRXDATA = 0;
			break;
		
		case CYLJ: 
				USART3_SendString((u8*)"厨余垃圾",strlen((char*)"厨余垃圾"));
				TIM_SetCompare1(TIM12,1750);
				delay_ms(3000);
				TIM_SetCompare1(TIM12,1945);
				YUYINRXDATA = 0;		
			break;
		
		case KHSLJ: 
				USART3_SendString((u8*)"可回收垃圾",strlen((char*)"可回收垃圾"));
				TIM_SetCompare2(TIM12,1750);
				delay_ms(3000);
				TIM_SetCompare2(TIM12,1945);
				YUYINRXDATA = 0;
			break;
		
		case QTLJ: 
				USART3_SendString((u8*)"其它垃圾",strlen((char*)"其它垃圾"));
				TIM_SetCompare1(TIM13,1750);
				delay_ms(3000);
				TIM_SetCompare1(TIM13,1945);
				YUYINRXDATA = 0;
			break;
		
		case YHLJ: 
				USART3_SendString((u8*)"有害垃圾",strlen((char*)"有害垃圾"));
				TIM_SetCompare1(TIM14,1750);
				delay_ms(3000);
				TIM_SetCompare1(TIM14,1945);
				YUYINRXDATA = 0;
			break;
		
		case OTHERCMD: 
			
				printf("其它\r\n");
				YUYINRXDATA = 0;
			break;
	}
}




void USART3_SendData(u8 data)
{
	while((USART3->SR&0X40)==0); 
	USART3->DR = data;
}

void USART3_SendString(u8 *DAT,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
	 	USART3_SendData(*DAT++);
	}	
}


void USART3_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
	{  
		YUYINRXDATA = USART_ReceiveData(USART3);
		//printf("yuyinshuji:%x\r\n",YUYINRXDATA);
		//USART_ClearFlag(USART3,USART_FLAG_RXNE);
	}
	
} 

