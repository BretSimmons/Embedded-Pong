#include "stm32l476xx.h"
#include "system_stm32l4xx.h"

uint32_t SystemCoreClock = 16000000U;

const uint8_t AHBPrescTable[16] = {
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U
};

const uint8_t APBPrescTable[8] = {
    0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U
};

const uint32_t MSIRangeTable[12] = {
    100000U, 200000U, 400000U, 800000U,
    1000000U, 2000000U, 4000000U, 8000000U,
    16000000U, 24000000U, 32000000U, 48000000U
};

void SystemInit(void) {
    RCC->CR |= RCC_CR_HSION;

    while ((RCC->CR & RCC_CR_HSIRDY) == 0U) {
    }

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
    }
}

void SystemCoreClockUpdate(void) {
    SystemCoreClock = 16000000U;
}
