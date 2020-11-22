/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t expe;
int blue_mode = 0;
int ticks;
uint32_t counter_systick = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config_expe1(void);
void SystemClock_Config_expe2(void);
void SystemClock_Config_expe3(void);
void SystemClock_Config_expe5_6_7(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void RTC_wakeup_init( int delay);
void Systick_config (int ticks);
void RTC_RWKUP_IRQHandler();
void SysTick_Handler (void);
static void MX_TIM16_Init(void);
void RTC_wakeup_init_from_standby_or_shutdown( int delay );
void RTC_wakeup_init_from_stop( int delay );
static void RTC_wakeup_init( int delay ); //Non-utilisé
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  if(LL_RCC_LSE_IsReady()==0){
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	  LL_PWR_EnableBkUpAccess();
	  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	  MX_RTC_Init();

	  //Ecriture du premier expe
	  expe=0;
	  LL_RTC_DisableWriteProtection(RTC);
	  LL_RTC_WriteReg(RTC, BKP0R, expe);
	  LL_RTC_EnableWriteProtection(RTC);
  }
  // Initialisation en commun

  LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );
  LL_PWR_EnableBkUpAccess();

  // Initialisation des peripheriques en commun
  GPIO_init();
  MX_GPIO_Init();



  expe = LL_RTC_ReadReg(RTC,BKP0R);

  //Detection du blue_button pendant le reset et incrémentation de expe
  if (BLUE_BUTTON()){
	  expe=expe+1;
	  LL_RTC_DisableWriteProtection(RTC);
	  LL_RTC_WriteReg(RTC, BKP0R, expe);
	  LL_RTC_EnableWriteProtection(RTC);

	  if(expe==9)
		  expe=1;
  }

