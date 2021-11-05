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
#include "Hc05.h"
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
UART_HandleTypeDef USART1InitStruct;
/*																																						*/
/* Private function prototypes -----------------------------------------------*/

/* Private function-----------------------------------------------------------*/
/*																																            */
/**
  * @brief HC05 modülü için ön ayarlarin konfigürasyonu
  * @param yok
  * @retval yok
  */
void Hc05_Init(void)
{
	
	/**
		* UART
		*PB6  ------> USART_TX
		*PB7  ------> USART_RX
		*/
	
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef uart1PinStruct;
	
	uart1PinStruct.Pin 				= HC05_TXPIN | HC05_RXPIN;
  uart1PinStruct.Mode 			= GPIO_MODE_AF_PP;
	uart1PinStruct.Alternate 	= GPIO_AF7_USART1;
  uart1PinStruct.Pull 			= GPIO_PULLUP;
  uart1PinStruct.Speed 			= GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &uart1PinStruct);

  USART1InitStruct.Instance 					= USART1;
  USART1InitStruct.Init.BaudRate 			= BAUDRATE;
  USART1InitStruct.Init.WordLength 		= UART_WORDLENGTH_8B;
  USART1InitStruct.Init.StopBits 			= UART_STOPBITS_1;
  USART1InitStruct.Init.Parity 				= UART_PARITY_NONE;
  USART1InitStruct.Init.Mode 					= UART_MODE_TX_RX;
  USART1InitStruct.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
  USART1InitStruct.Init.OverSampling 	= UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART1InitStruct);
	
}

/**
  * @brief HC05 modülüle veri gönderme fonksiyonu
  * @param gönderilmek istenilen veri isaretsiz sekiz bit pointer olarak girdi verilir 
  * @retval yok
  */
void Hc05_send(uint8_t* canRxMsg)
{
	char sendTxt[50];
	
	sprintf(sendTxt,"%x %x %x %x %x %x %x\r\n",canRxMsg[1],canRxMsg[2],canRxMsg[3],canRxMsg[4],canRxMsg[5],canRxMsg[6],canRxMsg[7]);
	HAL_UART_Transmit(&USART1InitStruct,(uint8_t*) sendTxt,strlen((const char*)sendTxt),0xffff);
}
/*																																            */

