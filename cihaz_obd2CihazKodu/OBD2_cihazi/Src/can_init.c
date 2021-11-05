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
#include "can_init.h"
/*																																						*/
/* Private includes ----------------------------------------------------------*/
/*																																						*/
#include "Hc05.h"
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
CAN_HandleTypeDef hcan1;
CAN_TxHeaderTypeDef pTxHeader;
CAN_RxHeaderTypeDef pRxHeader;
CAN_FilterTypeDef sFilterConfig;
uint32_t pTxMailbox;

uint8_t obd2ResponseMsg[OBD2_DATALENGTH] ; // Response message
uint8_t obd2RequestMsg_vecihleSpeed[OBD2_DATALENGTH] ={0x02, 0x01, 0x0D, 0x55, 0x55, 0x55, 0x55, 0x55} ; //Request message
uint8_t obd2RequestMsg_vecihleRpm[OBD2_DATALENGTH] ={0x02, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00} ; //Request message
/*																																						*/
/* Private function prototypes -----------------------------------------------*/

/* Private function-----------------------------------------------------------*/
/*																																            */

/**
  * @brief CAN1 için genel kesme fonksiyonu
  * @param None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{

  HAL_CAN_IRQHandler(&hcan1);

	HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&pRxHeader,obd2ResponseMsg);
	
	Hc05_send(obd2ResponseMsg);
	
}		

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
void CAN1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	
	__HAL_RCC_CAN1_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	/**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
	GPIO_InitStruct.Pin = CAN1_TX_PIN|CAN1_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 4;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  HAL_CAN_Init(&hcan1);
 

	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	
			//	Start CAN  communication	//
	HAL_CAN_Start(&hcan1);
			// Enable Interrupt	//
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
			//	Set Transmit Param 	//
  pTxHeader.DLC = OBD2_DATALENGTH ;
	pTxHeader.IDE = CAN_ID_STD ;
	pTxHeader.RTR = CAN_RTR_DATA ;
	pTxHeader.StdId = REQUEST_ID ;
			//	Set Filter Param	//
	sFilterConfig.FilterActivation = ENABLE ;
	sFilterConfig.FilterBank = 0 ;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;
	sFilterConfig.FilterIdHigh = NO_FILTER_ID;
	sFilterConfig.FilterIdLow = NO_FILTER_ID;
	sFilterConfig.FilterMaskIdHigh = NO_FILTER_ID;
	sFilterConfig.FilterMaskIdLow = NO_FILTER_ID;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK ;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT ;
	HAL_CAN_ConfigFilter(&hcan1,&sFilterConfig) ;
}


/**
  * @brief OBD2 hiz verisini talep etme fonksiyonu
  * @param yok
  * @retval yok
  */
void Obd_speedDataRequest(void)
{
	HAL_CAN_AddTxMessage(&hcan1,&pTxHeader,obd2RequestMsg_vecihleSpeed,&pTxMailbox);
	HAL_Delay(500);
}


/**
  * @brief OBD2 motor devir verisini talep etme fonksiyonu
  * @param yok
  * @retval yok
  */
void Obd_rpmDataRequest(void)
{
	HAL_CAN_AddTxMessage(&hcan1,&pTxHeader,obd2RequestMsg_vecihleRpm,&pTxMailbox);
	HAL_Delay(500);
}
/*																																						*/
