

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "Motor.h"
#include "defines.h"



//MOTOR_T *MOTOR_struct_init(void){
	//
	////Initialize space for the struct
	//MOTOR_T *s = (MOTOR_T * *) calloc(1,sizeof(MOTOR_T *));
	//
	//s->current_position = 0;
	//s->dir = POSITIVE;
	//s->enabled = NO;
	//s->step_size = FULLSTEP;
	//
	//return s;
//}

void Motor_Disable (void){
	
	    //Set Global Motors Disable Flag
		Controller_state = DISABLED;
	
		DIS1_on();
		PWM1A_off();
		PWM1B_off();
		DIS2_on();
		PWM2A_off();
		PWM2B_off();
		DIS3_on();
		PWM3A_off();
		PWM3B_off();
		
		//_delay_ms(500); //delay 0.5 second before allowing anything else to happen
	
}

void Motor_Enable (void){
	
	//Set Global Motors Disable Flag
	Controller_state = ENABLED;
	DIS1_off();
	DIS2_off();
	//DIS3_off();

	

	
}



void stepx(int dir, int STEPSIZE){
	MOTOR1_X.dir = dir;
	MOTOR1_X.enabled = YES;
	MOTOR1_X.step_size = STEPSIZE;
	if( (STEPSIZE == FULLSTEP) & (MOTOR1_X.current_state != STATE3)){
		MOTOR1_X.current_state += 1; //increment through stepper motor state table
	}else{
		MOTOR1_X.current_state =0; //go back to beginning of table
	}
}

void stepy(int dir, int STEPSIZE){
	MOTOR2_Y.dir = dir;
	MOTOR2_Y.enabled = YES;
	MOTOR2_Y.step_size = STEPSIZE;
	if( (STEPSIZE == FULLSTEP) & (MOTOR2_Y.current_state != STATE3)){
		MOTOR2_Y.current_state += 1; //increment through stepper motor state table
	}else{
		MOTOR2_Y.current_state =0; //go back to beginning of table
	}
}