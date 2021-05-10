#include "dac.h"



#define TIM2_CLK 84000000
u16 DAC_Data[dot_count] = {0};	


#define dianshu 128

uint16_t g_Wave1[128];


/*  ���Ҳ����ݣ�12bit��1������128����, 0-4095֮��仯 */
const uint16_t g_SineWave128[] = {
	2047 ,  2147 ,	2248 ,	2347 ,	2446 ,	2544 ,	2641 ,	2737 ,
	2830 ,  2922 ,	3012 ,	3099 ,	3184 ,	3266 ,	3346 ,	3422 ,
	3494 ,  3564 ,	3629 ,	3691 ,	3749 ,	3803 ,	3852 ,	3897 ,
	3938 ,	3974 ,	4006 ,	4033 ,	4055 ,	4072 ,	4084 ,	4092 ,
	4094 ,	4092 ,	4084 ,	4072 ,	4055 ,	4033 ,	4006 ,	3974 ,
	3938 ,	3897 ,	3852 ,	3803 ,	3749 ,	3691 ,	3629 ,	3564 ,
	3494 ,	3422 ,	3346 ,	3266 ,	3184 ,	3099 ,	3012 ,	2922 ,
	2830 ,	2737 ,	2641 ,	2544 ,	2446 ,	2347 ,	2248 ,	2147 ,
	2047 ,	1947 ,	1846 ,	1747 ,	1648 ,	1550 ,	1453 ,	1357 ,
	1264 ,	1172 ,	1082 ,	995  ,	910  ,	828  ,	748  ,	672  ,
	600  ,	530  ,	465  ,	403  ,	345  ,	291  ,	242  ,	197  ,
	156  ,	120  ,	88   ,	61   ,	39   ,	22   ,	10   ,	2    ,
	0    ,	2    ,	10   ,	22   ,	39   ,	61   ,	88   ,	120  ,
	156  ,	197  ,	242  ,	291  ,	345  ,	403  ,  465  ,	530  ,
	600  ,	672  ,	748  ,	828  ,	910  ,	995  ,	1082 ,	1172 ,
	1264 ,	1357 ,	1453 ,	1550 ,	1648 ,	1747 ,	1846 ,	1947
};


const uint16_t g_SineWave64[] = {
	2048, 2252, 2454, 2652, 2844, 3027, 3202, 3364, 
	3514, 3649, 3768, 3870, 3954, 4019, 4065, 4090, 
	4095, 4080, 4045, 3989, 3915, 3822, 3711, 3584, 
	3441, 3285, 3116, 2937, 2748, 2553, 2353, 2150, 
	1946, 1743, 1543, 1348, 1159, 980, 811, 655, 512, 
	385, 274, 181, 107, 51, 16, 1, 6, 31, 77, 142, 
	226, 328, 447, 582, 732, 894, 1069, 1252, 1444, 
	1642, 1844, 2048
};



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




