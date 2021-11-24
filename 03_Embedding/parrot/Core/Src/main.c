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
/**
 * @brief Enum of States the Program can Take
 * */
typedef enum {
    /** @brief The Program is Waiting for user input*/
    IDLE,
    /** @brief The Program is Recording sound (max RECORD_TIME_MS ms)*/
    RECORDING,
    /** @brief The Program is Playing recorded Sound on DAC*/
    PLAYING,
    /** @brief The Program is Transmitting Recorded Sound over UART */
    TRANSMITTING
} program_state_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

/** @brief PCM Sampling frequency in Hz*/
#define SOUND_FS 32000
/** @brief PDM -> PCM Sampling frequency factor */
#define DECIMATION_FACTOR 64

/** @brief Number of PDM Sample in A Word */
#define WORD_SIZE 16

// 1ms of PDM data @SOUND_FS*DECIMATION_FACTOR in WORD of WORD_SIZE
/** @brief PDM Processing buffer size (Contain 1ms of PDM Sample)*/
#define PDM_BUFFER_SIZE ((SOUND_FS / 1000) * DECIMATION_FACTOR / WORD_SIZE)

// 1ms of PCM_DATA in word of WORD_SIZE
/** @brief PCM Processing buffer size  (Contain 1ms of PDM Sample)*/
#define PCM_BUFFER_SIZE (SOUND_FS / 1000)

// length in second of the played back sound

/** @brief Maximum length of a record in ms */
#define RECORD_TIME_MS 2985

/** @brief Linear Gain applied on PCM Samples*/
#define LINEAR_GAIN 9

/** @brief Number of PCM samples to store in the recorded sound */
#define SAMPLE_COUNT (RECORD_TIME_MS * (SOUND_FS/1000))

/** @brief Macro configuring if the record is transmitted over UART (comment to disable UART transmission)*/
#define TRANSMIT

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;
DMA_HandleTypeDef hdma_dac2;

SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/** @brief PDM Receiving Buffer */
uint16_t PDM_BUFFER[PDM_BUFFER_SIZE * 2];
/** @brief PCM Processing Buffer */
uint16_t PCM_BUFFER[PCM_BUFFER_SIZE];

/** @brief PCM Record Buffer */
uint16_t PCM_SOUND[SAMPLE_COUNT];

/** @brief Left Channel DAC transmitting Buffer */
uint16_t DAC_BUFFER_LEFT[PCM_BUFFER_SIZE * 2];
/** @brief Right Channel DAC transmitting Buffer */
uint16_t DAC_BUFFER_RIGHT[PCM_BUFFER_SIZE * 2];


/** @brief Flag lowered when SAI buffer Callbacks to indicate that the one half of the SAI buffer is ready to be read*/
uint8_t sai_flag = 1;
/** @brief Flag indicating Witch half of the SAI is ready to be read */
uint8_t sai_half = 0;
/** @brief Flag lowered when DAC buffer Callbacks to indicate that one half of the DAC buffer Has been read*/
uint8_t dac_flag = 1;
/** @brief Flag indicating Witch half of the SAI is ready to be written to */
uint8_t dac_half = 0;


#ifdef TRANSMIT
/**
 * @brief Array Storing the WAV HEADER hear with the default values for an Empty File (16 bit Integer PCM, Mono, Sampled @ SOUND_FS).
 *
 * Header is composed as such:
 * | Index | Size (Bytes) | Value                                       |
 * |:------|:-------------|:--------------------------------------------|
 * | 0     | 4            | "RIFF"                                      |
 * | 1     | 4            | File Size in Bytes (Header + samples) - 8   |
 * | 2     | 4            | "WAVE"                                      |
 * | 3     | 4            | "fmt "                                      |
 * | 4     | 4            | Number of bytes in the block - 16  (0x10)   |
 * | 5_hi  | 2            | Sample Type (1: Integer PCM, 3: floating PCM, 65534: WAVE_FORMAT_EXTENSIBLE |
 * | 5_lo  | 2            | Number of channels (1 &rarr; 6)              |
 * | 6     | 4            | Sampling Frequency                           |
 * | 7     | 4            | Byte rate (Bytes to read per second)         |
 * | 8_hi  | 2            | bits per sample                              |
 * | 8_lo  | 2            | Bytes per Block (All Channels)               |
 * | 9     | 4            | "data"                                       |
 * | 10    | 4            | Data size in bytes (Size in Bytes of the written PCM Samples)|
 */
