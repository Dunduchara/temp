#include "stm32f0xx.h"
#include "delay.h"
#include "pwm.h"
#include "usart_io.h"
#include "io.h"
#include "stm32f0xx_usart.h"
#include "temp.h"
#define BOD (uint32_t) 9600
#define MAX_TEMP 500.0
#define AMPLIFY 80.0

static usart_io_t usart_writer;

void init_usart1 (void);

void change_pwm_ccr (double);

int main (void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	GPIOA->MODER |= 0xC;
	GPIOA->OSPEEDR |= 0x8;
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;
	
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0)
		;
	
	ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE);
	
	ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL)!=0){}
	

	if ((ADC1->ISR & ADC_ISR_ADRDY) != 0)
		ADC1->ISR &= ~ADC_ISR_ADRDY;
	
	ADC1->CR |= ADC_CR_ADEN;
	
	while (!(ADC1->ISR & ADC_ISR_ADRDY))
		;
	
	ADC1->CHSELR |= ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL17;
	ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
	
	ADC->CCR |= ADC_CCR_VREFEN;
		
	initTimer3Chanel1withPwm ();
	
	init_usart1 ();
	
	init_usart_io (&usart_writer, USART1, IO_WRITER);
	while (1)
	{
		ADC1->CR |= ADC_CR_ADSTART;
		while ((ADC1->ISR & ADC_ISR_EOC) == 0)
			;
		ADC1->CR |= ADC_CR_ADSTP;

		double adc_res = 3 * (double) ADC1->DR / (double) (1 << 12);
		
		double temp = get_temperature (adc_res * 1000.0 / AMPLIFY);
		change_pwm_ccr (temp);
		uprintf (&usart_writer, "temp = %d\n", (uint32_t) temp);
		delay (1000);
	}
}

void init_usart1 (void)
{
	RCC->AHBENR|= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= 0x280000;
	GPIOA->PUPDR	|= 0x280000;
	GPIOA->OSPEEDR	|= 0x140000;
	GPIOA->AFR[1] |= 0x11 << 4;
	RCC->APB2ENR	|= RCC_APB2ENR_USART1EN;
	USART1->BRR 	= 0x0341;
	USART1->CR1 	|= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void change_pwm_ccr (double temp)
{

	TIM3->CCR1 = TIM3->ARR * temp / MAX_TEMP;
}
