

# define F_CPU 8000000UL //8MHz clock speed, used for _delay_ms()

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> 
#include <avr/eeprom.h>
#include "USART.h"

//uint8_t  EEMEM NonVolatileChar="a";
uint16_t EEMEM NonVolatileInt=16;
uint8_t  EEMEM NonVolatileString[10]="This is 10";

unsigned char EEMEM  mydata[11][10] =
{
	{0x6D,0x6D,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09},
	{0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13},
	{0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D},
	{0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27},
	{0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31},
	{0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B},
	{0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45},
	{0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F},
	{0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59},
	{0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63},
	{0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D}
};

//Define constants
#define POSITIVE 1
#define NEGATIVE 0
#define ON 1
#define OFF 0
#define INIT 0
#define POS_CUR 1
#define NEG_CUR 2
#define DISABLED 1
#define ENABLED 0
#define VERIFIED 1
#define UNVERIFIED 0
#define NOT_UPDATED 0
#define NO 0
#define YES 1
#define UPDATED 1
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

//LED functions for turning on and off status LED 1
#define LED1_on()  PORTD |= _BV(6)
#define LED1_off()  PORTD &= ~_BV(6)
#define led1_toggle() PORTD ^= _BV(6);

//LED functions for turning on and off status LED 2
#define LED2_on()  PORTD |= _BV(4)
#define LED2_off()  PORTD &= ~_BV(4)

//Motor Driver Chip functions to control voltage outputs for H-Bridge 1
#define PWM1A_on()  PORTA |= _BV(2)
#define PWM1A_off()  PORTA &= ~_BV(2)
#define PWM1B_on()  PORTA |= _BV(1)
#define PWM1B_off()  PORTA &= ~_BV(1)
#define DIS1_on()  PORTA |= _BV(0)     //Disable Motor Driver Chip 1
#define DIS1_off()  PORTA &= ~_BV(0)   //Enable Motor Driver Chip 1

//Motor Driver Chip functions to control voltage outputs for H-Bridge 2
#define PWM2A_on()  PORTA |= _BV(5)
#define PWM2A_off()  PORTA &= ~_BV(5)
#define PWM2B_on()  PORTA |= _BV(4)
#define PWM2B_off()  PORTA &= ~_BV(4)
#define DIS2_on()  PORTA |= _BV(3)     //Disable Motor Driver Chip 2
#define DIS2_off()  PORTA &= ~_BV(3)   //Enable Motor Driver Chip 2

//Motor Driver Chip functions to control voltage outputs for H-Bridge 3
#define PWM3A_on()  PORTG |= _BV(2)
#define PWM3A_off()  PORTG &= ~_BV(2)
#define PWM3B_on()  PORTA |= _BV(7)
#define PWM3B_off()  PORTA &= ~_BV(7)
#define DIS3_on()  PORTA |= _BV(6)     //Disable Motor Driver Chip 3
#define DIS3_off()  PORTA &= ~_BV(6)   //Enable Motor Driver Chip 3

//Define volatile variables for use in ISRs

