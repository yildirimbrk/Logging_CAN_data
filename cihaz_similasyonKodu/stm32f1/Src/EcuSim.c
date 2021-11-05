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
#include "EcuSim.h"
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
CAN_HandleTypeDef hcan;

CAN_TxHeaderTypeDef pTxHeader;
CAN_RxHeaderTypeDef pRxHeader;
CAN_FilterTypeDef sFilterConfig;
uint32_t pTxMailbox;
uint8_t requestMsg[OBD2_DATALENGTH];
uint8_t responseMsg[OBD2_DATALENGTH] = {0} ;
/*																																						*/
/* Private function prototypes -----------------------------------------------*/
/*																																						*/

/*																																						*/
/* External variables --------------------------------------------------------*/
/*																																						*/
extern uint8_t speedValue_msb ;
extern uint8_t speedValue_lsb ;
extern uint8_t rpmValue_msb ;
extern uint8_t rpmValue_lsb ;
/*																																						*/
/* Private function-----------------------------------------------------------*/
/*																																            */



void USB_LP_CAN1_RX0_IRQHandler(void)
{

  HAL_CAN_IRQHandler(&hcan);

	HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,&pRxHeader,requestMsg);
	
	if(requestMsg[2]==PID_SPEED)
	{
		obd2Reponse_speed();
	}
	
	if(requestMsg[2]==PID_RPM)
	{
		obd2Reponse_rpm();
	}
	
}

/**
  * @brief Ecu dan hiz degeri istenirse hiz verisini gönderir
  * @param yok
  * @retval yok
  */
void obd2Reponse_speed(void)
{
	responseMsg[0]=	0x03;
	responseMsg[1]=	0x41;
	responseMsg[2]=	0x0D;
	responseMsg[3]=	speedValue_lsb;
	responseMsg[4]=	0xAA;
	responseMsg[5]=	0xAA;
	responseMsg[6]=	0xAA;
	responseMsg[7]=	0xAA;
	
	HAL_CAN_AddTxMessage(&hcan,&pTxHeader,responseMsg,&pTxMailbox);
}

/**
  * @brief Ecu dan motor devri istenirse rpm verisini gönderir
  * @param yok
  * @retval yok
  */
void obd2Reponse_rpm(void)
{
	responseMsg[0]=	0x04;
	responseMsg[1]=	0x41;
	responseMsg[2]=	0x0C;
	responseMsg[3]=	rpmValue_msb;
	responseMsg[4]=	rpmValue_lsb;
	responseMsg[5]=	0xAA;
	responseMsg[6]=	0xAA;
	responseMsg[7]=	0xAA;
	
	HAL_CAN_AddTxMessage(&hcan,&pTxHeader,responseMsg,&pTxMailbox);
}



/**
  * @brief can bus ön ayarlari bu fonksiyonda yapilir
  * @param yok
  * @retval yok
  */
void EcuSim_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_CAN1_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**CAN GPIO Configuration
	PB8     ------> CAN_RX
	PB9     ------> CAN_TX
	*/
	GPIO_InitStruct.Pin = CAN1_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = CAN1_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	__HAL_AFIO_REMAP_CAN1_2();
  
	hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  HAL_CAN_Init(&hcan);
	
		/* CAN1 interrupt Init */
	HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	
	//	Start CAN  communication	//
	HAL_CAN_Start(&hcan);
				// Enable Interrupt	//
	HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
				//	Set Transmit Param 	//
	pTxHeader.DLC = OBD2_DATALENGTH ;
	pTxHeader.IDE = CAN_ID_STD ;
	pTxHeader.RTR = CAN_RTR_DATA ;
	pTxHeader.StdId = MSG_ID;
				//	Set Filter Param	//
	sFilterConfig.FilterActivation = ENABLE ;
	sFilterConfig.FilterBank = 0 ;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;
	sFilterConfig.FilterIdHigh = FILTER_ID<<5;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST ;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT ;
	HAL_CAN_ConfigFilter(&hcan,&sFilterConfig) ;
  
}
/*																																						*/









