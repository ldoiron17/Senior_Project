

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



void Receive_Coords( void ){
	
	unsigned char tmp;
	//Ask for input Coordinate X
	Receiving_Coords = YES;
	Coordinates_updated = NOT_UPDATED;
	USART_putstring(Coord_request_x); 
	USART_putstring(newline);
	while( Coordinates_updated == NOT_UPDATED ){
		
		
		tmp = USART_Receive();
		//USART_Transmit(tmp);
		
		//LED1_off();
		if(Receiving_Coords == YES){
				

		if(Coordinates_updated == NOT_UPDATED){
			parsed_coord = atoi(&tmp);
			
			Coordinates_updated = UPDATED;
		}
				
		/*if(Coordinates_verified == UNVERIFIED){
			if( tmp == 'Y' | tmp == 'y'){
				Coordinates_correct = YES;
				} else{
				Coordinates_correct = NO;
				}	
			}
			*/	
				
		}
	}
	Axis1_x_coord = parsed_coord;
	
	//Ask for input Coordinate Y
	USART_putstring(Coord_request_y); 
	USART_putstring(newline);
	Coordinates_updated = NOT_UPDATED;
	while( Coordinates_updated == NOT_UPDATED ){
		
		tmp = USART_Receive();
		//USART_Transmit(tmp);
		if(Receiving_Coords == YES){
			

			if(Coordinates_updated == NOT_UPDATED){
				parsed_coord = atoi(&tmp);
				Coordinates_updated = UPDATED;
			}
			
			/*if(Coordinates_verified == UNVERIFIED){
				if( tmp == 'Y' | tmp == 'y'){
					Coordinates_correct = YES;
					} else{
					Coordinates_correct = NO;
				}
			}
			*/
			
		}
	}
	Axis1_y_coord = parsed_coord;
	
	//Verify that the input coordinates are correct
	Coordinates_verified = UNVERIFIED;
	Coordinates_correct = NO;
	USART_putstring(Verify_coords1);
	itoa(Axis1_x_coord, buffer, 10);
	USART_putstring(buffer);
	USART_putstring(Verify_coords2);
	itoa(Axis1_y_coord, buffer, 10);
	USART_putstring(buffer);
	USART_putstring(Verify_coords3);
	USART_putstring(newline);
	while( Coordinates_verified == UNVERIFIED ){
		
		tmp = USART_Receive();
		//USART_Transmit(tmp);
		//LED1_off();
		if(tmp == 'y'){
			Coordinates_correct = YES;
			Coordinates_verified = VERIFIED;
		}else{
			Receive_Coords();
		}
		//USART_putstring(clearline);

	}
	
	Receiving_Coords = NO;
	
}

void stepx(int dir, int STEPSIZE){
	MOTOR1_X.dir = dir;
	MOTOR1_X.enabled = YES;
	MOTOR1_X.step_size = STEPSIZE;
}

void stepy(int dir, int STEPSIZE){
	MOTOR2_Y.dir = dir;
	MOTOR2_Y.enabled = YES;
	MOTOR2_Y.step_size = STEPSIZE;
}