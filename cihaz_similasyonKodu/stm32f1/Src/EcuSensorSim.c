/********************************************************************************************************************************************
*																																																																						*
*********************************************************************************************************************************************
*		System Name: 																																																		*
*																																																																						*
*		Version: 1.0																																																														*			
*																																																																						*		
*		Module Name:																																																														*	
*																																																																						*
*		File Name: 																																																							*
*																																																																						*
*		Lead Author: Burak YILDIRIM.																																																						*
*																																																																						*
*		Modification 																																																			*
*																																																																						*	
*		Description:																																																														*
*																																																																						*

* 																																																																					*
*		Notes:																																																																	*
*																																																																						*
*	 [1] 
*	 																																																																					*
*	 [2]  							*
*********************************************************************************************************************************************
*																																																																						*	
********************************************************************************************************************************************/
/* Includes ------------------------------------------------------------------*/
/*																																						*/
#include "EcuSensorSim.h"
/*																																						*/
/* Private includes ----------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Private typedef -----------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Private define ------------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Private macro -------------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Private variables ---------------------------------------------------------*/
/*																																						*/
ADC_HandleTypeDef hadc1;

uint16_t EcuSensorsValues[2];

/*																																						*/
/* Private function prototypes -----------------------------------------------*/

/* Private function-----------------------------------------------------------*/
/*																																            */
/**
  * @brief Sensörler için adc çevre birimi ayari fonksiyonu
  * @param None
  * @retval None
  */
void EcuSensors_init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
	
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);
  
}

/**
  * @brief ECU hiz sensörü ayari fonksiyonu
  * @param None
  * @retval None
  */
void EcuSpeedSensor_init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
}

/**
  * @brief ECU devir sensörü ayari fonksiyonu
  * @param None
  * @retval None
  */
void EcuRpmSensor_init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
  
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
}

/**
  * @brief ECU hiz sensörü okuma fonksiyonu
  * @param None
  * @retval 12 bitlik hiz degerini döndürür
  */
uint16_t Ecu_speedSensorRead(void)
{
	EcuSpeedSensor_init();
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	EcuSensorsValues[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	
	
	return EcuSensorsValues[0];
}


/**
  * @brief ECU hiz sensörü okuma fonksiyonu
  * @param None
  * @retval 12 bitlik rpm degerini döndürür
  */
uint16_t Ecu_rpmSensorRead(void)
{
	  EcuRpmSensor_init();
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 1000);
	  EcuSensorsValues[1] = HAL_ADC_GetValue(&hadc1);
	  HAL_ADC_Stop(&hadc1);
		
		
	return EcuSensorsValues[1];
}

/*																																						*/


