#include "adc.h"
#include "delay.h"		


/*
	ADC3  CH1    PA1
	DMA2  CH2  Stream1     DMA2_Stream1_IRQHandler
	TIM3  
*/


u16 cnt_tim = 0; // ��������




//��ʼ��ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

}	


//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}


//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}

void TIM3_Int_Init_2(u16 arr,u16 psc)   //ͨ�ö�ʱ��3��ʼ��
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //ʹ�ܼ�ʱ����APB1ʱ��
 
    TIM_TimeBaseInitStructure.TIM_Period = arr; //�Զ���װ��ֵ
	
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //��ʱ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //����
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
 
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);  //��ʼ��TIM3
 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);        //ʹ�ø����ж�
    TIM_Cmd(TIM3,ENABLE);                           //ʹ�ܶ�ʱ��
 
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;   //��ʱ�����ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //�ж���ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;    //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}




void TIM3_IRQHandler(void)  // ��ʱ���жϷ�����
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //������ж�
	{
		//u16 x = 1;
		u16 tmp = Get_Adc(5);
		adc_get[cnt_tim] = tmp;
		cnt_tim++;
		if(cnt_tim >= 1024){
			tim_1024_judge = 1;
			cnt_tim=0;
		}
		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //���ü�����
}


void wave_form(void)
{
	
}