//Initialisation des peripheriques selon l'experience
  switch(expe)
  {
  case 1:
	  /* Configure the system clock */
	  SystemClock_Config_expe1();
	  // init systick timer (tick period at 1 ms)
	  LL_Init1msTick(SystemCoreClock);
	  /* System interrupt init*/
	  Systick_config (10);
	  break;

  case 2:
  	  /* Configure the system clock */
  	  SystemClock_Config_expe2();
  	  // init systick timer (tick period at 1 ms)
  	  MX_TIM16_Init(); // Timer pour la calibration
  	  LL_Init1msTick(SystemCoreClock);
  	  break;

  case 3:
  	  // Configure the system clock
  	  SystemClock_Config_expe3();
  	  // init systick timer (tick period at 1 ms)
  	  LL_Init1msTick(SystemCoreClock);
  	  /* System interrupt init*/
  	  Systick_config (10);
  	  break;

  case 4:
  	  // Configure the system clock
  	  SystemClock_Config_expe3();
  	  MX_TIM16_Init();
  	  // init systick timer (tick period at 1 ms)
  	  LL_Init1msTick(SystemCoreClock);
  	  break;

  	case 5:
  	  // Configure the system clock
  	  SystemClock_Config_expe5_6_7();
  	  // init systick timer (tick period at 1 ms)
  	  LL_Init1msTick(SystemCoreClock);
  	  LL_RCC_MSI_EnablePLLMode (); //Hardware auto-calibration with LSE (Attention)

  	  /* System interrupt init*/
  	  RTC_wakeup_init_from_stop(20);
  	  break;

  case 6:
  	  // Configure the system clock
  	  SystemClock_Config_expe3();
  	  // init systick timer (tick period at 1 ms)
  	  LL_Init1msTick(SystemCoreClock);
  	  LL_RCC_MSI_EnablePLLMode (); //Hardware auto-calibration with LSE (Attention)

  	  /* System interrupt init*/
  	  RTC_wakeup_init_from_stop(20);
  	  break;

  case 7:
  	  // Configure the system clock
  	  SystemClock_Config_expe3();
  	  // init systick timer (tick period at 1 ms)
  	  LL_Init1msTick(SystemCoreClock);
  	  LL_RCC_MSI_EnablePLLMode (); //Hardware auto-calibration with LSE (Attention)
  	  /* System interrupt init*/
  	  RTC_wakeup_init_from_stop(20);
  	  break;

  case 8:
  	  // Configure the system clock
  	  SystemClock_Config_expe3();
  	  // init systick timer (tick period at 1 ms)
  	  LL_Init1msTick(SystemCoreClock);
  	  LL_RCC_MSI_EnablePLLMode (); //Hardware auto-calibration with LSE (Attention)
  	  /* System interrupt init*/
  	  RTC_wakeup_init_from_standby_or_shutdown(20);
  	  break;
  }
  //expe = LL_RTC_ReadReg(RTC,BKP0R);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  if(blue_mode==1 && expe==1){
		  LL_PWR_EnableLowPowerRunMode();
		  while(LL_PWR_IsEnabledLowPowerRunMode()==0)
		  {
		  }
		  LL_PWR_EnterLowPowerRunMode();
		  LL_LPM_EnableSleep();
		  __WFI();
		  blue_mode=-1;
	  }
	  else if (blue_mode==1 && expe==2){
		  //Le LSE est deja enable
		  LL_RCC_MSI_EnablePLLMode (); //Hardware auto-calibration with LSE
	  }
	  else if(blue_mode==1 && expe==3){
		  LL_PWR_EnableLowPowerRunMode();
		  while(LL_PWR_IsEnabledLowPowerRunMode()==0)
		  {
		  }
		  LL_PWR_EnterLowPowerRunMode();
		  LL_LPM_EnableSleep();
		  __WFI();
		  blue_mode=-1;
	  }

	  else if(blue_mode==1 && expe==4){
		  LL_RCC_MSI_EnablePLLMode (); //Hardware auto-calibration with LSE
	  }

	  else if(blue_mode==1 && expe==5){
		  LL_PWR_SetPowerMode (LL_PWR_MODE_STOP0);
		  __WFI();
		  blue_mode=-1;
	  }

	  else if(blue_mode==1 && expe==6){
		  LL_PWR_SetPowerMode (LL_PWR_MODE_STOP1);
		  __WFI();
		  blue_mode=-1;
	  }

	  else if(blue_mode==1 && expe==7){
		  LL_PWR_SetPowerMode (LL_PWR_MODE_STOP2);
		  __WFI();
		  blue_mode=-1;
	  }

	  else if(blue_mode==1 && expe==8){
		  LL_PWR_SetPowerMode (LL_PWR_MODE_SHUTDOWN);
		  __WFI();
		  blue_mode=-1;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config_expe1(void)
{
	/* MSI configuration and activation */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    while (LL_FLASH_GetLatency()!=LL_FLASH_LATENCY_4)
    {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_MSI_Enable();

	// Wait till LSI is ready
	while (LL_RCC_MSI_IsReady() != 1)
	{ }

	LL_RCC_MSI_EnablePLLMode();
	LL_RCC_MSI_EnableRangeSelection();
	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
	LL_RCC_MSI_SetCalibTrimming(0);
	LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableBkUpAccess();
	LL_RCC_ForceBackupDomainReset();
	LL_RCC_ReleaseBackupDomainReset();
	LL_RCC_LSE_EnableBypass();
	LL_RCC_LSE_Enable();

	// Wait intil LSE is ready
	while(LL_RCC_LSE_IsReady() != 1 )
	{
	}
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
	LL_RCC_EnableRTC();

	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
	LL_RCC_PLL_EnableDomain_SYS();
	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1)
	{ }

	/* Sysclk activation on the main PLL */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
	}

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	/* Set APB1 & APB2 prescaler*/
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	// Pour l'interruption

	LL_Init1msTick(80000000);
	/* Update the global variable called SystemCoreClock */
	LL_SetSystemCoreClock(80000000);
	//SystemCoreClockUpdate(); // A voir
}

