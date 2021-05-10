#include "button_app.h"


u8 t=0;
u8 i=0;
u16 lastpos[5][2];		//���һ�ε�����
long long Freq_yuzhi = 0, A_yuzhi = 0;
u16 Freq_judge = 0, A_judge = 0;
u16 sweep_judge = 0;  // ɨƵ��־λ


/**
 * @brief 	��ĻUI��ʼ������
 * 
 * @param		none
 * @return	none 
 */
void bsp_GUI_Init(void)
{
	LCD_Fill(240,280,310,340, YELLOW);	
	LCD_Fill(310,280,380,340, YELLOW);
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(0, 400, 480, 400);
	
	LCD_DrawLine(0, 220, 240, 220);
	LCD_DrawLine(0, 280, 240, 280);
	LCD_DrawLine(0, 340, 240, 340);
	
	LCD_DrawLine(60, 220, 60, 400);
	LCD_DrawLine(120, 220, 120, 400);
	LCD_DrawLine(180, 220, 180, 400);
	LCD_DrawLine(240, 220, 240, 400);//��ఴ����Χ
	
	POINT_COLOR = RED;
	LCD_ShowString(25,240,200,24,24,"1");
	LCD_ShowString(85,240,200,24,24,"2");
	LCD_ShowString(145,240,200,24,24,"3");
	LCD_ShowString(25,300,200,24,24,"4");
	LCD_ShowString(85,300,200,24,24,"5");
	LCD_ShowString(145,300,200,24,24,"6");
	LCD_ShowString(25,360,200,24,24,"7");
	LCD_ShowString(85,360,200,24,24,"8");
	LCD_ShowString(145,360,200,24,24,"9");
	LCD_ShowString(185,240,200,24,24,"Back");
	LCD_ShowString(205,300,200,24,24,"0");
	LCD_ShowString(185,360,200,24,24,"Milk");//��ఴ��
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(380, 400, 480, 400);
	LCD_DrawLine(380, 340, 480, 340);
	LCD_DrawLine(380, 280, 480, 280);
	LCD_DrawLine(380, 220, 480, 220);
	LCD_DrawLine(380, 220, 380, 400);//ѡ���ΰ�����Χ
	
	POINT_COLOR = RED;
	LCD_ShowString(410,240,200,24,24,"Sin");
	LCD_ShowString(410,300,200,24,24,"Tri");
	LCD_ShowString(410,360,200,24,24,"Squa");//������ʽ����
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(240, 220, 380, 220);
	LCD_DrawLine(240, 280, 380, 280);
	LCD_DrawLine(240, 340, 380, 340);
	LCD_DrawLine(310, 220, 310, 400);//����������Χ
	
	POINT_COLOR = RED;
	LCD_ShowString(260,240,200,24,24,"UP");
	LCD_ShowString(250,300,200,24,24,"Freq");
	LCD_ShowString(250,360,200,24,24,"Down");
	LCD_ShowString(330,240,200,24,24,"UP");
	LCD_ShowString(340,300,200,24,24,"A");
	LCD_ShowString(320,360,200,24,24,"Down");//�Ҳಽ������
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(240, 170, 480, 170);
	LCD_DrawLine(240, 120, 480, 120);//�Ҳ�Ԥ��Ƶ�ʺͷ���
	
	POINT_COLOR = RED;
	LCD_ShowString(320,130,200,24,24,"Set Freq");
	LCD_ShowString(320,180,200,24,24,"Set A");//Ԥ�ð���
		
	
	POINT_COLOR = RED;
	LCD_ShowString(250,10,200,24,24,"Freq:");
	LCD_ShowString(250,70,200,24,24,"A:"); 
	LCD_ShowString(450,10,200,24,24,"Hz");
	LCD_ShowString(450,70,200,24,24,"mV");
	//��ʾ��������
	
	LCD_Fill(0,0,240,220,BLACK);//������ʾ����
	
	
	
	/******************��Ļ�°벿��***************************/
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(310, 400, 310, 460);
	LCD_DrawLine(310, 460, 480, 460);//ɨƵ��Χ
	POINT_COLOR = RED;
	LCD_ShowString(365,420,200,24,24,"SWEEP"); // ɨƵ
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(310, 460, 310, 670);//Ƶ�ʷ�ֵ������ʾ����
	POINT_COLOR = RED;
	LCD_ShowString(315,470,200,24,24,"Fs:");
	LCD_ShowString(315,540,200,24,24,"F:");
	LCD_ShowString(315,610,200,24,24,"A:");
	LCD_ShowString(450,470,200,24,24,"Hz");
	LCD_ShowString(450,540,200,24,24,"Hz");
	LCD_ShowString(450,610,200,24,24,"mV");
	POINT_COLOR = BLUE;
	LCD_ShowxNum(360,470,20000,5,24,0XFE);
	//Ƶ�ʷ�ֵ������ʾ

	LCD_Fill(0,400,310,670,BLACK);//�ɼ�������ʾ����
	
	
	POINT_COLOR = DARKBLUE;
	LCD_DrawLine(0, 670, 480, 670);//�������
	POINT_COLOR = LBBLUE;
	LCD_ShowString(10,700,360,24,24,"Designed by HEBUT Sirius");
	POINT_COLOR = CYAN;
	LCD_ShowString(340,680,240,24,24,"P X Y");
	POINT_COLOR = GRAY;
	LCD_ShowString(340,740,240,24,24,"Z Z Y");
	POINT_COLOR = GBLUE;
	LCD_ShowString(340,710,240,24,24,"S   X");
	POINT_COLOR = LGRAYBLUE;
	LCD_ShowString(340,780,240,16,16,"Milk = Enter");
	POINT_COLOR = BLUE;
}


