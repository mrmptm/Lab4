/*
 * scheduler.c
 *
 *  Created on: Nov 17, 2022
 *      Author: DELL
 */

#include "scheduler.h"
#include <stdio.h>
#include "Tasks.h"
int task_count=0;
unsigned char Error_code=0;
unsigned char Last_Error_code=0;
sTask SCH_tasks[MAX_TASK];

time_t rawtime;
void SCH_Report_Status();
void SCH_Go_To_Sleep();

void SCH_Init(){
	task_count=0;
	Error_code=0;
}

int SCH_Add_Task(void(*func)(), int32_t delay, uint32_t Period){
	if(task_count>=MAX_TASK){
		Last_Error_code=Error_code;
		Error_code=ERROR_SCH_TOO_MANY_TASKS;
		return 0;
	}
	/* CHECK IF THIS TASK EXISTED START */
	for(int i=0;i<task_count;i++){
		if(SCH_tasks[i].ptask==func){
			SCH_tasks[i].Delay=delay/TIMER_CYCLE;
			SCH_tasks[i].Period=Period/TIMER_CYCLE;
			SCH_tasks[i].Runme=0;
			return task_count;
		}
	}
	/* CHECK IF THIS TASK EXISTED END */
	SCH_tasks[task_count].ptask=func;
	SCH_tasks[task_count].Delay=delay/TIMER_CYCLE;
	SCH_tasks[task_count].Period=Period/TIMER_CYCLE;
	SCH_tasks[task_count].Runme=0;
	task_count++;
	return task_count;
}
void SCH_Update(){
	for(int i=0;i<task_count;i++){
		if(SCH_tasks[i].Delay>=0){
			SCH_tasks[i].Delay--;
			if(SCH_tasks[i].Delay<=0){
				SCH_tasks[i].Runme++;
				SCH_tasks[i].Delay=SCH_tasks[i].Period;
			}
		}
	}
}
void SCH_Dispatch(UART_HandleTypeDef*huart){
	for(int i=0;i<task_count;i++){
		if(SCH_tasks[i].Runme>0){
			/* Demonstration code start */
			char info[50];
			if(SCH_tasks[i].ptask==&ToggleRedLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Toggle red led start at %d\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOnGreenLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn on green led start at %d\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOffGreenLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn off green led start at %d\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOnYellowLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn on yellow led start at %d\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOffYellowLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn off yellow led start at %d\r",HAL_GetTick()), 100);
			/* Demonstration code end */
			(*SCH_tasks[i].ptask)();

			/* Demonstration code start */
			if(SCH_tasks[i].ptask==&ToggleRedLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Toggle red led end at %d\r\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOnGreenLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn on green led end at %d\r\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOffGreenLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn off green led end at %d\r\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOnYellowLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn on yellow led end at %d\r\r",HAL_GetTick()), 100);
			else if(SCH_tasks[i].ptask==&TurnOffYellowLed)
				HAL_UART_Transmit(huart, info,
					sprintf(info,"Turn off yellow led end at %d\r\r",HAL_GetTick()), 100);
			/* Demonstration code end */

			SCH_tasks[i].Runme--;
			if(SCH_tasks[i].Period==0){  //IF THIS IS ONE SHOT TASK
				SCH_Delete_Task(i);
			}
		}
	}
}

void SCH_Delete_Task(int idx){
	for(int i=idx;i<task_count-1;i++){
		SCH_tasks[i]=SCH_tasks[i+1];
	}
	if(task_count>0){
		task_count--;
		Error_code=0;
	}else{
		Last_Error_code=Error_code;
		Error_code=ERROR_SCH_CANNOT_DELETE_TASK;
	}
}
int number_current_task(){
	return task_count;
}
void SCH_Report_Status(){
#ifdef SCH_REPORT_ERRORS

#endif
}
