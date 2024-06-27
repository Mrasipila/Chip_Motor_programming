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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "button.h"
#include "Pololu.h"
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
BUTTON_TypeDef button;
BUTTON_TypeDef button1;
Pololu_TypeDef Pololu;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PWM(TIM_TypeDef * timer, uint8_t canal, uint32_t HCLKFrequency, uint32_t PWMFrequency,float duty_cycle);
void EXTI9_5_IRQHandler();
void EXTI4_IRQHandler();
void AlternateFunction(GPIO_TypeDef * port, uint8_t pin);
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
	Button_init(&button,GPIOC,9,LL_GPIO_PD);
	Button_init(&button1,GPIOC,10,LL_GPIO_PD);
	uint8_t tab[8] = {0,1,2,7,3,4,5,6};
	Pololu_init(&Pololu,GPIOC,tab);
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */


  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */
		RCC->APB1ENR |= 255;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	//Pololu_turnOn(&Pololu); // Turn_On Pololu driver 
	Pololu_ChoseStep(0, &Pololu); // Mode Full Step Chosen by default
	Button_enableIRQ(&button,LL_EXTI_TRIGGER_RISING_FALLING);
	Button_enableIRQ(&button1,LL_EXTI_TRIGGER_RISING_FALLING);
	//AlternateFunction(GPIOA, 8);
	PWM(TIM3,2, 16000000, 500,0.50);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
    Error_Handler();  
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_4);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(16000000);
  LL_SetSystemCoreClock(16000000);
}

/* USER CODE BEGIN 4 */

// On aurait pu utiliser un seul vecteur d'interruption en utilisant des if() 
// Pour vérifier quel pin est sujet à l'interruption. Mais on a décidé de creer
// Deux Handler pour avoir un code plus claire, lisible.
void EXTI9_5_IRQHandler(){
//s’il y une interruption en attente 
	if ( EXTI->PR&(1<<9)){
		if(Button_State(&button) & !Button_State(&button1)){
			Pololu_Dir1(&Pololu);
			Pololu_turnOn(&Pololu);
			//TIM3->CR1^=TIM_CR1_CEN_Msk;
		} else {
			Pololu_turnOff(&Pololu);
		}
//il ne faut pas oublier de réinitialiser le flag (quand on le set à 1 il se reinitialise)
		EXTI->PR|=(1<<9);
	}
}

void EXTI15_10_IRQHandler(){

		if ( EXTI->PR&(1<<10)){
			if(!Button_State(&button) & Button_State(&button1)){
				Pololu_Dir2(&Pololu);
				Pololu_turnOn(&Pololu);
			} else {
			Pololu_turnOff(&Pololu);
			}
			EXTI->PR|=(1<<10);
		}
}

void PWM(TIM_TypeDef * timer, uint8_t canal, uint32_t HCLKFrequency, uint32_t PWMFrequency,float duty_cycle){
	if(timer == TIM1 ){
		RCC->APB2ENR|= RCC_APB2ENR_TIM1EN;
	} else if (timer == TIM2){
		RCC->APB1ENR|= RCC_APB1ENR_TIM2EN;
	} else if (timer == TIM3){
		RCC->APB1ENR|= RCC_APB1ENR_TIM3EN;
	} else if (timer == TIM4){
		RCC->APB1ENR|= RCC_APB1ENR_TIM4EN;
	}
	
	// Lorsqu'on active le Timer3 sur le canal 2, il s'initialise pour le pin PC8
	// Car je le rappelle chaque Timer est lié à un ou plusieurs pins
	// particulier de la carte. 
	if (canal == 2 && timer == TIM3) {
		//AFIO->MAPR&=~(0b00<<10);
		AFIO->MAPR|=AFIO_MAPR_TIM3_REMAP_FULLREMAP;
	}
	// On definit la valeur de l'autoreload
	
	// le signal est la sortie du canal choisi (1,2,3,4) 
	// On definit le CCR (CRR) en fonction du canal
	//configuration en mode PWM mode1 (CCMR1 canaux(1,2) CCMR2 canaux (3,4))
	if(canal==1){
			timer->CCMR1&= ~TIM_CCMR1_OC1M_0;
			timer->CCMR1|= TIM_CCMR1_OC1M_1| TIM_CCMR1_OC1M_2;
			timer->CCER|= TIM_CCER_CC1E;
			timer->ARR = HCLKFrequency/(PWMFrequency + 1);
			timer->CCR1 = (timer->ARR + 1)*duty_cycle;
	} else if(canal == 2) {
			timer->CCMR1&= ~TIM_CCMR1_OC2M_0;
			timer->CCMR1|= TIM_CCMR1_OC2M_1| TIM_CCMR1_OC2M_2 | (1<<7); // (1<<7) On rend le canal 2 sensible à l'entrée ETRF		
			timer->CCER|= TIM_CCER_CC2E;
			timer->ARR = HCLKFrequency/(PWMFrequency + 1);
			timer->CCR2 = (timer->ARR + 1)*duty_cycle;
	} else if (canal == 3) {		
			timer->CCMR2&= ~TIM_CCMR2_OC3M_0;
			timer->CCMR2|= TIM_CCMR2_OC3M_1| TIM_CCMR2_OC3M_2;
			timer->CCER|= TIM_CCER_CC3E;
			timer->CCR3 = (timer->ARR + 1)*duty_cycle;
	} else if(canal == 4){	
			timer->CCMR1&= ~TIM_CCMR2_OC4M_0;
			timer->CCMR2|= TIM_CCMR2_OC4M_1| TIM_CCMR2_OC4M_2;
			timer->CCER|= TIM_CCER_CC4E;
			timer->ARR = HCLKFrequency/(PWMFrequency + 1);
			timer->CCR4 = (timer->ARR + 1)*duty_cycle;
	}
	
	//On active le main output
	timer->BDTR |= (TIM_BDTR_MOE);
	
	//On ne lance pas le timer car on veut controler le pin via le bouton 
	timer->CR1|= 1;
}

void AlternateFunction(GPIO_TypeDef * port, uint8_t pin){
	
	//Activation de l'horloge sur le port en question

	//déterminer le numéro du port 0--> GPIOA, 1-->GPIOB, etc.
	uint8_t nb_port;
	nb_port=(uint32_t)((uint32_t *)port - GPIOA_BASE)/ (uint32_t)0x400;
	//activation de l'hologe
	RCC->APB2ENR|=(1<<(nb_port+2));
	if(	pin<8){
			port->CRL &= ~(0xF << (4 * pin));	//met à 0000 les 4 cases du registre CRF[pin] et MODE[pin]
			port->CRL |= (0x9 << (4 * pin));	// pin en sortie et en fonction alternative
	} else{
			port->CRH &= ~(0xF << (4 * (pin-8)));	//met à 0000 les 4 cases du registre CRH[pin] et MODE[pin]
			port->CRH |= (0x9 << (4 * (pin-8)));	// pin en sortie et en fonction alternative
	}
}
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
