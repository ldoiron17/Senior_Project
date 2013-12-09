


extern volatile signed int Axis1_x_coord;
extern volatile signed int Axis1_y_coord;
extern volatile signed int Axis2_x_coord;
extern volatile signed int Axis2_y_coord;
extern volatile signed int Axis3_x_coord;
extern volatile signed int Axis3_y_coord;
extern volatile signed int Axis1_x_coord_old;
extern volatile signed int Axis1_y_coord_old;
extern volatile int Controller_state;
extern volatile int Coordinates_updated;
extern volatile int Coordinates_verified;
extern volatile int Receiving_Coords;
extern volatile int Coordinates_correct;
extern volatile int parsed_coord;
extern volatile int state1_hysterisis_delay;
extern volatile int state2_hysterisis_delay;
extern volatile int PWM1A;
extern volatile int PWM1B;
extern volatile int PWM2A;
extern volatile int PWM2B;
extern volatile char buffer[10];       //Used to Store ADC value readings from itoa() of ADC register
extern volatile char Gcode_buffer[3];
extern volatile char *clr_screen;
extern volatile char *Init_statement;
extern volatile char *ding;
extern volatile char *Coord_request_x;
extern volatile char *Coord_request_y;
extern volatile char *Gcode_request;
extern volatile char *Verify_coords1;
extern volatile char *Verify_coords2;
extern volatile char *Verify_coords3;
extern volatile char *Verify_coords4;
extern volatile char *Verify_coords5;
extern volatile char *newline;
extern volatile char *clearline;
extern volatile int motor_movement_distance;
extern volatile int motor_dir;
extern volatile int motor_moving;
extern volatile int  move_motor;
extern volatile int motor_step_count;
extern volatile int ADC_count;
extern volatile int count;
extern volatile int iter;
extern volatile int state1;
extern volatile int state2;
extern volatile char data;
extern volatile uint16_t ADCval;
extern volatile uint16_t ADCval1;
extern volatile uint16_t ADCval2;
extern volatile uint16_t ADCval3;
extern volatile uint16_t ADCval4;
extern volatile uint16_t ADC_avg;
extern volatile uint8_t  SRAMchar;
extern volatile float SRAMfloat;
extern volatile uint16_t SRAMint;
extern volatile uint8_t  SRAMstring[10];
//extern volatile char mydatabuffer[11][10];
//extern volatile float EEMEM x_coord_data[ 629 ];
//extern volatile float EEMEM y_coord_data[ 629 ];

struct MOTOR_T{
	int dir;
	int current_position;
	int enabled;
	int step_size;
	int current_state;
	int limit;
};

extern struct MOTOR_T MOTOR1_X;
extern struct MOTOR_T MOTOR2_Y;
extern struct MOTOR_T MOTOR3_Z;


//Define constants
#define STATE0 0
#define STATE1 1
#define STATE2 2
#define STATE3 3
#define STATE4 4
#define STATE5 5
#define STATE6 6
#define STATE7 7
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define FULLSTEP 0
#define HALFSTEP 1
#define QUARTERSTEP 2
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
#define FALSE = 1;
#define TRUE = 2;

//LED functions for turning on and off status LED 1
#define LED1_on()  PORTD |= _BV(6)
#define LED1_off()  PORTD &= ~_BV(6)
#define led1_toggle() PORTD ^= _BV(6)

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