uint32_t WAVE_HEADER[] = {
        // Declaration fichier
        ('F' << 24) + ('F' << 16) + ('I' << 8) + ('R' << 0), // 'RIFF'
        36, // file size + 44 (header) - 8
        ('E' << 24) + ('V' << 16) + ('A' << 8) + ('W' << 0), // WAVE
        (' ' << 24) + ('t' << 16) + ('m' << 8) + ('f' << 0), // 'fmt '
        0x10,
        (1 << 16) + (1 << 0), // PCM Entier + 1 canal
        SOUND_FS, // fs 32kHz
        SOUND_FS * 2, // 32kHz * 2 octet par bloc ech
        (16 << 16) + (2 << 0), // 2 octet par block d'ech et 16 bit par ech
        ('a' << 24) + ('t' << 16) + ('a' << 8) + ('d' << 0), //'data'
        0
};

#endif // TRANSMIT

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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    // Initializing Buffers
    for (size_t i = 0; i < PDM_BUFFER_SIZE * 2; i++) PDM_BUFFER[i] = 0;
    // Initializing PCM BUFFER At DAC Middle Point
    for (size_t i = 0; i < PCM_BUFFER_SIZE * 2; i++) {
        if (i < PCM_BUFFER_SIZE) PCM_BUFFER[i] = 0x7FF;
        DAC_BUFFER_LEFT[i] = 0x7FF;
        DAC_BUFFER_RIGHT[i] = 0x7FF;
    }
    for (size_t i = 0; i < SAMPLE_COUNT; i++) PCM_SOUND[i] = 0x7FF;

    //Initializing Program
    program_state_t program_state = IDLE;
    uint32_t recording_index = 0, playback_index = 0;
    uint32_t playback_start, samples_recorded = 0, cool_down = 0, idle_counter = 0;
#ifdef TRANSMIT
    uint32_t send_index;
