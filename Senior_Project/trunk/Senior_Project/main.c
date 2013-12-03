

# define F_CPU 8000000UL //8MHz clock speed, used for _delay_ms()

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h> 
#include <avr/eeprom.h>
#include "USART.h"
#include "Init.h"
#include "Motor.h"
#include "ISR.h"
#include "defines.h"

//uint8_t  EEMEM NonChar="a";
uint16_t EEMEM NonInt=16;
uint8_t  EEMEM NonString[10]="This is 10";

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


			////SRAMchar = eeprom_read_byte(&NonChar);
			//SRAMint  = eeprom_read_word(&NonInt);
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
			//eeprom_read_block((void*)&SRAMstring, (const void*)&NonString, 10);
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

