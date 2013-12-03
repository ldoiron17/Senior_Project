#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "USART.h"

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