


#define F_CPU 8000000UL //8MHz clock speed, used for _delay_ms()

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
#include "Gcodes.h"
#include "defines.h"



volatile signed int chucktesta;

int main(void)
{
	
	cli(); 
	count = 0;
	Init_PWM();
	Init_ADC();
	Motor_Initialization();	
	//USART_Init(416); //Baud rate = 1200
	USART_Init(1); //Baud rate = 250000
	//USART_Init(12); //Baud rate = 38400
	//USART_Init(51); //Baud rate = 9600
	sei(); //Enable all interrupts
	USART_putstring(clr_screen);
	USART_putstring(Init_statement); //Print Startup Message over USB
	LED1_on();
	LED2_on();
	
	while(1){
		
		Receive_Gcode();
	}
}


//possibly useful old code

//uint8_t  EEMEM NonChar="a";
//uint16_t EEMEM NonInt=16;
//uint8_t  EEMEM NonString[10]="This is 10";


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