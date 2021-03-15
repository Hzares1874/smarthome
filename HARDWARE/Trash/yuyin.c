#include "yuyin.h"

u8 YUYINRXDATA = 0;

void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
	NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
      
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ�ܴ���2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10
   
	//��PA2  PA3����ΪUSART2����
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1	

	USART_ClearFlag(USART3, USART_FLAG_RXNE);	              //������ձ�־λ
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            //���������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //���ô���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                          //���ô���2�ж� 

	USART_Cmd(USART3, ENABLE);                                //ʹ�ܴ���2
}


void yuyinProc(void)
{
	//printf("111\r\n");
	switch(YUYINRXDATA)
	{
		case YUYINCMD: 
				USART3_SendString((u8*)"�յ�",strlen((char*)"�յ�"));
				printf("�յ�\r\n");
				YUYINRXDATA = 0;
			break;
		
		case CYLJ: 
				USART3_SendString((u8*)"��������",strlen((char*)"��������"));
				TIM_SetCompare1(TIM12,1750);
				delay_ms(3000);
				TIM_SetCompare1(TIM12,1945);
				YUYINRXDATA = 0;		
			break;
		
		case KHSLJ: 
				USART3_SendString((u8*)"�ɻ�������",strlen((char*)"�ɻ�������"));
				TIM_SetCompare2(TIM12,1750);
				delay_ms(3000);
				TIM_SetCompare2(TIM12,1945);
				YUYINRXDATA = 0;
			break;
		
		case QTLJ: 
				USART3_SendString((u8*)"��������",strlen((char*)"��������"));
				TIM_SetCompare1(TIM13,1750);
				delay_ms(3000);
				TIM_SetCompare1(TIM13,1945);
				YUYINRXDATA = 0;
			break;
		
		case YHLJ: 
				USART3_SendString((u8*)"�к�����",strlen((char*)"�к�����"));
				TIM_SetCompare1(TIM14,1750);
				delay_ms(3000);
				TIM_SetCompare1(TIM14,1945);
				YUYINRXDATA = 0;
			break;
		
		case OTHERCMD: 
			
				printf("����\r\n");
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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
	{  
		YUYINRXDATA = USART_ReceiveData(USART3);
		//printf("yuyinshuji:%x\r\n",YUYINRXDATA);
		//USART_ClearFlag(USART3,USART_FLAG_RXNE);
	}
	
} 