void SystemClock_Config_expe2(void)
{
	//Configuration du clock system sur MSI
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
	while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
	{
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_MSI_Enable();

	/* Wait till MSI is ready */
	while(LL_RCC_MSI_IsReady() != 1)
	{

	}
	LL_RCC_MSI_EnablePLLMode();
	LL_RCC_MSI_EnableRangeSelection();
	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);
	LL_RCC_MSI_SetCalibTrimming(0);
	LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_PWR); ///necessaire?
	LL_PWR_EnableBkUpAccess();
	LL_RCC_ForceBackupDomainReset();  ///necessaire?
		LL_RCC_ReleaseBackupDomainReset();  ///necessaire?
	LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
	LL_RCC_LSE_Enable();

	/* Wait till LSE is ready */
	while(LL_RCC_LSE_IsReady() != 1)
	{

	}
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
	{

	}
	LL_RCC_EnableRTC(); //necessaire?
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_Init1msTick(24000000);

	LL_SetSystemCoreClock(24000000);
}

void SystemClock_Config_expe3(void)
{
	//Configuration du clock system sur MSI
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
	{
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
	LL_RCC_MSI_Enable();

	/* Wait till MSI is ready */
	while(LL_RCC_MSI_IsReady() != 1)
	{

	}
	LL_RCC_MSI_EnablePLLMode();
	LL_RCC_MSI_EnableRangeSelection();
	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);
	LL_RCC_MSI_SetCalibTrimming(0);
	LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_PWR); ///necessaire?
	LL_PWR_EnableBkUpAccess();
	LL_RCC_ForceBackupDomainReset();  ///necessaire?
		LL_RCC_ReleaseBackupDomainReset();  ///necessaire?
	LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
	LL_RCC_LSE_Enable();

	/* Wait till LSE is ready */
	while(LL_RCC_LSE_IsReady() != 1)
	{

	}
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
	{

	}
	LL_RCC_EnableRTC(); //necessaire?
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_Init1msTick(24000000);

	LL_SetSystemCoreClock(24000000);
}

void SystemClock_Config_expe5_6_7(void){
	//Configuration du clock system sur MSI
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
	{
	}
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
	LL_RCC_MSI_Enable();

	/* Wait till MSI is ready */
	while(LL_RCC_MSI_IsReady() != 1)
	{

	}
	LL_RCC_MSI_EnablePLLMode();
	LL_RCC_MSI_EnableRangeSelection();
	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);
	LL_RCC_MSI_SetCalibTrimming(0);
	LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_PWR); ///necessaire?
	LL_PWR_EnableBkUpAccess();
	LL_RCC_ForceBackupDomainReset();  ///necessaire?
		LL_RCC_ReleaseBackupDomainReset();  ///necessaire?
	LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
	LL_RCC_LSE_Enable();

	/* Wait till LSE is ready */
	while(LL_RCC_LSE_IsReady() != 1)
	{

	}
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
	{

	}
	LL_RCC_EnableRTC(); //necessaire?
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_Init1msTick(24000000);

	LL_SetSystemCoreClock(24000000);
}

