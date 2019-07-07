#include "LA.h"
#include "main.h"

#define TRUE  1
#define FALSE 0
//Global Data Declaration
uint8_t capturedData[SAMPLESIZE];
uint8_t isTransferComplete = FALSE;

//Local functions
static void DMATransferComplete(DMA_HandleTypeDef * hdma);

//Global Function Definition

void LASetSamplingFreq(uint32_t divider)
{
	  for(int i = 0; i < 200; i++)
	  {
		  capturedData[i] = (uint8_t)i;
	  }
	// Stop Timer
	HAL_TIM_Base_Stop_IT(&htim1);

	//Choose Prescalar and Period
	uint64_t requiredFrequency = SUMPBASEFREQ / (divider + 1);
	uint64_t period = 0u;
	uint64_t prescalar = 0u;
	uint64_t requiredPeriodScalarProduct = (TIMERBASEFREQ / requiredFrequency);

	for(uint32_t loopIndex = 1; loopIndex < 0xFFFFu; loopIndex++)
	{
		prescalar = loopIndex;
		period = requiredPeriodScalarProduct / prescalar;

		if(period < 0xFFFFu)
		{
			break;
		}
	}

	  //htim1.Instance = TIM1;
	  htim1.Init.Prescaler = (uint32_t)(prescalar - 1);
	  //htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim1.Init.Period =    (uint32_t)(period - 1);
	  //htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  //htim1.Init.RepetitionCounter = 0;
	  //htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	  {
	    Error_Handler();
	  }


}

void LABeginSampling  (uint32_t numberOfSamples)
{
	  isTransferComplete = FALSE;
	  if(numberOfSamples < SAMPLESIZE)
	  {
		  HAL_DMA_RegisterCallback(&hdma_tim1_up, HAL_DMA_XFER_CPLT_CB_ID, DMATransferComplete);
		  HAL_StatusTypeDef status = HAL_DMA_Start_IT(&hdma_tim1_up, (uint32_t)&(GPIOC->IDR), (uint32_t)capturedData, numberOfSamples);
		  if(status != HAL_OK)
		  {
			  while(1);
		  }
		  __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);
		  HAL_TIM_Base_Start_IT(&htim1);
	  }

	}

uint8_t LAIsSamplingComplete()
{
	return isTransferComplete;
}

//Local functions definition
static void DMATransferComplete(DMA_HandleTypeDef * hdma)
{
	isTransferComplete = TRUE;
	HAL_TIM_Base_Stop_IT(&htim1);

	}
