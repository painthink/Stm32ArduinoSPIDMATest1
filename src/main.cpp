#include <Arduino.h>





SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;





 #ifdef __cplusplus
          extern "C" {
            #include "ws2812b.h"
            #include "ws2812b_fx.h"


            void DMA1_Channel3_IRQHandler(void){
              HAL_DMA_IRQHandler(&hdma_spi1_tx);
            }
            void HAL_SYSTICK_Callback(void);
            static void MX_DMA_Init(void);
            static void MX_SPI1_Init(void);
          }
          #endif



void setup() {
  // put your setup code here, to run once:

  
  MX_DMA_Init();
  MX_SPI1_Init();
  
  /* USER CODE BEGIN 2 */
  WS2812B_Init(&hspi1);

  WS2812BFX_Init(3);	// Start 3 segments

  WS2812BFX_SetSpeed(0, 5000);	// Speed of segment 0
  WS2812BFX_SetSpeed(1, 2000);	// Speed of segment 1
  WS2812BFX_SetSpeed(2, 500);	// Speed of segment 2
  WS2812BFX_SetColorRGB(0, 32,0,64);	// Set color 0
  WS2812BFX_SetColorRGB(1, 32,0,0);		// Set color 1
  WS2812BFX_SetColorRGB(2, 0,64,0);		// Set color 2
  WS2812BFX_SetMode(0, FX_MODE_WHITE_TO_COLOR);	// Set mode segment 0

  WS2812BFX_SetColorRGB(0, 16,64,0);
  WS2812BFX_SetColorRGB(1, 0,32,64);
  WS2812BFX_SetColorRGB(2, 64,0,0);
  WS2812BFX_SetMode(1, FX_MODE_BLACK_TO_COLOR);	// Set mode segment 1

  WS2812BFX_SetColorRGB(0, 16,64,0);
  WS2812BFX_SetColorRGB(1, 0,32,64);
  WS2812BFX_SetColorRGB(2, 64,0,0);
  WS2812BFX_SetMode(2, FX_MODE_COLOR_WIPE); 	// Set mode segment 2

  WS2812BFX_Start(0);	// Start segment 0
  WS2812BFX_Start(1);	// Start segment 1
  WS2812BFX_Start(2);	// Start segment 2
  HAL_Delay(200);
  pinMode(LED_BUILTIN,OUTPUT);

  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  WS2812BFX_Callback();
  //WS2812BFX_Start(0);
  digitalToggle(LED_BUILTIN);
  delay(1000);
  HAL_Delay(1000);

}

void HAL_SYSTICK_Callback(void)
{
  WS2812BFX_SysTickCallback();
}


static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI1 DMA Init */
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA1_Channel3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_CIRCULAR;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmatx,hdma_spi1_tx);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }

}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_7);

    /* SPI1 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmatx);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }

}




