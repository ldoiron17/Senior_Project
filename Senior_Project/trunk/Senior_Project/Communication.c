#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "Communication.h"
#include "defines.h"



//Define Functions
void USART_Init( unsigned int ubrr )
{
	
	
	/* Set baud rate */
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);//|(1<<RXCIE1)|(1<<UDRIE1);//|(1<<TXCIE1);
	/* Set frame format: 8data, 2stop bit */
	UCSR1C = (1<<USBS1)|(3<<UCSZ10)|(1<<UPM11); //even parity
	
	//UCSR1A = 0; //asynchronous normal mode
}



void USART_Transmit(unsigned char data_send )
{
	
	UCSR1B |= (1<<TXEN1);
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) )
	;
	/* Put data into buffer, sends the data */
	UDR1 = data_send;
	
	UCSR1B &= ~(1<<TXEN1);
	
	unsigned char dummy;
	while ( UCSR1A & (1<<RXC1) ) {
		dummy = UDR1;
	}
	/* Wait for empty transmit buffer */
	//while ( !( UCSR1A & (1<<UDRE1)) )
	//;
	
	//UCSR1A = UCSR1A | (1 << TXC1);

	
}

unsigned char USART_Receive( void )
{
	UCSR1B &= ~(1<<TXEN1); //Disable USART TX pin
	unsigned char dummy, tmp;
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) );
	/* Get and return received data from buffer */
	tmp = UDR1;
	//Flush out the UDR1 buffer
	while ( UCSR1A & (1<<RXC1) ) {
		dummy = UDR1;
	}
	
	return tmp;
}

void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSR1A & (1<<RXC1) ) {
		dummy = UDR1;
	}
	UCSR1A = 0;
}



void USART_putstring(char* StringPtr){
	
	//UCSR1B &= ~(1 << RXCIE1);  //Disable receive char interrupt
	
	while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		USART_Transmit(*StringPtr);    //Using the simple send function we send one char at a time
	StringPtr++;}        //We increment the pointer so we can read the next char
	
	//UCSR1A &= ~(1 << TXC1);
	//UCSR1B |= (1 << TXCIE1);  //Enable send transmission complete interrupt
	
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


void Receive_Gcode( void ){
	
	//Example G01:
	//G01 X1Y1Z1F5
		
	char *tmp;
	
	var_check_T *s = (var_check_T *) calloc(1,sizeof(var_check_T));
	s->exit_message = "WHAT THE HELL IS YOUR PROBLEM?!";
	s->format = 0;
	
	
	//Ask for Gcode Command
	USART_putstring(Gcode_request);
	USART_putstring(newline);

	
	int i;
	for(i = 0; i < 3; i++){
		Gcode_buffer[i] = USART_Receive();
		USART_Transmit(Gcode_buffer[i]);
	}
		
	if( strcmp(&Gcode_buffer, "G01") == 0){  //If input is G01 command
		

		scan_var(s); //Read input coordinates
		if(s->format == 0){ //If there was user error in the input
			USART_putstring(newline);
			USART_putstring(ding);
			USART_putstring(s->exit_message);
			USART_putstring(newline);
		}else{
			check_format(s); // Make sure coordinates are in correct format and within bounds of machine size
			if(s->format == 0){ //If there was user error in the input
				USART_putstring(newline);
				USART_putstring(ding);
				USART_putstring(s->exit_message);
				USART_putstring(newline);
			}else{
				USART_putstring(newline);
				USART_putstring("Moving to (");
				USART_putstring(s->xdata);
				USART_putstring(", ");
				USART_putstring(s->ydata);
				USART_putstring(", ");
				USART_putstring(s->zdata);
				USART_putstring(") at a feedrate of ");
				USART_putstring(s->feedrate);
				USART_putstring(" IPM..");
				USART_putstring(newline);
			}
		}
		
		
	}else{
		USART_putstring(newline);
		USART_putstring(ding);
		USART_putstring("Incorrect Gcode Command, Try Again.");
		USART_putstring(newline);
		Receive_Gcode();
	}

	//Free s from memory and set pointer to NULL
	free(s);
	s = NULL;
	
	
}

