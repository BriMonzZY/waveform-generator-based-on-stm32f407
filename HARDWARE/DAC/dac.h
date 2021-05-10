#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	     			    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DAC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
#define dot_count 50 //����  
#define sine      1	  //���Ҳ�
#define triangle  2	  //���ǲ�
#define square    3   //����
#define sawtooth  4   //��ݲ�

//�źŷ�������
//����������--Ƶ��--��ֵ--ռ�ձ�
void Signal_Generator(u16 wf_mode, u16 v_peak, u32 frequency);


void bsp_InitDAC1(void);
void dac1_InitForDMA(uint32_t _BufAddr, uint32_t _Count, uint32_t _DacFreq);
void dac1_SetRectWave(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty);
void dac1_SetRectWave_100kHz(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty);
void dac1_SetSinWave(uint16_t _vpp, uint32_t _freq);
void dac1_SetSinWave_100kHz(uint16_t _vpp, uint32_t _freq);
void dac1_SetTriWave(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty);
void dac1_SetTriWave_100kHz(uint16_t _low, uint16_t _high, uint32_t _freq, uint16_t _duty);

void Show_Wave(void);

#endif

