/*
*********************************************************************************************************
*	�� �� ��: bsp_InitDAC1
*	����˵��: ����PA4/DAC1
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitDAC1(void)
{	
	/* ����GPIO */
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
		/* ����DAC����Ϊģ��ģʽ  PA4 / DAC_OUT1 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}	

	/* DACͨ��1���� */
	{
		DAC_InitTypeDef DAC_InitStructure;
		
		/* ʹ��DACʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);		

		DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;	/* ѡ���������, ����޸�DAC���ݼĴ��� */
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
		//DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
		DAC_Cmd(DAC_Channel_1, ENABLE);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: dac1_InitForDMA
*	����˵��: ����PA4 ΪDAC_OUT1, ����DMA2
*	��    ��: _BufAddr : DMA���ݻ�������ַ
*			  _Count   : ��������������
*			 _DacFreq  : DAC��������Ƶ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void dac1_InitForDMA(uint32_t _BufAddr, uint32_t _Count, uint32_t _DacFreq)
{	
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	DMA_Cmd(DMA1_Stream5, DISABLE);
	DAC_DMACmd(DAC_Channel_1, DISABLE);
	TIM_Cmd(TIM6, DISABLE);
	

	/* TIM6���� */
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

		uiTIMxCLK = SystemCoreClock / 2;
		
		if (_DacFreq < 100)
		{
			usPrescaler = 10000 - 1;						 /* ��Ƶ�� = 10000 */
			usPeriod =  (uiTIMxCLK / 10000) / _DacFreq  - 1; /* �Զ���װ��ֵ */
		}
		else if (_DacFreq < 3000)
		{
			usPrescaler = 100 - 1;							/* ��Ƶ�� = 100 */
			usPeriod =  (uiTIMxCLK / 100) / _DacFreq  - 1;	/* �Զ���װ��ֵ */
		}
		else	/* ����4K��Ƶ�ʣ������Ƶ */
		{
			usPrescaler = 0;						/* ��Ƶ�� = 1 */
			usPeriod = uiTIMxCLK / _DacFreq - 1;	/* �Զ���װ��ֵ */
		}

		TIM_TimeBaseStructure.TIM_Period = usPeriod;
		TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;		/* TIM1 �� TIM8 �������� */	

		TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

		/* ѡ��TIM6��DAC�Ĵ���ʱ�� */
		TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	}

	/* DACͨ��1���� */
	{
		DAC_InitTypeDef DAC_InitStructure;
		
		/* ʹ��DACʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);		

		DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
		//DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
		DAC_Cmd(DAC_Channel_1, ENABLE);
	}

	/* DMA1_Stream5���� */
	{
		DMA_InitTypeDef DMA_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

		/* ����DMA1 Stream 5 channel 7����DAC1 */
		DMA_InitStructure.DMA_Channel = DMA_Channel_7;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1; 
		DMA_InitStructure.DMA_Memory0BaseAddr = _BufAddr;	
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;		
		DMA_InitStructure.DMA_BufferSize = _Count;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ѭ��ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream5, &DMA_InitStructure);
		DMA_Cmd(DMA1_Stream5, ENABLE);

		/* ʹ��DACͨ��1��DMA */
		DAC_DMACmd(DAC_Channel_1, ENABLE);
	}

	/* ʹ�ܶ�ʱ�� */
	TIM_Cmd(TIM6, ENABLE);
}


/*
*********************************************************************************************************
*	�� �� ��: dac1_SetRectWave
*	����˵��: DAC1�������
*	��    ��: _low  : �͵�ƽʱDAC, 
*			  _high : �ߵ�ƽʱDAC
*			  _freq : Ƶ�� Hz
*			  _duty : ռ�ձ� 2% - 98%, ���ڲ��� 1%
*	�� �� ֵ: ��
*********************************************************************************************************
*/


void dac1_SetRectWave(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty)
{	
	uint16_t i;
	//uint16_t g_Wave1[128];
	
	TIM_Cmd(TIM6, DISABLE);
	
	for (i = 0; i < (_duty * 128) / 100; i++)
	{
		g_Wave1[i] = _high;
	}
	for (; i < 128; i++)
	{
		g_Wave1[i] = _low;
	}
	
	dac1_InitForDMA((uint32_t)&g_Wave1, 128, _freq * 128);
}
void dac1_SetRectWave_100kHz(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty)
{	
	uint16_t i;
	TIM_Cmd(TIM6, DISABLE);
	for (i = 0; i < (_duty * 64) / 100; i++)
	{
		g_Wave1[i] = _high;
	}
	for (; i < 64; i++)
	{
		g_Wave1[i] = _low;
	}
	dac1_InitForDMA((uint32_t)&g_Wave1, 64, _freq * 64);
}