char clear_whitespace(){
	
		//Read blank spaces 
		char tmp;
		tmp = USART_Receive();
		USART_Transmit(tmp);
		while(tmp == ' '){
			tmp = USART_Receive(); 
			USART_Transmit(tmp);
		}
		return tmp;
}

void scan_var(var_check_T* s){
	
	
	int i;
	uint8_t buf;
	s->format = 1;
	s->exit_message = "";
	s->xmessage = "";
	s->ymessage = "";
	s->zmessage = "";
	s->fmessage = "";
	s->current_input = '0';
	s->xdata[7] = "";
	s->ydata[7] ="";
	s->zdata[7] = "";
	s->feedrate[7] = "";
		

	while(1){
		
		buf = USART_Receive();
		

		if(buf == 0xD ){  // 0xD -> carriage return
			break;		
		}
		
		if(buf == 3 ){  // 3 = CTRL-C
			s->exit_message = "NOPE\r\n~Chuck Testa~";
		}
		
		else if(buf == ' '){
			USART_Transmit(buf); //Ignore blank spaces
		}
		
		else{
			USART_Transmit(buf);
			if(buf == 'X' | buf == 'x'){
				s->current_input = 'x';
				i = 0;
			}else if(buf == 'Y' | buf == 'y'){
				s->current_input = 'y';
				i = 0;
			} else if(buf == 'Z' | buf == 'z'){
				s->current_input = 'z';
				i = 0;
			} else if(buf == 'F' | buf == 'f'){
				s->current_input = 'f';
				i = 0;
			}else{
				if(s->current_input == 'x'){
					if((is_ascii_num(buf) == 1) | (buf == '.')){
						if(i == 6){
							s->xmessage = "Truncated";
							s->current_input = '0';
						}else{
							s->xdata[i] = buf;
							i++;
						}
					}else{
						s->exit_message = "Incorrect format for X coordinate";
					}
				}else if(s->current_input == 'y'){
					if((is_ascii_num(buf) == 1) | (buf == '.')){
						if(i == 6){
							s->ymessage = "Truncated";
							s->current_input = '0';
						}else{
							s->ydata[i] = buf;
							i++;
						}
						
					}else{
						s->exit_message = "Incorrect format for Y coordinate";
					}
				}else if(s->current_input == 'z'){
					if((is_ascii_num(buf) == 1) | (buf == '.')){
						if(i == 6){
							s->ymessage = "Truncated";
							s->current_input = '0';
						}else{
							s->zdata[i] = buf;
							i++;
						}
						
					}else{
						s->exit_message = "Incorrect format for Z coordinate";
					}				
				}else if(s->current_input == 'f'){
				if((is_ascii_num(buf) == 1) | (buf == '.')){
					if(i == 6){
						s->fmessage = "Truncated";
						s->current_input = '0';
					}else{
						s->feedrate[i] = buf;
						i++;
					}
					
					}else{
						s->exit_message = "Incorrect feedrate format";
					}
				}else{
					//do something?
					}
				}

			}
			
			if( s->exit_message != ""){
				s->format = 0;
				break;
			}
		
		}
	
	return s;
}



