/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_uart_ex.h"
#include "string.h"
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct Pin {
  GPIO_TypeDef* Letter;
  uint16_t Number;
  uint16_t index;
} Pin; 

typedef enum State {
  Start,
  Configure_Pins,
  Echo,
  Continue,
  Default,
} State;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// #define Debug
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USER CODE BEGIN PV */
#define GPIO_PINS_Len 72
Pin Pins[GPIO_PINS_Len] = {
    {.index=0, .Letter = GPIOE, .Number = GPIO_PIN_14},
    {.index=1, .Letter = GPIOE, .Number = GPIO_PIN_15},
    {.index=2, .Letter = GPIOE, .Number = GPIO_PIN_13},
    {.index=3, .Letter = GPIOF, .Number = GPIO_PIN_13},
    {.index=4, .Letter = GPIOF, .Number = GPIO_PIN_12},
    {.index=5, .Letter = GPIOG, .Number = GPIO_PIN_14},
    {.index=6, .Letter = GPIOG, .Number = GPIO_PIN_4},
    {.index=7, .Letter = GPIOG, .Number = GPIO_PIN_6},
    {.index=8, .Letter = GPIOG, .Number = GPIO_PIN_5},
    {.index=9, .Letter = GPIOG, .Number = GPIO_PIN_8},
    {.index=10, .Letter = GPIOE, .Number = GPIO_PIN_0},
    {.index=11, .Letter = GPIOF, .Number = GPIO_PIN_11},
    {.index=12, .Letter = GPIOF, .Number = GPIO_PIN_15},
    {.index=13, .Letter = GPIOF, .Number = GPIO_PIN_3},
    {.index=14, .Letter = GPIOE, .Number = GPIO_PIN_12},
    {.index=15, .Letter = GPIOE, .Number = GPIO_PIN_10},
    {.index=16, .Letter = GPIOD, .Number = GPIO_PIN_11},
    {.index=17, .Letter = GPIOD, .Number = GPIO_PIN_12},
    {.index=18, .Letter = GPIOD, .Number = GPIO_PIN_13},
    {.index=19, .Letter = GPIOA, .Number = GPIO_PIN_3},
    {.index=20, .Letter = GPIOB, .Number = GPIO_PIN_10},
    {.index=21, .Letter = GPIOF, .Number = GPIO_PIN_10},
    {.index=22, .Letter = GPIOE, .Number = GPIO_PIN_7},
    {.index=23, .Letter = GPIOD, .Number = GPIO_PIN_14},
    {.index=24, .Letter = GPIOD, .Number = GPIO_PIN_15},
    {.index=25, .Letter = GPIOF, .Number = GPIO_PIN_14},
    {.index=26, .Letter = GPIOE, .Number = GPIO_PIN_9},
    {.index=27, .Letter = GPIOE, .Number = GPIO_PIN_11},
    {.index=28, .Letter = GPIOE, .Number = GPIO_PIN_5},
    {.index=29, .Letter = GPIOE, .Number = GPIO_PIN_4},
    {.index=30, .Letter = GPIOE, .Number = GPIO_PIN_2},
    {.index=31, .Letter = GPIOG, .Number = GPIO_PIN_3},
    {.index=32, .Letter = GPIOG, .Number = GPIO_PIN_2},
    {.index=33, .Letter = GPIOD, .Number = GPIO_PIN_3},
    {.index=34, .Letter = GPIOC, .Number = GPIO_PIN_0},
    {.index=35, .Letter = GPIOA, .Number = GPIO_PIN_4},
    {.index=36, .Letter = GPIOD, .Number = GPIO_PIN_4},
    {.index=37, .Letter = GPIOD, .Number = GPIO_PIN_5},
    {.index=38, .Letter = GPIOD, .Number = GPIO_PIN_6},
    {.index=39, .Letter = GPIOD, .Number = GPIO_PIN_7},
    {.index=40, .Letter = GPIOE, .Number = GPIO_PIN_3},
    {.index=41, .Letter = GPIOF, .Number = GPIO_PIN_1},
    {.index=42, .Letter = GPIOF, .Number = GPIO_PIN_2},
    {.index=43, .Letter = GPIOF, .Number = GPIO_PIN_8},
    {.index=44, .Letter = GPIOF, .Number = GPIO_PIN_9},
    {.index=45, .Letter = GPIOG, .Number = GPIO_PIN_1},
    {.index=46, .Letter = GPIOE, .Number = GPIO_PIN_6},
    {.index=47, .Letter = GPIOG, .Number = GPIO_PIN_15},
    {.index=48, .Letter = GPIOG, .Number = GPIO_PIN_10},
    {.index=49, .Letter = GPIOG, .Number = GPIO_PIN_12},
    {.index=50, .Letter = GPIOG, .Number = GPIO_PIN_9},
    {.index=51, .Letter = GPIOE, .Number = GPIO_PIN_1},
    {.index=52, .Letter = GPIOG, .Number = GPIO_PIN_0},
    {.index=53, .Letter = GPIOD, .Number = GPIO_PIN_0},
    {.index=54, .Letter = GPIOD, .Number = GPIO_PIN_1},
    {.index=55, .Letter = GPIOF, .Number = GPIO_PIN_0},
    //------------------------------------------------
    {.index=56, .Letter= GPIOA, .Number = GPIO_PIN_6},
    {.index=57, .Letter= GPIOA, .Number = GPIO_PIN_5},
    {.index=58, .Letter= GPIOC, .Number = GPIO_PIN_6},
    {.index=59, .Letter= GPIOB, .Number = GPIO_PIN_11},
    {.index=60, .Letter= GPIOB, .Number = GPIO_PIN_2},
    {.index=61, .Letter= GPIOB, .Number = GPIO_PIN_1},
    {.index=62, .Letter= GPIOF, .Number = GPIO_PIN_4},
    {.index=63, .Letter= GPIOE, .Number = GPIO_PIN_8},
    //------------------------------------------------
    {.index=64, .Letter= GPIOC, .Number = GPIO_PIN_11},
    {.index=65, .Letter= GPIOC, .Number = GPIO_PIN_10},
    {.index=66, .Letter= GPIOC, .Number = GPIO_PIN_12},
    {.index=67, .Letter= GPIOF, .Number = GPIO_PIN_6},
    {.index=68, .Letter= GPIOF, .Number = GPIO_PIN_7},
    {.index=69, .Letter= GPIOB, .Number = GPIO_PIN_7},
    {.index=70, .Letter= GPIOC, .Number = GPIO_PIN_2},
    {.index=71, .Letter= GPIOC, .Number = GPIO_PIN_3},
};

