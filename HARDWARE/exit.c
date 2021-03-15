#include "exit.h"

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* 使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
	//SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);
	
	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

//void EXTI9_5_IRQHandler(void)
//{

//	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
//	{	
//		//printf("1\r\n");
//		EXTI_ClearITPendingBit(EXTI_Line7); /* 清除中断标志位 */		
//	}
//	
	
//	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
//	{	
//		LDRLEDSTATUS = !LDRLEDSTATUS;
//		EXTI_ClearITPendingBit(EXTI_Line8); /* 清除中断标志位 */		
//	}
//}