int volatile motor_movement_distance = 0;
int volatile motor_dir = POSITIVE;
int volatile motor_moving = NO;
int volatile move_motor = NO;
int volatile motor_step_count = 0;
int volatile ADC_count = 0;
int volatile count = 0;
int volatile iter = 0;
int volatile state1 = POS_CUR;
int volatile state2 = POS_CUR;
int volatile Controller_state = 1;
int volatile Coordinates_updated = 0;
int volatile Coordinates_verified = 0;
int volatile Receiving_Coords = 0;
int volatile Coordinates_correct = 0;
int volatile parsed_coord=0;
int volatile state1_hysterisis_delay = 0;
int volatile state2_hysterisis_delay = 0;
signed int volatile Axis1_x_coord=0;
signed int volatile Axis1_y_coord=0;
signed int volatile Axis2_x_coord=0;
signed int volatile Axis2_y_coord=0;
signed int volatile Axis3_x_coord=0;
signed int volatile Axis3_y_coord=0;
signed int volatile Axis1_x_coord_old=0;
signed int volatile Axis1_y_coord_old=0;
char volatile data;
int volatile PWM1A = 0;
int volatile PWM1B = 0;
int volatile PWM2A = 0;
int volatile PWM2B = 0;
uint16_t volatile ADCval=0;
uint16_t volatile ADCval1=0;
uint16_t volatile ADCval2=0;
uint16_t volatile ADCval3=0;
uint16_t volatile ADCval4=0;
uint16_t volatile ADC_avg=0;
char volatile buffer[10];       //Used to Store ADC value readings from itoa() of ADC register
char volatile dummy_dummy;
char *clr_screen = "^\033[2J\033[0;0H";
char *Init_statement = "^\033[2J\033[0;0HWelcome to Crazy Numerical Control 3000\r\nWhere All Of Your Dreams Will Come True\r\n.......................................\r\n\r\n\r\n";
char *Coord_request_x = "Please Input The Desired X Coordinate:";
char *Coord_request_y = "Please Input The Desired Y Coordinate:";
char *Verify_coords1 = "You input the Coordinates (";
char *Verify_coords2 = ",";
char *Verify_coords3 = "). Is this correct? (Y/N)";
char *Verify_coords4 = "Moving To Desired Coordinates...";
char *Verify_coords5 = "Please Renter Coordinates:";
char volatile *newline = "\r\n";
char volatile *clearline = "\r";
uint8_t  SRAMchar;
uint16_t SRAMint;
uint8_t  SRAMstring[10];
char mydatabuffer[11][10];



void Init_PWM(void){
	
	/************************Set up PWM timers***************************************/
	TIMSK = //Timer/Counter Interrupt Mask Register
	//(1 << TOIE1) |  //Overflow Interrupt Enable
	(1 << OCIE1A) | //Output Compare A Match Interrupt Enable
	(1 << OCIE2);// | //timer two output compare enable
	//(1 << TOIE2);// | //enable timer 2 interrupt
	//(1 << OCIE1B);  //Output Compare B Match Interrupt Enable
	
	
	//ETIMSK = (1 << TOIE3) | (1 << OCIE3B) | (1 << OCIE3A);
	//TCCR3A = (0 << WGM31) | (0 << WGM30);// | (1 << COM3A0) | (1 << COM3B0);
	//TCCR3B = (0 << WGM33) | (1 << WGM32)| (0 << CS32) |(0 << CS31) | (1 << CS30);
	TCCR1A = (0 << WGM11) | (0 << WGM10);// | (1 << COM1A0) | (1 << COM1B0);
	TCCR1B = (0 << WGM13) | (1 << WGM12) | (0 << CS02) |(1 << CS01) | (1 << CS00); //clk_IO = clk/64 (prescalar)
	
	TCCR2 = (1 << WGM21) | (1 << CS21); //set timer 2 clk to be clk/8
	OCR2 = 200;
	
	//Note OCR1B must be LESS THAN OCR1A
	OCR1A = 300;  //set PWM frequency  1/(OCR1A*clk_period*prescaler)
	//OCR1B = 300;   //set current hysteresis off time
	//OCR3A = 100;  
	//OCR3B = 100;   
	/************************Set up PWM timers***************************************/
}