/*
*********************************************************************************************************
*	�� �� ��: dac1_SetSinWave
*	����˵��: DAC1������Ҳ�
*	��    ��: _vpp : ���� 0-4095;
*			  _freq : Ƶ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void dac1_SetSinWave(uint16_t _vpp, uint32_t _freq)
{
	uint32_t i;
	uint32_t dac;
	
	TIM_Cmd(TIM6, DISABLE);
		
	/* �������Ҳ����� */		
	for (i = 0; i < 128; i++)
	{
		dac = (g_SineWave128[i] * _vpp) / 4095;
		if (dac > 4095)
		{
			dac = 4095;	
		}
		g_Wave1[i] = dac;
	}
	
	dac1_InitForDMA((uint32_t)&g_Wave1, 128, _freq * 128);
}

void dac1_SetSinWave_100kHz(uint16_t _vpp, uint32_t _freq)
{
	uint32_t i;
	uint32_t dac;
	
	TIM_Cmd(TIM6, DISABLE);
		
	/* �������Ҳ����� */		
	for (i = 0; i < 64; i++)
	{
		dac = (g_SineWave64[i] * _vpp) / 4095;
		if (dac > 4095)
		{
			dac = 4095;	
		}
		g_Wave1[i] = dac;
	}
	dac1_InitForDMA((uint32_t)&g_Wave1, 64, _freq * 64);
}


/*
*********************************************************************************************************
*	�� �� ��: dac1_SetTriWave
*	����˵��: DAC1������ǲ�
*	��    ��: _low : �͵�ƽʱDAC, 
*			  _high : �ߵ�ƽʱDAC
*			  _freq : Ƶ�� Hz
*			  _duty : ռ�ձ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void dac1_SetTriWave(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty)
{	
	uint32_t i;
	uint16_t dac;
	uint16_t m;
	
	TIM_Cmd(TIM6, DISABLE);
		
	/* �������ǲ����飬128���������� _low �� _high */		
	m = (_duty * 128) / 100;
	
	if (m == 0)
	{
		m = 1;
	}
	
	if (m > 127)
	{
		m = 127;
	}
	for (i = 0; i < m; i++)
	{
		dac = _low + ((_high - _low) * i) / m;
		g_Wave1[i] = dac;
	}
	for (; i < 128; i++)
	{
		dac = _high - ((_high - _low) * (i - m)) / (128 - m);
		g_Wave1[i] = dac;
	}	
	
	dac1_InitForDMA((uint32_t)&g_Wave1, 128, _freq * 128);
}

void dac1_SetTriWave_100kHz(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty)
{	
	uint32_t i;
	uint16_t dac;
	uint16_t m;
	
	TIM_Cmd(TIM6, DISABLE);
		
	/* �������ǲ����飬64���������� _low �� _high */		
	m = (_duty * 64) / 100;
	
	if (m == 0)
	{
		m = 1;
	}
	
	if (m > 63)
	{
		m = 63;
	}
	for (i = 0; i < m; i++)
	{
		dac = _low + ((_high - _low) * i) / m;
		g_Wave1[i] = dac;
	}
	for (; i < 64; i++)
	{
		dac = _high - ((_high - _low) * (i - m)) / (64 - m);
		g_Wave1[i] = dac;
	}	
	
	dac1_InitForDMA((uint32_t)&g_Wave1, 64, _freq * 64);
}






void Show_Wave(void)
{
	LCD_Fast_DrawPoint(23, 23, YELLOW);
	int i;
	for(i = 0; i < 128; i++){  // ������
		LCD_Fast_DrawPoint(i*240/dianshu, 220-g_Wave1[i]*200/4095-10, YELLOW);
	}
}






/*****************************************************************************************************************/













//��ʱ��2��ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM2_Int_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr;//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//��������ź�
	TIM_Cmd(TIM2,ENABLE);//ʹ�ܶ�ʱ��2
}
 
