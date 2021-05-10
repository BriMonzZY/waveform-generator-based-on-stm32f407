#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "dac.h"
#include "key.h"
#include "fft.h"

/* PA4�������   ADC���ղ���*/

/* TIM6		DACͨ��1
	 TIM3   ADC������ʱ���ж�
	 TIM4   ɨƵ��ʱ���ж�
*/


u16 cnt = 0;  // ����������

double THD;
double thd1,thd2,thd3,thd4,thd5=0;

u16 display_f_a_delay_cnt = 0;  // Ƶ�ʷ�ֵ��ʾ��ʱ����λ


u16 wave_jugde = 0;  //��ǰ�����Ĳ�������
long long Recent_A = 0;  // ��ǰ��ֵ
long long Recent_Freq = 0;// ��ǰƵ��


u32 adc_get[1024];  // ADC��������
float fft_inputbuf[FFT_LENGTH*2];	//FFT��������
float fft_outputbuf[FFT_LENGTH];	//FFT�������
u16 tim_1024_judge = 0;  // TIM�������


float32_t max = 0;
u32 max_index = 0;
float32_t second = 0;
u32 second_index = 0;
float32_t third = 0;
u32 third_index = 0;
u16 Freq_FFT;  // FFT��õ�Ƶ��

float Vpp_Sin = 0, Vpp_tri = 0, Vpp_Squa =  0, VPP_ALL = 0;  // FFT ���sin��ֵ

float32_t output_tmp[1024];


int findmax(float*array,__IO uint16_t len,__IO uint16_t s)  //s��ʾ�ӵڼ�������ʼ����
{
	int i;
	int j = s;
	for(i=s;i<len;i++)
	{ 
		if(array[i]>array[j])
		{
			j=i;
		}
	}
	return j;
}


u16 index_fir;
u16 index_sec;


