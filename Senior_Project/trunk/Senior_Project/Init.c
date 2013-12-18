

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "Init.h"
#include "Motor.h"
#include "defines.h"


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
	
	TCCR2 = (1 << WGM21) | (1 << CS21) | (1 << CS20); //set timer 2 clk to be clk/64
	OCR2 = 15; //used to control fixed off time of current control this number is 1/(clk/64)*OCR2 seconds
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

void Motor_Initialization( void ){
	
	
	//Set direction registers for GPIO (1 -> output, 0 -> input)
	DDRA  = 0b11111111;
	DDRD  = 0b01010000;
	DDRG  = 0b00000100;
	//DDRD  = 0b10000000;
	DDRB  = 0b11110100; //set OC1A and OC1B to outputs for use with PWM
	PORTB = 0x00; //initialize PORTB to zero
	
	//set limits to the axes of the CNC table
	MOTOR1_X.limit = 12000; //12000 MILS = 12 inches
	MOTOR2_Y.limit = 18000; //18000 MILS = 18 inches
	MOTOR3_Z.limit = 3000; //3000 MILS = 3 inches
	//Startup in a Disabled State
	Motor_Disable();
	

	
}