//DACͨ��1�����ʼ��
void Dac1_Init(void)
{  
	//����ṹ��
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	DMA_InitTypeDef DMA_InitStructure;	
	
	//ʹ��ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//ʹ��DMA1ʱ��
	   
	//ģ��ģʽ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//PA4��ʼ��

	//DAC����
	DAC_InitType.DAC_Trigger=DAC_Trigger_T2_TRGO;//ѡ�񴥷���ʽ����ʱ��2����
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);//��ʼ��DACͨ��1	
	DAC_Cmd(DAC_Channel_1, ENABLE);//ʹ��DMA
	
	//DMA����
  //DMA1_Stream5
	DMA_DeInit(DMA1_Stream5);
	//ͨ��ѡ��
  DMA_InitStructure.DMA_Channel = DMA_Channel_7; 
	//DMA�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DAC->DHR12R1;	
	//DMA�洢��0��ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DAC_Data;  
  //DMA���䷽��---�洢��������ģʽ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	
	//����DMA�ڴ���ʱ�������ĳ���  
	DMA_InitStructure.DMA_BufferSize = dot_count;
	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  //�ڴ��ַ�Ĵ�������  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  //���������ֳ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	//������ѭ������ģʽ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //����DMAΪ�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������:��ʹ��FIFOģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  //ָ��FIFO��ֵˮƽ 
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  //ָ����Burstת�������ڴ洫��  
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  //ָ����Burstת��������Χת��  
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	//����DMA��ͨ��   
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	//ʹ��DMA
  DMA_Cmd(DMA1_Stream5, ENABLE);
	//ʹ��ADC_DMA
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

//�źŷ�������
//����������--Ƶ��--��ֵ
void Signal_Generator(u16 wf_mode, u16 v_peak, u32 frequency)
{
	u16 i;
	u32 peak;
	u32 psc;
	u32 arr;
	float temp1;
	u16 temp2;
	
	//��ֵ
	peak = v_peak*4095/3300;//��ѹת��Ϊͨ��ֵ
	
	//Ƶ��
	frequency = frequency*dot_count;
	TIM_Cmd(TIM2, DISABLE);//ʧ��TIM2
	if(frequency < 100)
	{
		psc = 9999;//��Ƶ��Ϊ10000
		arr = (TIM2_CLK/10000)/frequency-1;//�Զ�װ��ֵ
	}	
	else if(frequency < 3000)
	{
		psc = 99;//��Ƶ��Ϊ100
		arr = (TIM2_CLK/100)/frequency-1;
	}
	else
	{
		psc = 0;//��Ƶ��Ϊ1
		arr = TIM2_CLK/frequency-1;
	}
	TIM_Cmd(TIM2, ENABLE);//ʹ��TIM2
	TIM2_Int_Init(arr,psc);
	
	//����
	switch(wf_mode)
	{
		//���Ҳ�
		case sine:
		{
				temp1 = 2*3.141592/dot_count;
				for(i=0 ; i< dot_count; i++)
				{
					DAC_Data[i] = (u16)(0.5*peak*(sin(temp1 * i)+1));
				}
				break;
		}
		//���ǲ�
		case triangle:
		{
			temp2 = dot_count/2;
			for(i=0; i<temp2; i++)//��벿�ֺ���
			{
				DAC_Data[i] = (u16)(i*peak/temp2);
			}
			for(i=temp2; i<dot_count; i++)//�Ұ벿�ֺ���
			{
				DAC_Data[i] = (u16)(peak-peak*(i-temp2)/temp2);
			}
			break;
		}
		//����
		case square:
		{
			temp2 = dot_count/2;
			for(i=0; i<temp2; i++)
			{
				DAC_Data[i] = peak;
			}
			for(i=temp2; i<dot_count; i++)
			{
				DAC_Data[i] = 0;
			}
			break;
		}
		//��ݲ�
		case sawtooth:
		{
			for(i=0 ; i<dot_count; i++)
			{
				DAC_Data[i] = (u16)(peak*i/dot_count); 
			}
			break;			
		}
		default:break;
	}
}
