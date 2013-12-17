//Gcodes.c


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "defines.h"
#include "Motor.h"
#include "Gcodes.h"

//Absolute Line Movements G-code Command
//Implemented using Bresenham's line algorithm
//Typically used for drawing lines screens and calculating pixel color.
//Input coordinates are in MILS (0.001 inches = 1 MIL)
void G00(int x, int y, int feedrate){
	
	//need to implement feedrate control still!
	//set_feedrate(feedrate); 
	
	int dx = x - MOTOR1_X.current_position;
	int dy = y - MOTOR2_Y.current_position;
	int dirx=dx>0?POSITIVE:NEGATIVE;
	int diry=dy>0?POSITIVE:NEGATIVE;
	dx = abs(dx);
	dy = abs(dy);
	
	uint16_t i;
	uint16_t error;
	
	MOTOR1_X.wait = 1;
	
	if (dx > dy){
		for(i=0; i < dx; i++){
			while(MOTOR1_X.wait == 1){}
			stepx(dirx, FULLSTEP);
			error+=dy;
			if(error>=dx){
				error-=dx;
				stepy(diry, FULLSTEP);
			}//end if
			MOTOR1_X.wait = 1;
		}//end for
	}else{
		for(i=0; i < dx; i++){
			while(MOTOR1_X.wait == 1){}
			stepy(diry, FULLSTEP);
			error+=dx;
			if(error>=dy){
				error-=dy;
				stepx(dirx, FULLSTEP);
			}//end if
			MOTOR1_X.wait = 1;
		}//end for
	}//end else
	
	MOTOR1_X.current_position = x;
	MOTOR2_Y.current_position = y;
	
};