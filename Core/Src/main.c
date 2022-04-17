/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {INIT=1,START,STOP} MT_MOTION;   //Ϊö����������һ����ǩΪMT_MOTION
MT_MOTION mt_motion = INIT;                   //����һ��ö���͵ı���mt_motion   ע��ö�����ͱ��������������
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AVERVIM  5
#define BEGINVIM 65
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t uart1TX;
char RxBuffer[4];
int initial = BEGINVIM ;
int addv = AVERVIM ;
int count = 1;

//volatile uint16_t AdcRes[2];  //volatile ��ʾ����ı������ױ�ģ���ʱ����Ҫ�Ż�
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc (int ch, FILE *f)   //����ض��򵽴���1
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
    return ch;
}
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
 HAL_UART_Receive_IT(&huart1,(uint8_t *)RxBuffer,3);   //���������жϣ��ȴ�����,3���ֽڽ�һ���ж�
 
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      switch(mt_motion)
      {
          case START:
						 HAL_TIM_Base_Start_IT(&htim2);  //������ʱ��2��Y�����
					   HAL_Delay(50);
						 HAL_TIM_Base_Start(&htim6);     //������ʱ��6�Զ�����x�����ǲ�
             HAL_TIM_Base_Start_IT(&htim7);  //������ʱ��7���ж�ʹ�ܣ��������
             mt_motion = INIT;
              break;

          case INIT:
            
              break;

          case STOP:
              HAL_TIM_Base_Stop(&htim6);         //ֹͣ��X������ԭ��״̬
              HAL_TIM_Base_Stop_IT(&htim7);      //ͣ����
					    HAL_TIM_Base_Stop_IT(&htim2);      //ͣY��
              HAL_GPIO_WritePin(GPIOB, F_PORT_B_Pin|Q_PORT_B_Pin, GPIO_PIN_RESET);  //����GPIO����͵�ƽ
						  
			}
			
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
}
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)  //�����жϻص�����
{
	  
    if(strcmp(RxBuffer,"stt") == 0 )
    {
			  printf("Start to work");
        mt_motion = START;
    }
		
    else if(strcmp(RxBuffer,"stp") == 0)
    {
			  printf("Stop working");
        mt_motion = STOP;
    }
		
		else
		{
			  printf("Please select another keys");
		}
    HAL_UART_Receive_IT(&huart1,(uint8_t *)RxBuffer,3 );     //���������ж�

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    //��ʱ��7�������жϻص�����
{

	
  if (htim->Instance == htim7.Instance)
	{  
     HAL_GPIO_WritePin(GPIOB, F_PORT_B_Pin, GPIO_PIN_SET);
     for(int i = 0; i < 2002; i++); //48us   
     HAL_GPIO_WritePin(GPIOB, F_PORT_B_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, Camera_Y_Pin, GPIO_PIN_SET);
	   for(int i = 0; i < 6802; i++); //196us   
		HAL_GPIO_WritePin(GPIOC, Camera_Y_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(GPIOB, Q_PORT_B_Pin, GPIO_PIN_SET);
     for(int i = 0; i < 2002; i++); //48us   
     HAL_GPIO_WritePin(GPIOB, Q_PORT_B_Pin, GPIO_PIN_RESET);	
		
		
		
	}
	
	if (htim->Instance == htim2.Instance)
	{
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, initial);
		initial += addv;	
	}
	
	/*if (htim->Instance == htim3.Instance)
	{
		HAL_GPIO_WritePin(GPIOC, Camera_Y_Pin, GPIO_PIN_SET);
		for(int i = 0;i < 2000;i++);
		HAL_GPIO_WritePin(GPIOC, Camera_Y_Pin, GPIO_PIN_RESET);
	}*/
				
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
