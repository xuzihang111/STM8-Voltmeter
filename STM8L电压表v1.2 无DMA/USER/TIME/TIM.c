#include "TIM.H"

void TIM4_Config(u8 TIM4_Prescaler, u8 TIM4_Period)	//Ԥ��Ƶ��������ֵ
{
	TIM4_PSCR_PSC = TIM4_Prescaler;//��Ƶ
	TIM4_ARR = TIM4_Period;	//����
	TIM4_SR1 = 0X00;
	TIM4_IER_UIE = 1;
	
	TIM4_CR1_CEN = 1;
}
























