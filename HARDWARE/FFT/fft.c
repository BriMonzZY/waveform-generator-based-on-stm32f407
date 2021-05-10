#include "fft.h"


/**
 * @brief 	�ҵڶ���ֵ
 * 
 * @param		fft_outputbuf[] : fft���������
 *					len : ���鳤��
 *
 * @return	none 
 */
void Find_Second(float fft_outputbuf[], u16 len)
{
	u16 i;
	for(i = 1; i < len; i++) {
		if(i == max_index) continue;
		if(fft_outputbuf[i] > second){
			second = fft_outputbuf[i];
			second_index = i;
		}
	}
}

/**
 * @brief 	�ҵ�����ֵ
 * 
 * @param		fft_outputbuf[] : fft���������
 *					len : ���鳤��
 *
 * @return	none 
 */
void Find_Third(float fft_outputbuf[], u16 len)
{
	u16 i;
	for (i = 1; i < len; i++){
		if(i == max_index || i == second_index) continue;
		if(fft_outputbuf[i] > third){
			third = fft_outputbuf[i];
			third_index = i;
		}
	}
}
