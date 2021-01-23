#include "TIM.H"

void TIM4_Config(u8 TIM4_Prescaler, u8 TIM4_Period)	//预分频器，重载值
{
	TIM4_PSCR_PSC = TIM4_Prescaler;//分频
	TIM4_ARR = TIM4_Period;	//计数
	TIM4_SR1 = 0X00;
	TIM4_IER_UIE = 1;
	
	TIM4_CR1_CEN = 1;
}
























