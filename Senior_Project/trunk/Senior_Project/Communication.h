
//Prototype functions
typedef struct {
	int format;
	char current_input;
	char xdata[7];
	char ydata[7];
	char zdata[7];
	char feedrate[7];
	char* exit_message, *xmessage, *ymessage, *zmessage, *fmessage;
	int final_data[4];
} var_check_T ;


void USART_Init( unsigned int ubrr );


void USART_Transmit(unsigned char data_send );


unsigned char USART_Receive( void );


void USART_Flush( void );


void USART_putstring(char* StringPtr);

void Receive_Coords( void );

void Receive_Gcode( void );

char clear_whitespace( void );

void scan_var( var_check_T * );

void check_format(var_check_T* s);

void verify_coords(var_check_T* s);

int convert_coord(char s[]);

int is_ascii_num(char var);

