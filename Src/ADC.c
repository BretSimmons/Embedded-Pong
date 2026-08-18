#include "stm32l476xx.h"

void ADC_Wakeup (void) {
	int wait_time;
	// Exit deep power down mode if still in that state
	// DEEPPWD = 0: ADC is not in deep power down
	// DEEPPWD = 1: ADC is in deep-power-down (default reset state)
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
		ADC1->CR &= ~ADC_CR_DEEPPWD;
	// Enable the ADC internal voltage regulator
	// Before performing any operations, such as launching a calibration or enabling the ADC,
	// the ADC voltage regulator must first be enabled, and the software must wait for the
	// regulator start-up time.
	ADC1->CR |= ADC_CR_ADVREGEN;
	// Wait for the ADC voltage regulator start-up time
	// The software must wait for the startup time of the ADC voltage regulator
	// (T_ADCVREG_STUP, i.e., 20 μs) before launching a calibration or enabling the ADC
	wait_time = 20 * (72000000 / 1000000);
	while (wait_time != 0) {
		wait_time--;
	}
}

void ADC_Pin_Init(void){
	// Turn on clock C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	// Set GPIOC pins (PC4) to analog mode using MODER.
	GPIOC->MODER &= ~(3U << 8);
	GPIOC->MODER |= GPIO_MODER_MODE4;
	GPIOC->ASCR |= GPIO_ASCR_ASC4;
}

void ADC_Common_Configuration(void){
	// Enable the I/O analog switches voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	// Enable conversion of internal channels
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	// Configure the ADC prescaler to have the clock not divided
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	// Select synchronous clock mode (HCLK/1) (0b01)
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ~(0b01 << ADC_CCR_CKMODE_Pos);
	// Configure all ADCs as independent
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
}

void ADC_Init(void){
	// Enable the ADC clock bit
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	// Disable ADC1 (as we can only modify the settings if it is disabled)
	ADC1->CR &= ~ADC_CR_ADEN;

	// Call other init functions
	ADC_Pin_Init();
	ADC_Common_Configuration();
	ADC_Wakeup();

	// Configure the ADC to have 12-bit resolution (0b00)
	ADC1->CFGR &= ~ADC_CFGR_RES;
	// Set right-alignment of the 12-bit result inside the result register
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	// Select 1 conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~ADC_SQR1_L;
	// Specify channel #7 as the 1st conversion by setting “7” to bits 10-6
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;
	ADC1->SQR1 |= (13U << ADC_SQR1_SQ1_Pos);
	ADC1->CFGR |= ADC_CFGR_OVRMOD;
	// Set channel #6 to be single-ended
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_13;
	// Select the ADC sampling time. Set it to “247.5 ADC clock cycles”.
	ADC1->SMPR1 |= ADC_SMPR2_SMP13;

	// Disable ADC continuous and discontinuous mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	ADC1->CFGR &= ~ADC_CFGR_DISCEN;
	// Disable Hardware Triggers
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;

	// Enable ADC1
	ADC1->CR |= ADC_CR_ADEN;
	// Wait until ADC1 is ready
	while (!(ADC1->ISR & ADC_ISR_ADRDY));
}

int Read_ADC(void) {
    // Read the ADC using software trigger
    ADC1->CR |= ADC_CR_ADSTART;

    // Wait for the End Of Conversion (EOC) flag
    while (!(ADC1->ISR & ADC_ISR_EOC));

    // Return the Data Register
    return ADC1->DR;
}