void Init_ADC( void ){
	/************************Set up ADCs***************************************/

	//Set ADC data register to be right aligned
	//Set ADC MUX to read from ADC Channel 3
/*	ADMUX =
	(1 << REFS1) |	//set Reference to internal 2.56V
	(1 << REFS0) |
	(0 << ADLAR) |
	(0 << MUX4) |  //ADC => ADC3
	(0 << MUX3) |
	(0 << MUX2) |
	(1 << MUX1) |
	(1 << MUX0);*/
	
	//ADMUX &= 0xC0;
	//ADMUX |= 0x03;
	ADMUX = 0b11000011;


	ADCSRA =
	(1 << ADEN) | //enable ADC
	(1 << ADSC) | //Start free-running mode conversions
	(1 << ADATE) | //Enable auto-trigger
	(1 << ADIE) | //Enable Interrupt Flag
	(0 << ADPS2) | //Setting these three bits to zero sets the ADC clk to the processor_clk/2
	(0 << ADPS1) |
	(0 << ADPS0);
	
	ADCSRB =
	(0 << ADTS2) | //setting these three bits to zero enables free running mode of the ADC
	(0 << ADTS1) |
	(0 << ADTS0);
	

	
	//ADC ref voltage is 2.56 volts at 10 bit resolution
	// 1 bit on the ADC is 2.5mV input at the pin
	//0.1Ohm sense resistor -> 4[A]*0.1[Ohm] = 400[mV]
	//400 mV/2.5 -> I_max_ADC = 160
	/************************Set up ADCs***************************************/
	
}

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
void Motor_Initialization( void ){
	
	
	//Set direction registers for GPIO (1 -> output, 0 -> input)
	DDRA  = 0b11111111;
	DDRD  = 0b01010000;
	DDRG  = 0b00000100;
	//DDRD  = 0b10000000;
	DDRB  = 0b11110100; //set OC1A and OC1B to outputs for use with PWM
	PORTB = 0x00; //initialize PORTB to zero
	
	//Startup in a Disabled State
	Motor_Disable();
	

	
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



	/*
ISR(USART1_RX_vect)
{

	data = UDR1;
	UCSR1B &= ~(1 << RXCIE1);  //Disable receive char interrupt
	//USCR1B |= (1<<UDRIE1);
	//USART_Transmit(data);    //Using the simple send function we send one char at a time
		
		
	UCSR1A &= ~(1 << TXC1);
	UCSR1B |= (1 << TXCIE1);  //Enable send transmission complete interrupt


	LED1_off();
	_delay_ms(1000);
	
	
	
	//LED1_on();
	if(Receiving_Coords == YES){
		
		unsigned char tmp;//, stat;
		tmp= UDR1; // get the data
		if(Coordinates_updated == NOT_UPDATED){
			parsed_coord = atoi(tmp);
			Coordinates_updated = UPDATED;
		}
	
		if(Coordinates_verified == UNVERIFIED){
			if( tmp == 'Y' | tmp == 'y'){
				Coordinates_correct = YES;
			} else{
				Coordinates_correct = NO;
			}
		}
	
	
	}
}


ISR(USART1_TX_vect)
{
	
	UDR1 = data;
	UCSR1B &= ~(1 << TXCIE1);  //Disable send transmission complete interrupt
	UCSR1A &= ~(1<<UDRE1);
	UCSR1B |= (1 << RXCIE1);  //Enable receive char interrupt
	//LED1_on();
	//UCSR1A = (0 << TXC1);
}
*/

ISR(TIMER1_COMPA_vect) //interrupt service routine for timer1 compare A flag
	{
	
	

	if((move_motor == YES) & (motor_step_count < motor_movement_distance) & (motor_dir == POSITIVE)){
		//motor_moving = YES;
		// Full step code
		if(count == 0){
			count += 1;
			PWM1A = ON;
			PWM1B = OFF;
			PWM1A_on();
			PWM1B_off();
		}else if(count == 1){
			count += 1;
			PWM2A = ON;
			PWM2B = OFF;
			PWM2A_on();
			PWM2B_off();
		}else if(count == 2){
			count += 1;			
			PWM1A = OFF;
			PWM1B = ON;
			PWM1A_off();
			PWM1B_on();
		} else{
			PWM2A = OFF;
			PWM2B = ON;
			PWM2A_off();
			PWM2B_on();
			count = 0;
			motor_step_count += 1;
		} 
	}else if((move_motor == YES) & (motor_step_count < motor_movement_distance) & (motor_dir == NEGATIVE)){
		//motor_moving = YES;
		// Full step code
		if(count == 0){
			count += 1;
			PWM1A = ON;
			PWM1B = OFF;
			PWM1A_on();
			PWM1B_off();
			}else if(count == 1){
			count += 1;
			PWM2A = OFF;
			PWM2B = ON;
			PWM2A_off();
			PWM2B_on();
			}else if(count == 2){
			count += 1;
			PWM1A = OFF;
			PWM1B = ON;
			PWM1A_off();
			PWM1B_on();
			} else{
			PWM2A = ON;
			PWM2B = OFF;
			PWM2A_on();
			PWM2B_off();
			count = 0;
			motor_step_count += 1;
		}
	}
	else{
		move_motor = NO;
		motor_step_count = 0;
		PWM1A_off();
		PWM1B_off();
		PWM2A_off();
		PWM2B_off();
		//motor_moving = NO;
	}
	
	if(OCR1A > 180){
		OCR1A -= 1;
	}
		
/*	// Half step code
		if(count == 0){
			count += 1;
			PWM1A = ON;
			PWM1B = OFF;
			PWM1A_on();
			PWM1B_off();
			PWM2A = ON;
			PWM2B = OFF;
			PWM2A_on();
			PWM2B_off();
		}else if(count == 1){
			count += 1;
			PWM1A = OFF;
			PWM1B = OFF;
			PWM1A_off();
			PWM1B_off();
			PWM2A = ON;
			PWM2B = OFF;
			PWM2A_on();
			PWM2B_off();
		}else if(count == 2){
			count += 1;
			PWM1A = OFF;
			PWM1B = ON;
			PWM1A_off();
			PWM1B_on();
			PWM2A = ON;
			PWM2B = OFF;
			PWM2A_on();
			PWM2B_off();
		}else if(count == 3){
			count += 1;
			PWM1A = OFF;
			PWM1B = ON;
			PWM1A_off();
			PWM1B_on();
			PWM2A = OFF;
			PWM2B = OFF;
			PWM2A_off();
			PWM2B_off();
		}else if(count == 4){
			count += 1;
			PWM1A = OFF;
			PWM1B = ON;
			PWM1A_off();
			PWM1B_on();
			PWM2A = OFF;
			PWM2B = ON;
			PWM2A_off();
			PWM2B_on();
		}else if(count == 5){
			count += 1;
			PWM1A = OFF;
			PWM1B = OFF;
			PWM1A_off();
			PWM1B_off();
			PWM2A = OFF;
			PWM2B = ON;
			PWM2A_off();
			PWM2B_on();
		}else if(count == 6){
			count += 1;
			PWM1A = ON;
			PWM1B = OFF;
			PWM1A_on();
			PWM1B_off();
			PWM2A = OFF;
			PWM2B = ON;
			PWM2A_off();
			PWM2B_on();
		} else{
			count = 0;
			PWM1A = ON;
			PWM1B = OFF;
			PWM1A_on();
			PWM1B_off();
			PWM2A = OFF;
			PWM2B = OFF;
			PWM2A_off();
			PWM2B_off();
	}
	*/
	
	/*	if(iter == 100){
			iter = 0;
			if(OCR1A > 50){
				OCR1A -= 1;
			}
		}else{
			iter += 1;
		}
*/
	}

	ISR(TIMER2_COMP_vect) //interrupt service routine for timer1 compare B flag
	{
		
		led1_toggle();
		
		if(state1_hysterisis_delay == 1){
			state1 = POS_CUR;
			state1_hysterisis_delay = 0;
		}
		if(state2_hysterisis_delay == 1){
			state2 = POS_CUR;
			state2_hysterisis_delay = 0;
		}
		if(state1 == NEG_CUR){			
			state1_hysterisis_delay += 1;
		}
		if(state2 == NEG_CUR){
			state2_hysterisis_delay += 1;
		}
			
	
	}
	
//	ISR(TIMER3_COMPA_vect) //interrupt service routine for timer3 compare A flag
	//{

	//}

	//ISR(TIMER3_COMPB_vect) //interrupt service routine for timer1 compare B flag
	//{
	/*			PORTD ^= _BV(6);
				
				if(state1_hysterisis_delay == 1){
					state1 = POS_CUR;
					state1_hysterisis_delay = 0;
				}
				if(state2_hysterisis_delay == 1){
					state2 = POS_CUR;
					state2_hysterisis_delay = 0;
				}
				if(state1 == NEG_CUR){
					state1_hysterisis_delay += 1;
				}
				if(state2 == NEG_CUR){
					state2_hysterisis_delay += 1;
				}*/
	//}

	
	ISR(ADC_vect) 
	{
		

		
		//LED1_on();
		ADCval4 = ADCval3;
		ADCval3 = ADCval2;
		ADCval2 = ADCval1;
		ADCval1 = ADCval;
		ADCval = ADC;
		
		//ADC_avg = (3*ADCval)/4 + ADCval1/4;
		ADC_avg = ADC;
		
		if(move_motor == YES){
			if(ADMUX == 0b11000010){
/*else if ( (ADC_avg > 180) & (ADC_avg<200) ){
					if(state1 == POS_CUR){
						if( PWM1A == ON){
							PWM1A_on();
							PWM1B_off();
						}else if ( PWM1B == ON){
								PWM1B_on();
								PWM1A_off();
							}
					}else if(state1 == NEG_CUR){
							
							PWM1A_off();
							PWM1B_off();
							
						}
				}*/
				if (ADC_avg >=200){
					state1 = NEG_CUR;
					PWM1A_off();
					PWM1B_off();
					//TIMSK |= (1 << OCIE1B); //fire timer for current hysteresis off time
				
				}else{  //current =
					if(state1 == POS_CUR){  //current =
						if( PWM1A == ON){
							PWM1A_on();
							PWM1B_off();
						}else if ( PWM1B == ON){
							PWM1B_on();
							PWM1A_off();
						}
					}
				}
				/*}else{
					state1 = NEG_CUR;
					PWM1A_off();
					PWM1B_off();
					ETIMSK |= (1 << OCIE3A); //fire timer for current hysteresis off time
				}*/
			
				ADMUX = 0b11000011;
			}
			else if(ADMUX == 0b11000011){

/*else if ( (ADC_avg > 180) & (ADC_avg<200) ){
					if(state2 == POS_CUR){
						if( PWM2A == ON){
							PWM2A_on();
							PWM2B_off();
							}else if ( PWM2B == ON){
							PWM2B_on();
							PWM2A_off();
						}
						}else if(state2 == NEG_CUR){
			
						PWM2A_off();
						PWM2B_off();
					}
				}*/
				if (ADC_avg >=200){
					state2 = NEG_CUR;
					PWM2A_off();
					PWM2B_off();
					//TIMSK |= (1 << OCIE1B);
				
				}else{
					if(state2 == POS_CUR){  //current =
						if( PWM2A == ON){
							PWM2A_on();
							PWM2B_off();
							}else if ( PWM2B == ON){
							PWM2B_on();
							PWM2A_off();
						}
					}
				}/*else{
					state2 = NEG_CUR;
					PWM2A_off();
					PWM2B_off();
					ETIMSK |= (1 << OCIE3A);
				}*/
			
				ADMUX = 0b11000010;
			}
		}
		//}
/*		else{
			ADMUX &= 0xC0;	//clear ADCMUX channel selection
			ADMUX |= ((0 << MUX4)) | ((0 << MUX3)) | ((0 << MUX2)) | ((1 << MUX1)) | ((0 << MUX1));
		}*/
	
		//itoa(ADC_avg, buffer, 10);
		//USART_putstring(newline);
		//USART_putstring(buffer);
		
		
		
}
		
/*

//Multiplex through ADC readings
		switch(state){
			case 1:  //ADC 3
			ADMUX =
			(0 << MUX4) |
			(0 << MUX3) |
			(0 << MUX2) |
			(1 << MUX1) |
			(1 << MUX0);
			//state = 2;
			break;
			case 2:  //ADC 4
			ADMUX =
			(0 << MUX4) |
			(0 << MUX3) |
			(1 << MUX2) |
			(0 << MUX1) |
			(0 << MUX0);
			//state = 1;
			break;
			default:
			ADMUX =
			(0 << MUX4) |
			(0 << MUX3) |
			(0 << MUX2) |
			(1 << MUX1) |
			(1 << MUX0);
		}
		

			
			
	}
	*/


	
int main(void)
{
	
	cli(); 
	//char data_test = 'a';
	//unsigned int ubrr;	//Used to Initial Baud Rate
	//ubrr = MYUBRR;
	//int state=0;
	count = 0;
	Init_PWM();
	Init_ADC();
	
	Motor_Initialization();	
	Motor_Enable();
	//LED1_on();
	//LED2_on();
	//USART_Init(416); //Baud rate = 1200
	USART_Init(1); //Baud rate = 250000
	sei(); //Enable all interrupts
	
	//PORTB &= ~(_BV(2));
	USART_putstring(clr_screen);
	//_delay_ms(500);
	USART_putstring(Init_statement); //Print Startup Message over USB
	//USART_putstring("Moving To Desired Coordinates...\r\n");
	//USART_putstring("Moving To Desired Coordinates...\r\n");
	LED1_on();
	LED2_on();
	
	
		//PORTB &= ~(_BV(2));
		//PORTB |= (_BV(2));
		
		//USART_putstring("Moving To Desired Coordinates...\r\n");
		
	//motor_dir = NEGATIVE;	
	while(1){
		
		if( move_motor == NO){
			Axis1_x_coord_old = Axis1_x_coord;
			Axis1_y_coord_old = Axis1_y_coord;
			Receive_Coords();


			////SRAMchar = eeprom_read_byte(&NonVolatileChar);
			//SRAMint  = eeprom_read_word(&NonVolatileInt);
			//USART_putstring(newline);
			//itoa(SRAMint, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(newline);
			//
			//int i=0;
			//char * ptr;
			//ptr = &mydata;
			//for(i=0; i < 110; i++){
				//SRAMint = eeprom_read_byte(ptr); 
				//USART_putstring(newline);
				//itoa(SRAMint, buffer, 10);
				//USART_putstring(buffer);
				//ptr++;
			//}
			//USART_putstring(newline);
			//
//
			////USART_putstring(SRAMchar);
			////USART_putstring(newline);
			//eeprom_read_block((void*)&SRAMstring, (const void*)&NonVolatileString, 10);
			//USART_putstring(SRAMstring);
			
		}
		
		if( (Coordinates_correct == YES) & (Coordinates_verified == VERIFIED)){
			//USART_putstring("Moving To Desired Coordinates...\r\n");
			USART_putstring("OLD COORDS: (");
			itoa(Axis1_x_coord_old, buffer, 10);
			USART_putstring(buffer);
			USART_putstring(", ");
			itoa(Axis1_y_coord_old, buffer, 10);
			USART_putstring(buffer);
			USART_putstring(")");
			USART_putstring(newline);
			USART_putstring("NEW COORDS: (");
			itoa(Axis1_x_coord, buffer, 10);
			USART_putstring(buffer);
			USART_putstring(", ");
			itoa(Axis1_y_coord, buffer, 10);
			USART_putstring(buffer);
			USART_putstring(")");
			USART_putstring(newline);
			
			motor_movement_distance = 250*abs((Axis1_x_coord - Axis1_x_coord_old));
			if(Axis1_x_coord > Axis1_x_coord_old){
				motor_dir = POSITIVE;
			}else{
				motor_dir = NEGATIVE;
			}//}else{
			//	move_motor = NO;
			//}				
			move_motor = YES;
			Coordinates_updated = NOT_UPDATED;
			Coordinates_correct = NO;
			Coordinates_verified = UNVERIFIED;
		}
		
		

		/*if( (ADCSRA & (1<<ADIF)) == 1){
			LED2_on();
			}else{
			LED2_off();

		*/
	}
}