#endif // TRANSMIT
    pdm_fir_filter_config_t fir_filter;
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
    // Starting Peripherals
    pdm_fir_flt_config_init(&fir_filter, DECIMATION_FACTOR, -50, 0X7FF, LINEAR_GAIN, 12);

    HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *) PDM_BUFFER, PDM_BUFFER_SIZE * 2);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *) DAC_BUFFER_LEFT, PCM_BUFFER_SIZE * 2, DAC_ALIGN_12B_R);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *) DAC_BUFFER_RIGHT, PCM_BUFFER_SIZE * 2, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        switch (program_state) {
            case IDLE:
                if (HAL_GPIO_ReadPin(GPIOA, USER_BTN_Pin) == GPIO_PIN_SET && cool_down == 0) {
                    program_state = RECORDING;
                    HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_RESET);
                    cool_down = 500;
                    samples_recorded = 0;
                }
                if (!sai_flag) { // filtering BUT ignoring output -> Keeping filter up to date
                    (void) pdm_fir_flt_chunk(&fir_filter, PCM_BUFFER, PDM_BUFFER + PDM_BUFFER_SIZE * sai_half,
                                             PDM_BUFFER_SIZE);
                    sai_flag = 1;
                }
                if (!dac_flag) dac_flag = 1; // resetting DAC flag
                idle_counter = (idle_counter + 1) % 1000000;
                if (cool_down > 0) {
                    cool_down--;
                    HAL_Delay(1);
                } else {
                    HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET);
                }
                break;
            case RECORDING:
                if (!sai_flag) {
                    uint32_t filtered_words = pdm_fir_flt_chunk(&fir_filter,
                                                               PCM_BUFFER,
                                                               PDM_BUFFER + PDM_BUFFER_SIZE * sai_half,
                                                               PDM_BUFFER_SIZE);
                    sai_flag = 1;
                    if (samples_recorded < SAMPLE_COUNT) samples_recorded += filtered_words;

                    //Saving Filtered Samples into SOUND
                    memcpy(PCM_SOUND + recording_index, PCM_BUFFER, sizeof(uint16_t) * filtered_words);

                    recording_index = (recording_index + filtered_words) % SAMPLE_COUNT;

                    if (cool_down > 0) cool_down--;
                }
                if (HAL_GPIO_ReadPin(GPIOA, USER_BTN_Pin) == GPIO_PIN_SET && cool_down == 0) {
                    program_state = PLAYING;
                    HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET);
                    if (samples_recorded > SAMPLE_COUNT) samples_recorded = SAMPLE_COUNT;
                    int64_t th_start = ((int64_t) recording_index - (int64_t) samples_recorded);
                    if (th_start < 0)
                        playback_start = SAMPLE_COUNT + th_start;
                    else
                        playback_start = th_start;
                }
                break;
            case PLAYING:
                if (!dac_flag) {
                    if (playback_index < recording_index) {
                        uint32_t real_idx = (playback_start + playback_index) % SAMPLE_COUNT;

                        // Copying Data to LEFT Buffer
                        memcpy(DAC_BUFFER_LEFT + dac_half * PCM_BUFFER_SIZE, PCM_SOUND + real_idx,
                               sizeof(uint16_t) * PCM_BUFFER_SIZE);

                        // Copying Data to RIGHT Buffer
                        memcpy(DAC_BUFFER_RIGHT + dac_half * PCM_BUFFER_SIZE, PCM_SOUND + real_idx,
                               sizeof(uint16_t) * PCM_BUFFER_SIZE);

                        playback_index += PCM_BUFFER_SIZE;
                    } else {
                        playback_index = 0;
                        HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_RESET);
                        for (size_t i = 0; i < PCM_BUFFER_SIZE * 2; i++) {
                            DAC_BUFFER_LEFT[i] = 0x7FF;
                            DAC_BUFFER_RIGHT[i] = 0x7FF;
                        }
#ifdef TRANSMIT
                        program_state = TRANSMITTING;
#else
                        program_state = IDLE;
#endif // TRANSMIT
                        cool_down = 250;
                        recording_index = 0;
                    }
                    dac_flag = 1;
                }
                break;
#ifdef TRANSMIT
            case TRANSMITTING:
                //Optimizing sound for 16 bit WAV Samples
                for (size_t i = 0; i < SAMPLE_COUNT; i++)
                    PCM_SOUND[i] = (int16_t) (PCM_SOUND[i] - 0x7FF) * (0x7FFF / 0x7FF);
                WAVE_HEADER[1] = 36 + samples_recorded * sizeof(uint16_t);
                WAVE_HEADER[10] = samples_recorded * sizeof(uint16_t);
                HAL_UART_Transmit(&huart1, (uint8_t *) WAVE_HEADER, 44, HAL_MAX_DELAY);
                for (size_t i = 0; i < samples_recorded; i += PCM_BUFFER_SIZE) {
                    send_index = (playback_start + i) % SAMPLE_COUNT;
                    HAL_UART_Transmit(&huart1, (uint8_t *) (PCM_SOUND + send_index), PCM_BUFFER_SIZE * sizeof(uint16_t),
                                      HAL_MAX_DELAY);
                }
                program_state = IDLE;
                break;
#endif //TRANSMIT
            default:
                break;
        }
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
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
static void MX_SAI1_Init(void)
{

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
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_DISABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.ClockSource = SAI_CLKSOURCE_PLLSAI;
  hsai_BlockA1.FrameInit.FrameLength = 64;
  hsai_BlockA1.FrameInit.ActiveFrameLength = 32;
  hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockA1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
  hsai_BlockA1.SlotInit.FirstBitOffset = 0;
  hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockA1.SlotInit.SlotNumber = 4;
  hsai_BlockA1.SlotInit.SlotActive = 0x0000FFFF;
  if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK)
  {
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
static void MX_TIM2_Init(void)
{

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
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
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
static void MX_USART1_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LD3_Pin|LD4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_BTN_Pin */
  GPIO_InitStruct.Pin = USER_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD4_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD4_Pin;
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
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
void Error_Handler(void)
{
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
