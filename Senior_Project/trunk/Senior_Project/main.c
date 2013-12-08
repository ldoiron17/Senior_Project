



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> 
#include <stdlib.h>
#include <avr/eeprom.h>
#include "Communication.h"
#include "Init.h"
#include "Motor.h"
#include "ISR.h"
#include "defines.h"

//uint8_t  EEMEM NonChar="a";
uint16_t EEMEM NonInt=16;
uint8_t  EEMEM NonString[10]="This is 10";

# define F_CPU 8000000UL //8MHz clock speed, used for _delay_ms()

	
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
	//USART_Init(12); //Baud rate = 38400
	//USART_Init(51); //Baud rate = 9600
	sei(); //Enable all interrupts
	
	//PORTB &= ~(_BV(2));
	USART_putstring(clr_screen);
	//_delay_ms(500);
	USART_putstring(Init_statement); //Print Startup Message over USB
	//USART_putstring("Moving To Desired Coordinates...\r\n");
	//USART_putstring("Moving To Desired Coordinates...\r\n");
	LED1_on();
	LED2_on();
	int toggle=0;
	
		//PORTB &= ~(_BV(2));
		//PORTB |= (_BV(2));
		
		//USART_putstring("Moving To Desired Coordinates...\r\n");
		
	//motor_dir = NEGATIVE;	
	
	_delay_ms(50);
	
	while(1){
		
		//USART_Transmit('a');
		Receive_Gcode();
		//char tmp;
		//tmp = USART_Receive();
		
		//USART_putstring("CUNTFACE");
		//USART_Transmit("a");
		//USART_Transmit(USART_Receive());
		//USART_putstring("GOONS");
		//USART_putstring(newline);
		
			//if(toggle == 0){
				//G00(-12000, 1000, 1);
				//toggle = 1;
			//}
			//else if(toggle == 1){
				//G00(0, 1000, 1);
				//toggle = 0;
			//}
			//
			//_delay_ms(50);
		

		//Axis1_x_coord_old = Axis1_x_coord;
		//Axis1_y_coord_old = Axis1_y_coord;
		//Receive_Coords();
		//
		//
		//if( (Coordinates_correct == YES) & (Coordinates_verified == VERIFIED)){
			////USART_putstring("Moving To Desired Coordinates...\r\n");
			//USART_putstring("OLD COORDS: (");
			//itoa(Axis1_x_coord_old, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(", ");
			//itoa(Axis1_y_coord_old, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(")");
			//USART_putstring(newline);
			//USART_putstring("NEW COORDS: (");
			//itoa(Axis1_x_coord, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(", ");
			//itoa(Axis1_y_coord, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(")");
			//USART_putstring(newline);
			//
			////if(toggle == 0){
				////G00(-800, 1000, 1);
				////toggle = 1;
			////}
			////else if(toggle == 1){
				////G00(0, 1000, 1);
				////toggle = 0;
			////}
			//
			//stepx(POSITIVE, FULLSTEP);
			//
			//Coordinates_updated = NOT_UPDATED;
			//Coordinates_correct = NO;
			//Coordinates_verified = UNVERIFIED;
		//}
		//
		//
//
//
//
//
		//if( (Coordinates_correct == YES) & (Coordinates_verified == VERIFIED)){
			////USART_putstring("Moving To Desired Coordinates...\r\n");
			//USART_putstring("OLD COORDS: (");
			//itoa(Axis1_x_coord_old, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(", ");
			//itoa(Axis1_y_coord_old, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(")");
			//USART_putstring(newline);
			//USART_putstring("NEW COORDS: (");
			//itoa(Axis1_x_coord, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(", ");
			//itoa(Axis1_y_coord, buffer, 10);
			//USART_putstring(buffer);
			//USART_putstring(")");
			//USART_putstring(newline);
			//
			//motor_movement_distance = 250*abs((Axis1_x_coord - Axis1_x_coord_old));
			//if(Axis1_x_coord > Axis1_x_coord_old){
				//motor_dir = POSITIVE;
				//}else{
				//motor_dir = NEGATIVE;
				//}//}else{
				////	move_motor = NO;
				////}
				//move_motor = YES;
				//Coordinates_updated = NOT_UPDATED;
				//Coordinates_correct = NO;
				//Coordinates_verified = UNVERIFIED;
			//}
			
		/*if( (ADCSRA & (1<<ADIF)) == 1){
			LED2_on();
			}else{
			LED2_off();

		*/
	}
}

////SRAMchar = eeprom_read_byte(&NonChar);
//SRAMint  = eeprom_read_word(&NonInt);
//USART_putstring(newline);
//itoa(SRAMint, buffer, 10);
//USART_putstring(buffer);
//USART_putstring(newline);
//
//int i=0;
//char * ptr;
//ptr = &x_coord_data;
//	SRAMfloat = 3.234;


//	char* str[30];
//	float flt = 2.4567;

//for(i=0; i < sizeof(x_coord_data); i++){
//SRAMchar = eeprom_read_byte(ptr);
//eeprom_read_block((void*)&buffer, (const void*)ptr, 10);
//itoa(SRAMint, buffer, 10);
//	USART_putstring(str);
//	USART_putstring(newline);
//ptr++;
//}
//	USART_putstring(newline);