void check_format(var_check_T* s){
	
	
	int i;
	s->format = 0;// incorrect format
	int b;

	if((s->xdata[0] == '.') & (is_ascii_num(s->xdata[1]) == 1)){
			
		 if((is_ascii_num(s->xdata[2]) == 1) & (is_ascii_num(s->xdata[3]) == 1) & ((is_ascii_num(s->xdata[4]) == 1) | s->xdata[4] == 0) & ((is_ascii_num(s->xdata[5]) == 1) | s->xdata[5] == 0) ){
			s->xdata[5] = s->xdata[3];
			s->xdata[4] = s->xdata[2];
			s->xdata[3] = s->xdata[1];
			s->xdata[2] = '.';
			s->xdata[1] = '0';
			s->xdata[0] = '0';
			s->format = 1;//Correct format		
		 }
	}else if( (is_ascii_num(s->xdata[0]) == 1) & (s->xdata[1] == '.') ){
		if((is_ascii_num(s->xdata[2]) == 1) & ((is_ascii_num(s->xdata[3]) == 1) | s->xdata[3] == 0) & ((is_ascii_num(s->xdata[4]) == 1) | s->xdata[4] == 0) & ((is_ascii_num(s->xdata[5]) == 1) | s->xdata[5] == 0) ){
			s->xdata[5] = s->xdata[4];
			s->xdata[4] = s->xdata[3];
			s->xdata[3] = s->xdata[2];
			s->xdata[2] = '.';
			s->xdata[1] = s->xdata[0];
			s->xdata[0] = '0';
			s->format = 1;//Correct format
		}
			 
	}else if( (is_ascii_num(s->xdata[0]) == 1) & (is_ascii_num(s->xdata[1]) == 1) & (s->xdata[2] == '.') & ( (is_ascii_num(s->xdata[3]) == 1) | s->xdata[3] == 0) & ( (is_ascii_num(s->xdata[4]) == 1) | s->xdata[4] == 0) & ( (is_ascii_num(s->xdata[5]) == 1) | s->xdata[5] == 0) ){
		s->format = 1;//Correct format
	}else if( (is_ascii_num(s->xdata[0]) == 1) & (is_ascii_num(s->xdata[1]) == 1) & (s->xdata[2] == 0) & (s->xdata[3] == 0) & (s->xdata[4] == 0) & (s->xdata[5] == 0)){
		s->xdata[2] = '.';
		s->xdata[3] = '0';
		s->xdata[4] = '0';
		s->xdata[5] = '0';
		s->format = 1;//Correct format
	}else if( (is_ascii_num(s->xdata[0]) == 1) & (s->xdata[1] == 0) & (s->xdata[2] == 0) & (s->xdata[3] == 0) & (s->xdata[4] == 0) & (s->xdata[5] == 0)){
		s->xdata[1] = s->xdata[0];
		s->xdata[0] = '0';
		s->xdata[2] = '.';
		s->xdata[3] = '0';
		s->xdata[4] = '0';
		s->xdata[5] = '0';
		s->format = 1;//Correct format
	}else{
	s->exit_message = "X coordinate is not in the correct format, please input coordinates as XX.xxx \r\nWhere XX is the integer portion and xxx is the decimal portion.\r\nNote the precision is limited to 1 MIL and extra decimals will be truncated.";
}
	
	
	//
	////If y coordinate is in the triple digits
	//if((is_ascii_num(s->ydata[0]) == 1) & (is_ascii_num(s->ydata[1]) == 1) & (is_ascii_num(s->ydata[2]) == 1)){
		//s->format = 0;// incorrect format
		//s->exit_message = "Y coordinate is not in the correct format, please input coordinates as XX.xxx \r\nWhere XX is the integer portion and xxx is the decimal portion.\r\nNote the precision is limited to 1 MIL and extra decimals will be truncated.";
	//}
		//
	////If z coordinate is in the triple digits
	//if((is_ascii_num(s->zdata[0]) == 1) & (is_ascii_num(s->zdata[1]) == 1) & (is_ascii_num(s->zdata[2]) == 1)){
		//s->format = 0;// incorrect format
		//s->exit_message = "Z coordinate is not in the correct format, please input coordinates as XX.xxx \r\nWhere XX is the integer portion and xxx is the decimal portion.\r\nNote the precision is limited to 1 MIL and extra decimals will be truncated.";
	//}
	
	
	
	
}





int is_ascii_num(char var){
	
	if( var == '0' |
	    var == '1' |
		var == '2' |
		var == '3' |
		var == '4' |
		var == '5' |
		var == '6' |
		var == '7' |
		var == '8' |
		var == '9'
	){
		return 1;
	}else{
		return 0;
	}
}