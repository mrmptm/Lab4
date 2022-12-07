/*
 * button.c
 *
 *  Created on: Oct 7, 2022
 *      Author: DELL
 */
#include "button.h"
int debounceButtonBuffer2[NUM_OF_BTN];
int debounceButtonBuffer1[NUM_OF_BTN];
int buttonBuffer[NUM_OF_BTN];
int flagForButtonPress1s[NUM_OF_BTN];
int counterForButtonPress1s[NUM_OF_BTN];

/* DOUBLE CLICK CONTROL VAR START */
int click_count[NUM_OF_BTN];
int increased_click_count[NUM_OF_BTN];
int time_out_btn_click[NUM_OF_BTN];
int flagForDoubleClicked[NUM_OF_BTN];
/* DOUBLE CLICK CONTROL VAR END */


/* 	BUTTON1 PARAMETER END */
GPIO_TypeDef*BTN_GPIO_Port[NUM_OF_BTN];
uint16_t BTN_GPIO_Pin[NUM_OF_BTN];
/* 	BUTTON2 PARAMETER END */
void BTN_INIT(GPIO_TypeDef*GPIO_Port,uint16_t GPIO_Pin,int idx){
	BTN_GPIO_Port[idx]=GPIO_Port;
	BTN_GPIO_Pin[idx]=GPIO_Pin;
	debounceButtonBuffer1[idx]=NORMAL_STATE;
	debounceButtonBuffer2[idx]=NORMAL_STATE;
	buttonBuffer[idx]=NORMAL_STATE;
	flagForButtonPress1s[idx]=0;

	counterForButtonPress1s[idx]=1000;

	flagForDoubleClicked[idx]=0;
	click_count[idx]=0;
	increased_click_count[idx]=0;
	time_out_btn_click[idx]=DOUBLE_CLICK_INTERVAL;
	flagForDoubleClicked[idx]=0;
}

void button_reading(int idx){
	debounceButtonBuffer2[idx] =debounceButtonBuffer1[idx];
	debounceButtonBuffer1[idx] = HAL_GPIO_ReadPin(BTN_GPIO_Port[idx], BTN_GPIO_Pin[idx]);
	if(debounceButtonBuffer1[idx] == debounceButtonBuffer2[idx])
		buttonBuffer[idx] = debounceButtonBuffer1[idx];

	if(buttonBuffer[idx] == PRESSED_STATE){
	//if a button is pressed, we start counting
		if(counterForButtonPress1s[idx] >0){
			counterForButtonPress1s[idx]=counterForButtonPress1s[idx]-TIMER_CYCLE;
		} else {
		//the flag is turned on when 1 second has passed
		//since the button is pressed.
			flagForButtonPress1s[idx] = 1;
		}

		/* DOUBLE CLICK START */
		if(increased_click_count[idx]==0){
			click_count[idx]++;
			increased_click_count[idx]=1;
		}
		time_out_btn_click[idx]=DOUBLE_CLICK_INTERVAL;
		if(click_count[idx]==2){
			flagForDoubleClicked[idx]=1;
			click_count[idx]=0;
		}
		/* DOUBLE CLICK END */
	}else if(buttonBuffer[idx] == NORMAL_STATE){
		/* BUTTON PRESSED START */
		counterForButtonPress1s[idx] = 1000;
		flagForButtonPress1s[idx] = 0;

		/* BUTTON PRESSED END */

		/* DOUBLE CLICK START */
		increased_click_count[idx]=0;
		flagForDoubleClicked[idx]=0;
		if(click_count[idx]==1){
			if(time_out_btn_click[idx]>0)
				time_out_btn_click[idx]=time_out_btn_click[idx]-TIMER_CYCLE;
			if(time_out_btn_click[idx]<=0){ //TIME OUT FOR DOUBLE CLICK
				click_count[idx]=0;
			}
		}
		/* DOUBLE CLICK END */
	}
}

/* 	BUTTON3 PARAMETER START */

int button_pressed(int idx){
	return (buttonBuffer[idx] == PRESSED_STATE);
}


int button_pressed_1s(int idx){
	return (flagForButtonPress1s[idx] == 1);
}

int button_doubleClicked(int idx){
	return flagForDoubleClicked[idx]==1;
}