/**
 * @brief 	������Ӧ����
 * 
 * @param		none
 * @return	none 
 */
void Button_App(void)
{
	//printf("%d %d \n", tp_dev.x[t], tp_dev.y[t]);
	
	
	if(cnt == 1){
		if(tp_dev.x[t] >= 380 && tp_dev.x[t] <= 480 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){ 
			beee();		
			LCD_Fill(0,0,240,220,BLACK);
			if(Recent_Freq >= 50000) dac1_SetSinWave_100kHz(Recent_A,Recent_Freq);
			else if(Recent_Freq < 50000) dac1_SetSinWave(Recent_A,Recent_Freq);
			wave_jugde = SIN;
		}//Sinѡ��
		else if(tp_dev.x[t] >= 380 && tp_dev.x[t] <= 480 && tp_dev.y[t] >= 280 && tp_dev.y[t] <= 340){ 
			beee();	
			LCD_Fill(0,0,240,220,BLACK);
			if(Recent_Freq >= 50000) dac1_SetTriWave_100kHz(0, Recent_A, Recent_Freq, 50);
			else if(Recent_Freq < 50000) dac1_SetTriWave(0, Recent_A, Recent_Freq, 50);
			wave_jugde = TRI;
		}//Triѡ��
		else if(tp_dev.x[t] >= 380 && tp_dev.x[t] <= 480 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			beee();	
			LCD_Fill(0,0,240,220,BLACK);
			if(Recent_Freq >= 50000) dac1_SetRectWave_100kHz(0, Recent_A, Recent_Freq, 50);
			else if(Recent_Freq < 50000) dac1_SetRectWave(0, Recent_A, Recent_Freq, 50);
			
			wave_jugde = SQU;
		}//squaѡ��
		
		
		
		//��ֵ����
		else if(tp_dev.x[t] >= 310 && tp_dev.x[t] <= 380 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){
			LCD_Fill(0,0,240,220,BLACK);
			beee();
			Recent_A += 26;
			if(Recent_A >= 4095) Recent_A = 4095;
			if(Recent_A <= 0) Recent_A = 0;
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
		}// A up
		else if(tp_dev.x[t] >= 310 && tp_dev.x[t] <= 380 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			LCD_Fill(0,0,240,220,BLACK);
			beee();
			Recent_A -= 26;
			if(Recent_A >= 4095) Recent_A = 4095;
			if(Recent_A <= 0) Recent_A = 0;
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
		}// A down
		
		
		
		//Ƶ�ʲ���
		else if(tp_dev.x[t] >= 240 && tp_dev.x[t] <= 310 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){
			LCD_Fill(0,0,240,220,BLACK);
			beee();
			Recent_Freq += 1;
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
		}// Freq up
		else if(tp_dev.x[t] >= 240 && tp_dev.x[t] <= 310 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			LCD_Fill(0,0,240,220,BLACK);  // ���²�����ʾ����
			beee();
			Recent_Freq -= 1;
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
		}// Freq down
		
		
		
		
		//Ƶ��Ԥ��
		else if(tp_dev.x[t] >= 240 && tp_dev.x[t] <= 480 && tp_dev.y[t] >= 110 && tp_dev.y[t] <= 170){
			beee();
			Freq_yuzhi = 0;
			Freq_judge = 1;
			A_judge = 0;
		}// Set Freq
		
		//��ֵԤ��
		else if(tp_dev.x[t] >= 240 && tp_dev.x[t] <= 480 && tp_dev.y[t] >= 170 && tp_dev.y[t] <= 220){
			beee();
			//printf("********************************");
			A_yuzhi = 0;
			Freq_judge = 0;
			A_judge = 1;
		} //Set A
		
		
		
		//��������
		else if(tp_dev.x[t] >= 1 && tp_dev.x[t] <= 60 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 1;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 1;
			}
		}  // 1
		else if(tp_dev.x[t] >= 60 && tp_dev.x[t] <= 120 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 2;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 2;
			}
		}  // 2
		else if(tp_dev.x[t] >= 120 && tp_dev.x[t] <= 180 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 3;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 3;
			}
		}  // 3
		else if(tp_dev.x[t] >= 180 && tp_dev.x[t] <= 240 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 280){
			beee();
			
			if(Freq_judge == 1){
				Freq_yuzhi /= 10;
			}
			else if(A_judge == 1){
				A_yuzhi /= 10;
			}
		}  // back
		else if(tp_dev.x[t] >= 1 && tp_dev.x[t] <= 60 && tp_dev.y[t] >= 280 && tp_dev.y[t] <= 340){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 4;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 4;
			}
		}  // 4
		else if(tp_dev.x[t] >= 60 && tp_dev.x[t] <= 120 && tp_dev.y[t] >= 280 && tp_dev.y[t] <= 340){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 5;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 5;
			}
		}  // 5
		else if(tp_dev.x[t] >= 120 && tp_dev.x[t] <= 180 && tp_dev.y[t] >= 280 && tp_dev.y[t] <= 340){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 6;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 6;
			}
		}  // 6
		else if(tp_dev.x[t] >= 180 && tp_dev.x[t] <= 240 && tp_dev.y[t] >= 280 && tp_dev.y[t] <= 340){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 0;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 0;
			}
		}  // 0
		else if(tp_dev.x[t] >= 1 && tp_dev.x[t] <= 60 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 7;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 7;
			}
		}  // 7
		else if(tp_dev.x[t] >= 60 && tp_dev.x[t] <= 120 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi += 8;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 8;
			}
		}  // 8
		else if(tp_dev.x[t] >= 120 && tp_dev.x[t] <= 180 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			beee();
			if(Freq_judge == 1){
				Freq_yuzhi *= 10;
				Freq_yuzhi +=9;
			}
			else if(A_judge == 1){
				A_yuzhi *= 10;
				A_yuzhi += 9;
			}
		}  // 9
		else if(tp_dev.x[t] >= 180 && tp_dev.x[t] <= 240 && tp_dev.y[t] >= 340 && tp_dev.y[t] <= 400){
			LCD_Fill(0,0,240,220,BLACK);
			beee();
			if(Freq_judge == 1) Recent_Freq = Freq_yuzhi;
			else if(A_judge == 1) Recent_A = (u16)((double)A_yuzhi/3333*4095); // nimasile
		
			Freq_judge = 0;  // �ָ�Ԥ�ñ�־λ 
			A_judge = 0;
		
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
		}  //enter  milk
		
		else if(tp_dev.x[t] >= 310 && tp_dev.x[t] <= 480 && tp_dev.y[t] >= 400 && tp_dev.y[t] <= 460){
			beee();
			if(sweep_judge == 0){
				Recent_Freq = 1; // Ƶ������Ϊ1
				sweep_judge = 1;
			}
			else if(sweep_judge == 1){
				sweep_judge = 0;
			}
			
		}  // SWEEP
		
	}//end of if(cnt == 1)
}


//�����Ļ�������Ͻ���ʾ"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//����   
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	//LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//��ʾ��������
  	POINT_COLOR=RED;//���û�����ɫ 
}

////////////////////////////////////////////////////////////////////////////////
//���ݴ�����ר�в���
//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}

//��ʵ��Բ
//x0,y0:����
//r:�뾶
//color:��ɫ
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  

//������֮��ľ���ֵ 
//x1,x2����ȡ��ֵ��������
//����ֵ��|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  

//��һ������
//(x1,y1),(x2,y2):��������ʼ����
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		gui_fill_circle(uRow,uCol,size,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
////////////////////////////////////////////////////////////////////////////////
//5�����ص����ɫ(���ݴ�������)												 
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  

/**
 * @brief 	TFT��ʼ������
 * 
 * @param		none
 * @return	none 
 */
void TFT_LCD_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ�� 
	KEY_Init(); 				//������ʼ��  
	tp_dev.init();				//��������ʼ��
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
  if(tp_dev.touchtype!=0XFF)//LCD_ShowString(30,130,200,16,16,"Press KEY0 to Adjust");//����������ʾ
	delay_ms(1500);
 	Load_Drow_Dialog();
}
