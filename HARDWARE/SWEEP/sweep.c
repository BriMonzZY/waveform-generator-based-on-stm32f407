#include "sweep.h"


/*
********************************************************************************
 
system_stm32f4xx.c �ļ��� voidSetSysClock(void) ������ʱ�ӵ��������£�

HCLK = SYSCLK / 1     (AHB1Periph)
 
PCLK2 = HCLK / 2      (APB2Periph)
 
PCLK1 = HCLK / 4      (APB1Periph)
 
��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock/ 2;
 
��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 =SystemCoreClock;
 
APB1 ��ʱ���� TIM2, TIM3,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
 
APB2 ��ʱ���� TIM1, TIM8,TIM9, TIM10, TIM11
 
TIM ���������� = TIMCLK / ��TIM_Period + 1��/��TIM_Prescaler+ 1��
********************************************************************************
*/

/**
 * @brief 	ɨƵ��ʱ���жϳ�ʼ������
 * 
 * @param		arr
 *					psc
 *
 * @return	none 
 */
void TIM4_Int_Init_2(u16 arr,u16 psc)   //ͨ�ö�ʱ��3��ʼ��   84MHz
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;  
    NVIC_InitTypeDef NVIC_InitStruct;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //ʹ�ܼ�ʱ����APB1ʱ��
 
		TIM_TimeBaseInitStructure.TIM_Period=arr; //�����Զ���װֵ����ֵ���ⲿ����
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //���ö�ʱ��Ԥ��Ƶ
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //����ʱ�ӷ�Ƶ������Ϊ1��Ƶ��
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ü���ģʽ�����ϼ�����
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //ʹ���ж�
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //��������жϱ�־λ
		
		TIM_Cmd(TIM4,ENABLE);

		NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn; //�����ж����ȼ���ʹ��
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStruct);

}

/**
 * @brief 	TIM4�жϷ�����
 * 
 * @param		none
 * @return	none 
 */
void TIM4_IRQHandler(void)  // ��ʱ���жϷ�����
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //������ж�
	{
		//printf("tim2\n");
		Recent_Freq++;
		if(Recent_Freq >= 1000) Recent_Freq = 1;
		
		if(wave_jugde == SIN){
			if(Recent_Freq >= 50000) dac1_SetSinWave_100kHz(Recent_A,Recent_Freq);
			else if(Recent_Freq < 50000) dac1_SetSinWave(Recent_A,Recent_Freq);
		}
		else if(wave_jugde == TRI){
			if(Recent_Freq >= 50000) dac1_SetTriWave_100kHz(0, Recent_A, Recent_Freq, 50);
			else if(Recent_Freq < 50000) dac1_SetTriWave(0, Recent_A, Recent_Freq, 50);
		}
		else {
			if(Recent_Freq >= 50000) dac1_SetRectWave_100kHz(0, Recent_A, Recent_Freq, 50);
			else if(Recent_Freq < 50000) dac1_SetRectWave(0, Recent_A, Recent_Freq, 50);
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //���ü�����
}
