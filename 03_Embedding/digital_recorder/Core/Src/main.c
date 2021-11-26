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
#include "fatfs.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pdm_fir.h"
#include "file_handling.h"
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// States the program can take
/**
 * @brief Enum of States the Program can Take
 * */
typedef enum {
    /** @brief Program Waiting for User Input */
    IDLE,
    /** @brief Program recording sound on USB*/
    RECORDING,
    /** @brief Program waiting for USb key to be inserted in OTG port*/
    WAITING_FOR_USB
} program_state_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// CLANG WARNING SUPPRESSION
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

/** @brief PCM Sampling frequency in Hz*/
#define FS 48000
/** @brief PDM -> PCM Sampling frequency factor */
#define DECIMATION_FACTOR 64

/** @brief Number of PDM Sample in A Word */
#define PDM_WORD_SIZE 16

// 1ms @ FS*DECIMATION_FACTOR (With PDM_WORD_SIZE samples per word)
/** @brief PDM Processing buffer size (Contain 1ms of PDM Sample)*/
#define PDM_BUFFER_SIZE (100*((FS / 1000) * (DECIMATION_FACTOR / PDM_WORD_SIZE)))

// 1ms @ FS (With 1 sample per word)
/** @brief PCM Processing buffer size  (Contain 1ms of PDM Sample)*/
#define PCM_BUFFER_SIZE (100*(FS/1000))

/** @brief Linear Gain applied on PCM Samples*/
#define LINEAR_GAIN 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/** @brief Flag lowered when SAI buffer Callbacks to indicate that the SAI buffer one half of the SAI buffer is ready to be read*/
uint8_t sai_flag = 1;
/** @brief Flag indicating Witch half of the SAI is ready to be read */
uint8_t sai_half = 0;

/** @brief Variable storing the state of the USB state machine (Idle, Start, Ready, Disconnect)*/
extern ApplicationTypeDef Appli_state;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_DMA_Init(void);

static void MX_SAI1_Init(void);

static void MX_USART1_UART_Init(void);

void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Configuring SAI Interruption Callbacks

/**
  * @brief Rx Transfer half completed callback. SAI DMA Buffer
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
    sai_flag = 0;
    sai_half = 0;
}

/**
  * @brief Rx Transfer completed callback.  SAI DMA Buffer
  * @param  hsai pointer to a SAI_HandleTypeDef structure that contains
  *               the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
    sai_flag = 0;
    sai_half = 1;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    uint16_t pdm_buffer[PDM_BUFFER_SIZE * 2] = {0};
    uint16_t pcm_buffer[PCM_BUFFER_SIZE] = {0}; // initializing Buffer @ 0

    program_state_t programState = WAITING_FOR_USB;
    pdm_fir_filter_config_t pdm_filter_wav;

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
    MX_SAI1_Init();
    MX_USART1_UART_Init();
    MX_FATFS_Init();
    MX_USB_HOST_Init();
    /* USER CODE BEGIN 2 */
    // Initializing FIR FILTER
    //pdm_fir_flt_init(&pdm_filter);
    pdm_fir_flt_config_init(&pdm_filter_wav, DECIMATION_FACTOR, -820, 0, LINEAR_GAIN, 16);
    // Starting Peripherals
    // Starting Microphone Capture
    HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t *) pdm_buffer, PDM_BUFFER_SIZE * 2);

    // GREETING in serial Port
    char *str_buff = malloc(sizeof(char) * 100);
    sprintf(str_buff, "+----------------------------------------------------------+\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "|                                                          |\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "|                                                          |\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "|                    STM32 USB Recorder                    |\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "|                                                          |\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "|                                                          |\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "+----------------------------------------------------------+\n\r");
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    free(str_buff);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    uint32_t idle_counter = 0, cool_down = 0, sample_count = 0, file_number = 1;
    char file_name[100];
    while (1) {
        /* USER CODE END WHILE */
        MX_USB_HOST_Process();

        /* USER CODE BEGIN 3 */
        // State Transition related to USB
        ApplicationTypeDef local_app_state = Appli_state;
        if (local_app_state == APPLICATION_READY && programState == WAITING_FOR_USB)
            programState = IDLE;
        else if (local_app_state != APPLICATION_READY)
            programState = WAITING_FOR_USB;

        // State Machine
        switch (programState) {
            case IDLE:
                // Handling Interrupts
                if (!sai_flag) {
                    (void) pdm_fir_flt_chunk(&pdm_filter_wav,
                                             pcm_buffer,
                                             pdm_buffer + sai_half * PDM_BUFFER_SIZE,
                                             PDM_BUFFER_SIZE);
                    sai_flag = 1;

                    // Updating cool down (sai flag raised every 1ms...)
                    if (cool_down > 0) cool_down--;
                }
                // Checking State transition conditions
                if (HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET && cool_down == 0) {
                    //transitioning to RECORDING
                    programState = RECORDING;
                    cool_down = 500;
                    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
                    sprintf(file_name, "REC_%08lx.wav", file_number++);
                    init_wav(file_name, FS);
                    sample_count = 0;
                }
                break;
            case RECORDING:
                // Handling Interrupts
                if (!sai_flag) {
                    HAL_GPIO_WritePin(SAI_FILTERING_GPIO_Port, SAI_FILTERING_Pin, GPIO_PIN_SET);
                    uint32_t filtered = pdm_fir_flt_chunk(&pdm_filter_wav,
                                                          pcm_buffer,
                                                          pdm_buffer + sai_half * PDM_BUFFER_SIZE,
                                                          PDM_BUFFER_SIZE);
                    sai_flag = 1;
                    HAL_GPIO_WritePin(SAI_FILTERING_GPIO_Port, SAI_FILTERING_Pin, GPIO_PIN_RESET);
                    sample_count += filtered;
                    HAL_GPIO_WritePin(USB_WRITING_GPIO_Port, USB_WRITING_Pin, GPIO_PIN_SET);
                    write_wav(pcm_buffer, filtered);
                    HAL_GPIO_WritePin(USB_WRITING_GPIO_Port, USB_WRITING_Pin, GPIO_PIN_RESET);
                    // Updating cool down (sai flag raised every 1ms...)
                    if (cool_down > 0) cool_down--;
                }
                // Checking State transition conditions
                if (HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET && cool_down == 0) {
                    //transitioning to RECORDING
                    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
                    finish_wav(file_name, sample_count);
                    programState = IDLE;
                    cool_down = 500;
                }
                break;
            case WAITING_FOR_USB:
                idle_counter = (idle_counter + 1) % 0xFFFFFF;
                break;
            default:
                Error_Handler();
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
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
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
    if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SAI1_Init 2 */

    /* USER CODE END SAI1_Init 2 */

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

    /* DMA interrupt init */
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(USB_PSO_GPIO_Port, USB_PSO_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, USB_WRITING_Pin | SAI_FILTERING_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, LD3_Pin | LD4_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : USER_BTN_Pin */
    GPIO_InitStruct.Pin = USER_BTN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_PSO_Pin */
    GPIO_InitStruct.Pin = USB_PSO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_PSO_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : USB_WRITING_Pin SAI_FILTERING_Pin */
    GPIO_InitStruct.Pin = USB_WRITING_Pin | SAI_FILTERING_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
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