/**
  * @brief TIM16 Initialization Function Pour la calibration MSI&LSE
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM16, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM16);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_FROZEN;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(TIM16, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM16, LL_TIM_CHANNEL_CH1);
  TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
  TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
  TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
  TIM_BDTRInitStruct.DeadTime = 0;
  TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
  TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
  TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
  LL_TIM_BDTR_Init(TIM16, &TIM_BDTRInitStruct);
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**TIM16 GPIO Configuration
  PA6   ------> TIM16_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  //Verification que le module LSE est desactive i.e demarrage a froid
  if(LL_RCC_LSE_IsReady()==0){
	  LL_RCC_ForceBackupDomainReset();
	  LL_RCC_ReleaseBackupDomainReset();
	  LL_RCC_LSE_Enable();
	  while(LL_RCC_LSE_IsReady()!=1)
	  {}
  }
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* RTC interrupt Init */ // A voir apres
  NVIC_SetPriority(RTC_WKUP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(RTC_WKUP_IRQn);

  /* USER CODE BEGIN RTC_Init 1 */
  LL_RTC_DisableWriteProtection(RTC);
  LL_RTC_EnableInitMode(RTC);
  while(LL_RTC_IsActiveFlag_INIT(RTC)==0)
  {}
  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC and set the Time and Date
  */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  /** Initialize RTC and set the Time and Date
  */
  /* USER CODE BEGIN RTC_Init 2 */
  LL_RTC_DisableInitMode(RTC);
  LL_RTC_EnableWriteProtection(RTC);
  /* USER CODE END RTC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/**
  * @brief GPIO Initialization Function
  * @param ticks in ms
  * @retval None
  */
void Systick_config (int ticks)
{
	SysTick->VAL = 0;
    SysTick->LOAD =ticks-1;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    NVIC_SetPriority( SysTick_IRQn, 7 );
    LL_SYSTICK_EnableIT();
    while(!LL_SYSTICK_IsEnabledIT())
      {}
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief GPIO Initialization Function
  * @param delay in ms
  * @retval None
  */
// partie commune a toutes les utilisations du wakeup timer
static void RTC_wakeup_init( int delay )
{
	LL_RTC_DisableWriteProtection(RTC);
	LL_RTC_WAKEUP_Disable(RTC);
	while	( !LL_RTC_IsActiveFlag_WUTW(RTC))
		{ }
	// connecter le timer a l'horloge 1Hz de la RTC
	LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);
	// fixer la duree de temporisation
	LL_RTC_WAKEUP_SetAutoReload(RTC, delay);	// 16 bits
	LL_RTC_ClearFlag_WUT(RTC);
	LL_RTC_EnableIT_WUT(RTC);
	LL_RTC_WAKEUP_Enable(RTC);
	LL_RTC_EnableWriteProtection(RTC);
}

// Dans le cas des modes STANDBY et SHUTDOWN, le MPU sera reveille par reset
// causé par 1 wakeup line (interne ou externe) (le NVIC n'est plus alimenté)
void RTC_wakeup_init_from_standby_or_shutdown( int delay )
{
	RTC_wakeup_init( delay );
	// enable the Internal Wake-up line
	LL_PWR_EnableInternWU();	// ceci ne concerne que Standby et Shutdown, pas STOPx
}

// Dans le cas des modes STOPx, le MPU sera reveille par interruption
// le module EXTI et une partie du NVIC sont encore alimentes
// le contenu de la RAM et des registres étant préservé, le MPU
// reprend l'execution après l'instruction WFI

void RTC_wakeup_init_from_stop( int delay )
{
	RTC_wakeup_init( delay );
	// valider l'interrupt par la ligne 20 du module EXTI, qui est réservée au wakeup timer
	LL_EXTI_EnableIT_0_31( LL_EXTI_LINE_20 );
	LL_EXTI_EnableRisingTrig_0_31( LL_EXTI_LINE_20 );
	// valider l'interrupt chez NVIC
	NVIC_SetPriority( RTC_WKUP_IRQn, 1 );
	NVIC_EnableIRQ( RTC_WKUP_IRQn );
}

// wakeup timer interrupt Handler (inutile mais doit etre defini)
void RTC_WKUP_IRQHandler()
{
	LL_EXTI_ClearFlag_0_31( LL_EXTI_LINE_20 );
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
	if (counter_systick<5*expe)
		LED_GREEN(1);
	if (counter_systick>5*expe)
		LED_GREEN(0);
	if (counter_systick==200)
		counter_systick=0;

	counter_systick++;

	if (BLUE_BUTTON() ){
		LED_GREEN(1);
		blue_mode++;

	}
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
// wakeup timer interrupt Handler (inutile mais doit etre defini)
/*void RTC_WKUP_IRQHandler(){
	LL_EXTI_ClearFlag_0_31( LL_EXTI_LINE_20 );
}
*/

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
