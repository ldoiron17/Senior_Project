

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "Motor.h"
#include "defines.h"


volatile signed int Axis1_x_coord=0;
volatile signed int  Axis1_y_coord=0;
volatile signed int  Axis2_x_coord=0;
volatile signed int  Axis2_y_coord=0;
volatile signed int  Axis3_x_coord=0;
volatile signed int  Axis3_y_coord=0;
volatile signed int  Axis1_x_coord_old=0;
volatile signed int  Axis1_y_coord_old=0;
volatile int  Controller_state = 1;
volatile int  Coordinates_updated = 0;
volatile int  Coordinates_verified = 0;
volatile int  Receiving_Coords = 0;
volatile int  Coordinates_correct = 0;
volatile int  parsed_coord=0;
volatile int  state1_hysterisis_delay = 0;
volatile int  state2_hysterisis_delay = 0;
volatile int  PWM1A = 0;
volatile int  PWM1B = 0;
volatile int  PWM2A = 0;
volatile int  PWM2B = 0;
volatile char  buffer[10];       //Used to Store ADC value readings from itoa() of ADC register
volatile char  *clr_screen = "^\033[2J\033[0;0H";
volatile char  *Init_statement = "^\033[2J\033[0;0HWelcome to Crazy Numerical Control 3000\r\nWhere All Of Your Dreams Will Come True\r\n.......................................\r\n\r\n\r\n";
volatile char  *Coord_request_x = "Please Input The Desired X Coordinate:";
volatile char  *Coord_request_y = "Please Input The Desired Y Coordinate:";
volatile char  *Verify_coords1 = "You input the Coordinates (";
volatile char  *Verify_coords2 = ",";
volatile char  *Verify_coords3 = "). Is this correct? (Y/N)";
volatile char  *Verify_coords4 = "Moving To Desired Coordinates...";
volatile char  *Verify_coords5 = "Please Renter Coordinates:";
volatile char  *newline = "\r\n";
volatile char  *clearline = "\r";
extern volatile int motor_movement_distance=0;
volatile int motor_dir = POSITIVE;
volatile int motor_moving = NO;
volatile int  move_motor = NO;
volatile int motor_step_count = 0;
volatile int ADC_count = 0;
volatile int count = 0;
volatile int iter = 0;
volatile int state1 = POS_CUR;
volatile int state2 = POS_CUR;
volatile char data;
volatile uint16_t ADCval=0;
volatile uint16_t ADCval1=0;
volatile uint16_t ADCval2=0;
volatile uint16_t ADCval3=0;
volatile uint16_t ADCval4=0;
volatile uint16_t ADC_avg=0;
volatile uint8_t  SRAMchar;
volatile uint16_t SRAMint;
volatile uint8_t  SRAMstring[10];
volatile char mydatabuffer[11][10];


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