//ISR.c

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "Motor.h"
#include "defines.h"



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