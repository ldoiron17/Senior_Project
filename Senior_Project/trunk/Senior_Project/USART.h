
//Prototype functions

void USART_Init( unsigned int ubrr );


void USART_Transmit(unsigned char data_send );


unsigned char USART_Receive( void );


void USART_Flush( void );


void USART_putstring(char* StringPtr);