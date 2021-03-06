/********************************************************************************************************************************************
*																																																																						*
*********************************************************************************************************************************************
*		System Name: 																																																														*
*																																																																						*
*		Version: 1.0																																																														*			
*																																																																						*		
*		Module Name:																																																														*	
*																																																																						*
*		File Name: 																																																															*
*																																																																						*
*		Lead Author: Burak YILDIRIM.																																																						*
*																																																																						*
*		Modification Dates: 																																																										*
*																																																																						*	
*		Description:																																																														*
*																																																																						*
																																																																						*	
*																																																																						*
* 																																																																					*
*		Notes:																																																																	*
*																																																																						*
*	 [1]																																																																			*
*	 																																																																					*
*	 [2]																																																										    							*
*********************************************************************************************************************************************
*																																																																						*	
********************************************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
/*																																						*/
#ifndef __ecuSensorSim_init
#define __ecuSensorSim_init
/*																																						*/

/* Includes ------------------------------------------------------------------*/
/*																																						*/
#include "main.h"
/*																																						*/
/*																																						*/
/* Private includes ----------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Exported types ------------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Exported constants --------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Exported macro ------------------------------------------------------------*/
/*																																						*/

/*																																						*/
/* Exported functions prototypes ---------------------------------------------*/
/*																																						*/
void EcuSensors_init(void);
void EcuSpeedSensor_init(void);
void EcuRpmSensor_init(void);
uint16_t Ecu_speedSensorRead(void);
uint16_t Ecu_rpmSensorRead(void);

/*																																						*/
/* Private defines -----------------------------------------------------------*/
/*																																						*/

/*																																						*/

#endif
