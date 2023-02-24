#include "main.h"
#define SYSCLOCK 72000000U

void SystemClock_Config(void);
static void MX_GPIO_Init(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  LL_GPIO_AF_DisableRemap_SWJ();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  while (1)
  {
    // blink();

    if(LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_0)) {
      LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
      LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
    } else {
      LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
      LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
    }
  }

}


void blink(){
    LL_mDelay(100);
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
    LL_mDelay(100);
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2) {}
  
  LL_RCC_HSE_Enable(); // Включаем внешний кварц
  while(LL_RCC_HSE_IsReady() != 1) {} // Ждем пока разгонится

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9); // /1 *9 - первый делитель/умножитель частоты
  LL_RCC_PLL_Enable(); // врубаем его
  while(LL_RCC_PLL_IsReady() != 1) {} // ждем стабилизации

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // ставим предделители на шину ahb
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL); // выставляем источником тактирования наш умножитель

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {} // дожидаемся выставления частот

  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
}

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

// Настройка входных пинов:
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);

// Настройка выходных пинов:
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_MEDIUM);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif 
