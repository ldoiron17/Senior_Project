//Gcodes.c


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "defines.h"
#include "Motor.h"

//Absolute Line Movements G-code Command
//Implemented using Bresenham's line algorithm
//Typically used for drawing lines screens and calculating pixel color.
void G00(int x, int y, int feedrate){
	
	
	//set_feedrate(feedrate);
	
	int dx = x - MOTOR1_X.current_position;
	int dy = y - MOTOR2_Y.current_position;
	int dirx=dx>0?1:-1;
	int diry=dy>0?1:-1;
	dx = abs(dx);
	dy = abs(dy);
	
	uint16_t i;
	uint16_t error;
	
	if (dx > dy){
		for(i=0; i < dx; i++){
			stepx(dirx, FULLSTEP);
			error+=dy;
			if(error>=dx){
				error-=dx;
				stepy(diry, FULLSTEP);
			}//end if
		}//end for
	}else{
		for(i=0; i < dx; i++){
			stepy(diry, FULLSTEP);
			error+=dx;
			if(error>=dy){
				error-=dy;
				stepx(dirx, FULLSTEP);
			}//end if
		}//end for
	}//end else
	
	MOTOR1_X.current_position = x;
	MOTOR2_Y.current_position = y;
	
};