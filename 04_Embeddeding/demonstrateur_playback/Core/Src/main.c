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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pdm_fir.h"
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum program_state {
    IDLE, RECORDING, PLAYING, SENDING
} program_state;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#define SOUND_FS 32000
#define DECIMATION_FACTOR 64
#define WORD_SIZE 16

#define PDM_BUFFER_SIZE 128
#define PCM_BUFFER_SIZE PDM_BUFFER_SIZE / (DECIMATION_FACTOR / WORD_SIZE) // HERE PCM_BUFFER_SIZE = 128 / (64 / 16) = 32

#define RECORD_TIME 3 // Record Time in Seconds


//#define TRANSMIT
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t sai_flag = 1, sai_half = 0;
uint8_t dac_flag = 1, dac_half = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_DMA_Init(void);

static void MX_DAC_Init(void);

static void MX_SAI1_Init(void);

static void MX_TIM2_Init(void);

static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Configuring SAI Interruption Callbacks
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
    sai_flag = 0;
    sai_half = 0;
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
    sai_flag = 0;
    sai_half = 1;
}

// Configuring DAC Interruption Callbacks
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac1) {
    dac_flag = 0;
    dac_half = 0;
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac1) {
    dac_flag = 0;
    dac_half = 1;
}

// function to Filter a chunk of PDM_DATA
size_t filter_pdm_chunk(struct pdm_fir_filter *filter, uint16_t *pcm_buffer, uint16_t *pdm_buffer, size_t pdm_size,
                        uint16_t decimation) {
    uint16_t decimation_words = decimation / WORD_SIZE;
    size_t pcm_to_write = pdm_size / decimation_words;
    for (size_t i = 0; i < pcm_to_write; i++) {
        for (size_t j = 0; j < decimation_words; j++) {
            pdm_fir_flt_put(filter, pdm_buffer[i * decimation_words + j]);
        }
        pcm_buffer[i] = (pdm_fir_flt_get(filter, 12) + 0x7FF) & 0x0FFF;
    }
    return pcm_to_write;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
    // Declaring Initial STATE
    program_state current_state = IDLE;
    //declaring pdm_filter
    struct pdm_fir_filter pdm_filter;

    //declaring Buffers and Buffer Indexes
    uint16_t pdm_buffer[PDM_BUFFER_SIZE * 2]; // Double buffer for HALF READ
    uint16_t pcm_buffer[PCM_BUFFER_SIZE];
    uint16_t pcm_sound[SOUND_FS * RECORD_TIME];
    uint32_t pcm_sound_index = 0;

    uint32_t dac_buffer[PCM_BUFFER_SIZE * 2]; // Double buffer for HALF WRITE

    //initializing Buffers
    for (size_t i = 0; i < PDM_BUFFER_SIZE * 2; i++) pdm_buffer[i] = 0;
    for (size_t i = 0; i < PCM_BUFFER_SIZE; i++) pcm_buffer[i] = 0;
    for (size_t i = 0; i < SOUND_FS * RECORD_TIME; i++) pcm_sound[i] = 0;
    for (size_t i = 0; i < PCM_BUFFER_SIZE * 2; i++) dac_buffer[i] = 0x7ff;
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
    MX_DMA_Init();
    MX_DAC_Init();
    MX_SAI1_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */

    pdm_fir_flt_init(&pdm_filter);
    HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *) pdm_buffer, PDM_BUFFER_SIZE * 2);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *) dac_buffer, PCM_BUFFER_SIZE * 2, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim2);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    uint32_t idle_counter = 0;

    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        switch (current_state) {
            case RECORDING:
                if (!sai_flag) {
                    uint32_t filtered_words = filter_pdm_chunk(&pdm_filter,
                                                               pcm_buffer,
                                                               pdm_buffer + PDM_BUFFER_SIZE * sai_half,
                                                               PDM_BUFFER_SIZE,
                                                               DECIMATION_FACTOR);
                    sai_flag = 1;
                    if (pcm_sound_index < SOUND_FS * RECORD_TIME) {
                        memcpy(pcm_sound + pcm_sound_index, pcm_buffer, sizeof(uint16_t) * filtered_words);
                        pcm_sound_index += filtered_words;
                    } else {
                        HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_RESET);
                        pcm_sound_index = 0;
                        current_state = PLAYING;
                        HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET);
                    }
                }
                break;
            case PLAYING:
                if (!dac_flag) {
                    if (pcm_sound_index < SOUND_FS * RECORD_TIME) {
                        memcpy(dac_buffer + PCM_BUFFER_SIZE * dac_half, pcm_sound + pcm_sound_index,
                               sizeof(uint16_t) * PCM_BUFFER_SIZE);
                        pcm_sound_index += PCM_BUFFER_SIZE;
                    } else {
                        HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_RESET);
                        for (size_t i = 0; i < PCM_BUFFER_SIZE * 2; i++) dac_buffer[i] = 0x7FF;
#ifdef TRANSMIT
                        current_state = SENDING;
#else
                        current_state = IDLE;
#endif
                        pcm_sound_index = 0;
                    }
                    dac_flag = 1;
                }
                break;
            case SENDING:
                HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET);
                uint16_t samples_per_send = (SOUND_FS) / 2;  // 0.5s of samples
                uint16_t n_sends = RECORD_TIME * 2;
                for (size_t i = 0; i < n_sends; i++) { // transmit .5s of data @ 115200 Baud on USART1
                    HAL_UART_Transmit(&huart1, (uint8_t *) pcm_sound + i * samples_per_send,
                                      samples_per_send * sizeof(uint16_t), HAL_MAX_DELAY * 10);
                }
                HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_RESET);
                current_state = IDLE;
            case IDLE:
                if (HAL_GPIO_ReadPin(GPIOA, USER_BTN_Pin) == GPIO_PIN_SET) {
                    current_state = RECORDING;
                    HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                }
                if (!sai_flag) {
                    (void) filter_pdm_chunk(&pdm_filter, pcm_buffer, pdm_buffer + PDM_BUFFER_SIZE * sai_half,
                                            PDM_BUFFER_SIZE, DECIMATION_FACTOR);
                    sai_flag = 1;
                }
                if (!dac_flag) dac_flag = 1;
                idle_counter = (idle_counter + 1) % 1000000;
                break;
            default:
                idle_counter = (idle_counter + 1) % 1000000;
                HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                break;
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Macro to configure SAI1BlockA clock source selection
    */
    __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(SAI_CLKSOURCE_PLLSAI);
    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 10;
    RCC_OscInitStruct.PLL.PLLN = 90;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void) {

    /* USER CODE BEGIN DAC_Init 0 */

    /* USER CODE END DAC_Init 0 */

    DAC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN DAC_Init 1 */

    /* USER CODE END DAC_Init 1 */
    /** DAC Initialization
    */
    hdac.Instance = DAC;
    if (HAL_DAC_Init(&hdac) != HAL_OK) {
        Error_Handler();
    }
    /** DAC channel OUT1 config
    */
    sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN DAC_Init 2 */

    /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void) {

    /* USER CODE BEGIN SAI1_Init 0 */

    /* USER CODE END SAI1_Init 0 */

    /* USER CODE BEGIN SAI1_Init 1 */

    /* USER CODE END SAI1_Init 1 */
    hsai_BlockA1.Instance = SAI1_Block_A;
    hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;
    hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_RX;
    hsai_BlockA1.Init.DataSize = SAI_DATASIZE_16;
    hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;
    hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
    hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
    hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
    hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
    hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
    hsai_BlockA1.Init.ClockSource = SAI_CLKSOURCE_PLLSAI;
    hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_32K;
    hsai_BlockA1.FrameInit.FrameLength = 64;
    hsai_BlockA1.FrameInit.ActiveFrameLength = 32;
    hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
    hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
    hsai_BlockA1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
    hsai_BlockA1.SlotInit.FirstBitOffset = 0;
    hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
    hsai_BlockA1.SlotInit.SlotNumber = 4;
    hsai_BlockA1.SlotInit.SlotActive = 0x0000FFFF;
    if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SAI1_Init 2 */

    /* USER CODE END SAI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void) {

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 2249;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void) {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) {

    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    /* DMA2_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, LD3_Pin | LD4_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : USER_BTN_Pin */
    GPIO_InitStruct.Pin = USER_BTN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LD3_Pin LD4_Pin */
    GPIO_InitStruct.Pin = LD3_Pin | LD4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
