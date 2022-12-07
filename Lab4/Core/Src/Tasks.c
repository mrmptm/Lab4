/*
 * Tasks.c
 *
 *  Created on: Nov 22, 2022
 *      Author: DELL
 */
#include "Tasks.h"
#include "scheduler.h"
void ToggleRedLed(){
	HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
}
void TurnOnYellowLed(){
	SCH_Add_Task(&TurnOffYellowLed, 2500, 0);  //after 2.5s turn off
	HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin, 0);
}
void TurnOffYellowLed(){
	SCH_Add_Task(&TurnOnYellowLed, 2000, 0);  //after 2s turn on
	HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin, 1);
}
void TurnOnGreenLed(){
	SCH_Add_Task(&TurnOffGreenLed, 1000, 0);  //after 1s turn off
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 0);
}
void TurnOffGreenLed(){
	SCH_Add_Task(&TurnOnGreenLed, 1500, 0);  //after 1.5s turn on
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 1);
}
void TurnOffAllLed(){
	HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin, 1);
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, 1);
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, 1);
}