int main(void)
{
	u16 i;
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	BEEP_Init();  // ��������ʼ��
	beee();
	
	printf("****\n");
	TIM4_Int_Init_2(420-1, 1000-1);  // ɨƵ��ʼ��   1/1000s
	printf("****");
	TIM_Cmd(TIM4,DISABLE);  // ʧ��TIM4
	printf("****\n");
	
	Recent_A = 4095;
	Recent_Freq = 1000;
	
	
	Adc_Init();
		printf("init");
	TIM3_Int_Init_2(42-1, 100-1);  // Fs: 20kHz
		printf("init");
	arm_cfft_radix4_instance_f32 scfft;
		printf("init");
	arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1);//��ʼ��scfft�ṹ�壬�趨FFT��ز���    FFT���� λ��ת
		printf("init");
	
	
	TFT_LCD_Init();
	bsp_GUI_Init();
	//scan_touch_key();  // while(1)
	
	//dac1_SetRectWave_100kHz(0, 4095, 100000, 50);
	//dac1_SetSinWave_100kHz(4095, 100000);
	//dac1_SetTriWave_100kHz(0, 4095, 100000, 50);
	
	while(1)
	{
		tp_dev.scan(0);	
		if(tp_dev.sta&TP_PRES_DOWN)//������������
		{		
			if(tp_dev.sta&TP_PRES_DOWN)//������������
			{
				Button_App();
				cnt++;
				delay_ms(50);
			}
		}
		
		if(A_judge == 1 && Freq_judge == 0){
			LCD_ShowxNum(350,70,A_yuzhi,6,24,0XFE);
		}
		else if(Freq_judge == 1 && A_judge == 0){
			LCD_ShowxNum(350,10,Freq_yuzhi,6,24,0XFE);
		}
		else if(A_judge == 0 && Freq_judge == 0){
			LCD_ShowxNum(350,10,Recent_Freq,6,24,0XFE);
			LCD_ShowxNum(350,70,(((double)(Recent_A)/4095)*3.3*1000),6,24,0XFE);
		}
	
		
		
		
		if(!(tp_dev.sta&TP_PRES_DOWN))//���������ɿ�
		{		
			if(!(tp_dev.sta&TP_PRES_DOWN))//���������ɿ�
			{
				cnt = 0;  // cnt��0
			}
		}
		Show_Wave();
		
		/*ɨƵ*/
		if(sweep_judge == 1) TIM_Cmd(TIM4,ENABLE);  // ʹ��
		else if(sweep_judge == 0) TIM_Cmd(TIM4,DISABLE);  // ʧ��
		
		
		/*FFT����*/
		if(tim_1024_judge == 1){
			max = 0; second = 0; third = 0;
			tim_1024_judge = 0;
			TIM_Cmd(TIM3,DISABLE); //TIM3ʧ��
			for(i=0;i<FFT_LENGTH;i++){  // ������������
				fft_inputbuf[2*i]=(float)adc_get[i]/4095;	//���������ź�ʵ��
				fft_inputbuf[2*i+1]=0;//�鲿ȫ��Ϊ0
			}
			
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT���㣨��4��
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//��������������ģ�÷�ֵ 

			for(i = 0; i < 1024; i++){  // ����ת��
				output_tmp[i] = (float32_t)fft_outputbuf[i];
			}
			/*��ֵ*/
			output_tmp[0] = 0;
			arm_max_f32(output_tmp, 1024, &max, &max_index);
			output_tmp[max_index] = 0;
			output_tmp[1024-max_index] = 0;
			arm_max_f32(output_tmp, 1024, &second, &second_index);
			output_tmp[second_index] = 0;
			output_tmp[1024-second_index] = 0;
			arm_max_f32(output_tmp, 1024, &third, &third_index);
			output_tmp[third_index] = 0;
			
			/*��Ƶ��*/
			Freq_FFT = max_index*Fs/FFT_LENGTH;
			
			thd1=max;
			thd2=second;
			thd3=third;
			
			
//			index_fir=findmax(fft_outputbuf,1024,5);
//			index_sec=findmax(fft_outputbuf,1024,index_fir*1.2);
			
			//thd1 = fft_outputbuf[index_fir];
			//thd2 = fft_outputbuf[index_sec];
			
			THD=thd2/thd1;
	    THD=THD*100;
			//printf("THD: %f\n", THD);
			
			
			if(30<THD&&THD<=50){printf("sin\n");
				POINT_COLOR = RED;
				LCD_ShowString(10,750,40,40,24,"sin");
			}
			if(8<THD&&THD<=20){printf("tri\n");
				POINT_COLOR = RED;
				LCD_ShowString(10,750,40,40,24,"tri");
			}
			if(20<THD&&THD<=30){printf("squ\n");
				POINT_COLOR = RED;
				LCD_ShowString(10,750,40,40,24,"squ");
			}
			
			
			
			
			
			//printf("max: %f  max_index: %d  second: %f  third: %f", max, max_index, second, third);
			//Find_Second(fft_outputbuf,FFT_LENGTH);
			//Find_Third(fft_outputbuf,FFT_LENGTH);
			//printf("%f %d  Freq: %d  2rd:%f  3rd:%f", max, max_index, Freq_FFT , second , third);
			
			
			/*ת���ɷ�ֵ*/
			if(wave_jugde == SIN){
				Vpp_Sin = (0.1+0.013*(double)max)*1000;
				VPP_ALL = Vpp_Sin;
				//printf("Vpp_Sin: %f", Vpp_Sin);
			}
			else if(wave_jugde == SQU){  // ����
				Vpp_Squa = (0.38+0.01*(double)max)*1000;
				VPP_ALL = Vpp_Squa;
				//printf("Vpp_Squa: %f", Vpp_Squa);
			}
			else if(wave_jugde == TRI){  // ����
				Vpp_tri = (0.31+0.017*(double)max)*1000;
				VPP_ALL = Vpp_tri;
				//printf("Vpp_tri: %f", Vpp_tri);
			}
			
			//printf("\n\n\n");	
			
			
			//��Ƶ��ͼ�Ͳ���ͼ
//			for(i = 0; i < 1024; i++){
//				LCD_Fast_DrawPoint(i*310/1024, 645 - (double)adc_get[i]/4095*220, YELLOW);
//			}
			for(i = 0; i < 1024; i++){
				
				LCD_DrawLine(i*310/1024, 670, i*310/1024, 670-output_tmp[i]);							//����
			}
			LCD_Fill(0,400,310,670,BLACK);
			
			
			TIM_Cmd(TIM3,ENABLE);  // TIM3ʹ��
		}// end of if(tim_1024_judge == 1)
		
		
		/*�ɼ��Ĳ��β�����ʾ����*/
		display_f_a_delay_cnt++;
		if(display_f_a_delay_cnt >= 17){
			display_f_a_delay_cnt = 0;
			LCD_ShowxNum(360,540,Freq_FFT,6,24,0XFE);
			LCD_ShowxNum(360,610,VPP_ALL*3,6,24,0XFE);
		}
		
		
	} // end of while(1)
}

