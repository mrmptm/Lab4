/*
 * input_processing.c
 *
 *  Created on: Dec 6, 2022
 *      Author: DELL
 */
#include "input_processing.h"
#include "scheduler.h"
#include "main.h"
#include "button.h"
#include "Tasks.h"
int button1_executed=0;
void input_processing(){
	if(button_pressed(0)==1){  //button pressed
		//Update task or reset task
		if(button1_executed==0){
			SCH_Add_Task(&TurnOnGreenLed, 0, 0);
			button1_executed=1;
		}
	}else if(button_pressed(0)==0){  //button released
		button1_executed=0;
	}
}


