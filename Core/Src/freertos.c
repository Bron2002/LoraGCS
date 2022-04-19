/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "stdio.h"
#include "SX1278.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern SX1278_hw_t sx1278Hw;
extern SX1278_t sx1278;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LoRaTask */
osThreadId_t LoRaTaskHandle;
uint32_t myTask02Buffer[ 128 ];
osStaticThreadDef_t myTask02ControlBlock;
const osThreadAttr_t LoRaTask_attributes = {
  .name = "LoRaTask",
  .cb_mem = &myTask02ControlBlock,
  .cb_size = sizeof(myTask02ControlBlock),
  .stack_mem = &myTask02Buffer[0],
  .stack_size = sizeof(myTask02Buffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for commandConsole */
osThreadId_t commandConsoleHandle;
uint32_t commandConsoleBuffer[ 1024 ];
osStaticThreadDef_t commandConsoleControlBlock;
const osThreadAttr_t commandConsole_attributes = {
  .name = "commandConsole",
  .cb_mem = &commandConsoleControlBlock,
  .cb_size = sizeof(commandConsoleControlBlock),
  .stack_mem = &commandConsoleBuffer[0],
  .stack_size = sizeof(commandConsoleBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for printfMutex */
osMutexId_t printfMutexHandle;
osStaticMutexDef_t printfMutexControlBlock;
const osMutexAttr_t printfMutex_attributes = {
  .name = "printfMutex",
  .cb_mem = &printfMutexControlBlock,
  .cb_size = sizeof(printfMutexControlBlock),
};
/* Definitions for spi1Mutex */
osMutexId_t spi1MutexHandle;
osStaticMutexDef_t spi1MutexControlBlock;
const osMutexAttr_t spi1Mutex_attributes = {
  .name = "spi1Mutex",
  .cb_mem = &spi1MutexControlBlock,
  .cb_size = sizeof(spi1MutexControlBlock),
};
/* Definitions for eventGroup1 */
osEventFlagsId_t eventGroup1Handle;
osStaticEventGroupDef_t myEvent01ControlBlock;
const osEventFlagsAttr_t eventGroup1_attributes = {
  .name = "eventGroup1",
  .cb_mem = &myEvent01ControlBlock,
  .cb_size = sizeof(myEvent01ControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void StartLoRaTask(void *argument);
extern void vCommandConsoleTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of printfMutex */
  printfMutexHandle = osMutexNew(&printfMutex_attributes);

  /* creation of spi1Mutex */
  spi1MutexHandle = osMutexNew(&spi1Mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LoRaTask */
  LoRaTaskHandle = osThreadNew(StartLoRaTask, NULL, &LoRaTask_attributes);

  /* creation of commandConsole */
  commandConsoleHandle = osThreadNew(vCommandConsoleTask, NULL, &commandConsole_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of eventGroup1 */
  eventGroup1Handle = osEventFlagsNew(&eventGroup1_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    SX1278_init(&sx1278, 500000000,
                SX1278_POWER_11DBM,
                SX1278_LORA_SF_6,
                SX1278_LORA_BW_20_8KHZ,
                SX1278_LORA_CR_4_6,
                SX1278_LORA_CRC_EN,
                250);

    /* Infinite loop */
  for(;;)
  {
      osDelay(1000);
      GPIOC->ODR ^= 1 << 9;
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