uint8_t Output_Pins_len = 0;
Pin Output_Pins[GPIO_PINS_Len ];
uint8_t Input_Pins_len = 0;
Pin Input_Pins[GPIO_PINS_Len];

#define RX3_BUF_SIZE 256
//uint8_t* rx3_buf = (uint8_t*)0x30000000;
uint8_t rx3_buf[RX3_BUF_SIZE] __attribute__((section(".dma_buffer"), aligned(32)));
volatile uint8_t rx3_len = 0;
volatile uint8_t rx3_offset = 0;
volatile bool rx3_done = false;

#define CONN_MSG_SIZE 256
uint8_t conn_msg[CONN_MSG_SIZE] __attribute__((section(".dma_buffer"), aligned(32)));

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//FF FF FF 1C 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38
void do_Tx(UART_HandleTypeDef* huart, uint8_t* buf, uint16_t len) {
  while(huart->gState != HAL_UART_STATE_READY) {}
  HAL_UART_Transmit_DMA(huart, buf, len);
}

void set_pin_output(Pin pin) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin.Number;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  
  GPIO_InitStruct.Pull = GPIO_NOPULL;          
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; 
  HAL_GPIO_Init(pin.Letter, &GPIO_InitStruct);
}

void set_pin_input(Pin pin) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin.Number;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;      
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(pin.Letter, &GPIO_InitStruct);
}

int append_pin_config(uint8_t* confptr, uint8_t len) {
    if(confptr[0] >= len) {
    #ifdef Debug
    do_Tx(&huart3, (uint8_t*)"impossible pin config continue\n", 32);
    #endif
    return 1;
  }

  return 0;

}

int apply_pin_config(uint8_t* confptr, uint8_t len, bool append) {
  if(confptr[0] >= len) {
    #ifdef Debug
    do_Tx(&huart3, (uint8_t*)"impossible pin config\n", 22);
    #endif
    return 1;
  }
  #ifdef Debug 
    char conn_unit[4] = "";
    conn_msg[0] = '\0';
  #endif /* if  */
  uint8_t output_count = confptr[0];

  if (!append) {
    Output_Pins_len = 0;
    Input_Pins_len = 0;
  }

  for(uint8_t i = 1; i <= output_count; i++) {
    if (confptr[i] >= GPIO_PINS_Len) {
      #ifdef Debug
        do_Tx(&huart3, (uint8_t*)"undefined pin in config !\n", 26);
      #endif
      return 1;
    }
    Output_Pins[Output_Pins_len] = Pins[confptr[i]];
    #ifdef Debug
      snprintf(conn_unit, 4, "%d,",Output_Pins[Output_Pins_len].index);
      strcat((char*)conn_msg, conn_unit);
    #endif /* ifdef Debug */
    Output_Pins_len++; 
  }
  
  strcat((char*)conn_msg, ":");
  
  for(uint8_t i = output_count+1; i < len; i++) {
    if (confptr[i] >= GPIO_PINS_Len) {
      #ifdef Debug
        do_Tx(&huart3, (uint8_t*)"undefined pin in config !\n", 26);
      #endif
      return 1;
    }
    Input_Pins[Input_Pins_len] = Pins[confptr[i]];
    #ifdef  Debug
      snprintf(conn_unit, 4, "%d,",Input_Pins[Input_Pins_len].index);
      strcat((char*)conn_msg, conn_unit);
    #endif /* ifdef  DEBUG */
    Input_Pins_len++;
  }
  #ifdef Debug
    strcat((char*)conn_msg, "\n");
  #endif /* ifdef Debug */
  #ifdef Debug
    do_Tx(&huart3, conn_msg, strlen((char*)conn_msg));
  #endif
  return 0;
}

uint8_t* trim_leading(uint8_t thing, uint8_t* org_str, uint16_t* len ) {
  uint8_t* str = org_str;
  while(*str == thing) {str++; (*len)--;}
  return str;
}

int process_ascii_input(char* result, uint8_t size) {
    if(rx3_len <= 1) {
      #ifdef Debug
      do_Tx(&huart3, (uint8_t*)"Input Error (ASCII) !\n", 22);
      #endif
      return 1;
    }

    uint16_t rx3_clean_len = rx3_len;
    uint8_t* rx3_clean_buf = trim_leading(' ',rx3_buf, &rx3_clean_len);

    //do_Tx(&huart3, (uint8_t*)rx3_clean_buf, rx3_clean_len);

    uint8_t copy_len = ((rx3_clean_len) < (size)) ? (rx3_clean_len) : (size );
    strncpy(result, (char*)rx3_clean_buf, copy_len-1);
    result[copy_len-1] = '\0';
    return 0;
}

int process_hex_input(uint8_t* result, uint8_t* size) {
    if(rx3_len <= 1) {
      #ifdef Debug
      do_Tx(&huart3, (uint8_t*)"Input Error (HEX) !\n", 20) ;
      #endif
      return 1;
    }

    uint16_t rx3_clean_len = rx3_len;
    uint8_t* rx3_clean_buf = trim_leading(0xFF, rx3_buf, &rx3_clean_len);

    //do_Tx(&huart3, (uint8_t*)rx3_clean_buf, rx3_clean_len); //Since HEX, provides WIERD ASCII

    memset(result,0,*size);
    uint8_t copy_len = ((rx3_clean_len) < (*size)) ? (rx3_clean_len) : (*size);
    memcpy(result, rx3_clean_buf, copy_len);
    *size = copy_len;
    return 0;
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

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  State state = Default;
  bool append = false;
  HAL_UART_Transmit_DMA(&huart3, (uint8_t*)"Startup...\n", 11);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx3_buf, RX3_BUF_SIZE);
  for(int i = 0; i < GPIO_PINS_Len; i++) {
    set_pin_input(Pins[i]);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    switch (state) {
      case Default:
        while(1) {
          if(rx3_done == true) {
            rx3_done = false;
            #ifdef Debug
            do_Tx(&huart3, rx3_buf, rx3_len);
            #endif /* ifdef Debug */
            #define CMD_SIZE 5
            char cmd[CMD_SIZE];
            if (process_ascii_input(cmd,CMD_SIZE) != 0)
              break;                

            if (strcmp(cmd,"go") == 0) {
              state = Start;
              break;
            }
            else if(strcmp(cmd,"conf") == 0) {
              state = Configure_Pins;
              break;
            }
            else if(strcmp(cmd,"cont") == 0) {
              state = Continue;
              break;
            }
            else 
             break; 
          }
        }
      break;

      case Echo:
        while(1) {
          if (rx3_done == true) {
            rx3_done = false;
            do_Tx(&huart3, rx3_buf, rx3_len);
            HAL_Delay(50);
          }
        }
      break;

      case Configure_Pins:
        #ifdef Debug
        do_Tx(&huart3, (uint8_t*)"Config Pins\n", 12);
        #endif /* ifdef Debug */
        do_Tx(&huart3, (uint8_t*)"conf ACK\n",9);
        while(1) {
          if(rx3_done == true) {
            rx3_done = false;
            uint8_t config_size = 64;
            uint8_t config_data[config_size];
            if (process_hex_input(config_data, &config_size) != 0)
              break;
            if (apply_pin_config(config_data, config_size, append) != 0)
              break;
            #ifdef Debug
            do_Tx(&huart3, (uint8_t*)"Config Successfull\n", 19);
            #endif
            do_Tx(&huart3, (uint8_t*)"ACK\n",4);
            state = Default;
            append = false;
            break;
          }
        }
      
      break;
      case Continue: 
        do_Tx(&huart3, (uint8_t*)"cont ACK\n",9);
        append = true;
        state = Configure_Pins;
      break;
      case Start:
          #ifdef Debug
          do_Tx(&huart3, (uint8_t*)"Run Check\n", 10);
          #endif
          if (Output_Pins_len < 1 || Input_Pins_len < 1) {
              #ifdef Debug
              do_Tx(&huart3, (uint8_t*)"Pins not configured\n", 21);
              #endif /* ifdef Debug */
              do_Tx(&huart3, (uint8_t*)"go NACK\n", 8);
              state = Default;
              break;
          }
          conn_msg[0] = '\0';
          char conn_unit[7];

          for(uint8_t i = 0; i < Output_Pins_len; i++) {
            set_pin_output(Output_Pins[i]);
            HAL_GPIO_WritePin(Output_Pins[i].Letter, Output_Pins[i].Number, GPIO_PIN_SET);
            sprintf(conn_unit, "%d:", Output_Pins[i].index);
            strcat((char*)conn_msg,conn_unit);

            for(int k = 0; k < GPIO_PINS_Len; k++) {
              if ((HAL_GPIO_ReadPin(Pins[k].Letter, Pins[k].Number) == GPIO_PIN_SET) && (Pins[k].index != Output_Pins[i].index)) {
                sprintf(conn_unit, "%d,", Pins[k].index);
                strcat((char*)conn_msg,conn_unit);
              } 
            }

            HAL_GPIO_WritePin(Output_Pins[i].Letter, Output_Pins[i].Number, GPIO_PIN_RESET);
            set_pin_input(Output_Pins[i]);
            strcat((char*)conn_msg,"\n");
          }
          do_Tx(&huart3, conn_msg, strlen((char*)conn_msg));
          do_Tx(&huart3, (uint8_t*)"go ACK\n",7);
          state = Default;
      break;
    }
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart3.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart3.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE7 PE9 PE10
                           PE11 PE12 PE13 PE14
                           PE15 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0 PF1 PF2 PF3
                           PF8 PF9 PF10 PF11
                           PF12 PF13 PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PF4 PF6 PF7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC2 PC3 PC6 PC8
                           PC9 PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB2 PB11 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_11|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG0 PG1 PG2 PG3
                           PG4 PG5 PG6 PG8
                           PG9 PG10 PG12 PG14
                           PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PE8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD11 PD12 PD13 PD14
                           PD15 PD0 PD1 PD3
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART3)
    {
        //HAL_UART_RxEventTypeTypeDef event_type = HAL_UARTEx_GetRxEventType(huart);
        //if (event_type == HAL_UART_RXEVENT_IDLE) {
          //SCB_InvalidateDCache_by_Addr((uint32_t*)rx3_buf, RX3_BUF_SIZE);
          rx3_len = Size;
          rx3_done = true;
          HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx3_buf, RX3_BUF_SIZE);
        //}
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart3.Instance) 
    {
        uint32_t err = huart->ErrorCode;
          if (err & (HAL_UART_ERROR_ORE | HAL_UART_ERROR_NE | HAL_UART_ERROR_FE | HAL_UART_ERROR_PE)) {
              HAL_UART_AbortReceive(huart);
              __HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);
              __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF | UART_CLEAR_PEF);
              memset(rx3_buf, 0, RX3_BUF_SIZE);
              HAL_UARTEx_ReceiveToIdle_DMA(huart, rx3_buf, RX3_BUF_SIZE);
          }
    }
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
