/*******************************/
/* Author: MAHAMMAD HESHMAT    */
/* Date: 1-8-2022              */
/* Version: 1.0                */
/* Module: DIO                 */
/*******************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "STD_types.h"
#include "BIT_math.h"
#include "DIO_int.h"
#include "Timer_int.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include "FUNCTIONS.h"
#include "GIE_int.h"
#include "LCD_int.h"

f64 L_DISTANCE , R_DISTANCE ;

volatile u64 TimerOverflow = 0;
u64 count;
volatile f64 distance;


ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;		/* Increment Timer Overflow count */
}


void GO_FORWARD (void)
{
	DIO_voidSetPinValue( H_A1 , HIGH );
	DIO_voidSetPinValue( H_A2 , LOW );
	DIO_voidSetPinValue( H_A3 , HIGH );
	DIO_voidSetPinValue( H_A4 , LOW );
	
	DIO_voidSetPinValue( H_EN1 , HIGH );
	DIO_voidSetPinValue( H_EN2 ,HIGH );	
}

void BACKWORD (void)
{
	DIO_voidSetPinValue( H_A1 , LOW );
	DIO_voidSetPinValue( H_A2 , HIGH );
	DIO_voidSetPinValue( H_A3 , LOW );
	DIO_voidSetPinValue( H_A4 , HIGH );
	
	DIO_voidSetPinValue( H_EN1 , HIGH );
	DIO_voidSetPinValue( H_EN2 ,HIGH );
	
	_delay_ms(500);	
	
	STOP();
}

void STOP (void)
{
	DIO_voidSetPinValue( H_EN1 , LOW );
	DIO_voidSetPinValue( H_EN2 , LOW );
		
	DIO_voidSetPinValue( H_A1 , LOW );
	DIO_voidSetPinValue( H_A2 , LOW );
	DIO_voidSetPinValue( H_A3 , LOW );
	DIO_voidSetPinValue( H_A4 , LOW);

}

void RETURN_180 (void)
{
	DIO_voidSetPinValue( H_A1 , LOW );  // RIGHT MOTOR BACKWORD
	DIO_voidSetPinValue( H_A2 , HIGH ); 
	DIO_voidSetPinValue( H_A3 , HIGH ); // LEFT MOTOR FORWORD
	DIO_voidSetPinValue( H_A4 , LOW );	
	
	DIO_voidSetPinValue( H_EN1 , HIGH );
	DIO_voidSetPinValue( H_EN2 , HIGH );
	_delay_ms(1000);						//DESIRED TIME TO ROTATE 180 
	
	STOP ();                                      
}

void TURN_RIGHT (void)
{	
	DIO_voidSetPinValue( H_A1 , LOW );
	DIO_voidSetPinValue( H_A2 , LOW );
	DIO_voidSetPinValue( H_A3 , HIGH );
	DIO_voidSetPinValue( H_A4 , LOW );
	
	DIO_voidSetPinValue( H_EN1 , HIGH);
	DIO_voidSetPinValue( H_EN2 , HIGH);
	_delay_ms(1000);
	STOP ();  
}

void TURN_LEFT (void)
{
	DIO_voidSetPinValue( H_A1 , HIGH);
	DIO_voidSetPinValue( H_A2 , LOW );
	DIO_voidSetPinValue( H_A3 , LOW );
	DIO_voidSetPinValue( H_A4 , LOW );
	
	DIO_voidSetPinValue( H_EN1 , HIGH);
	DIO_voidSetPinValue( H_EN2 , HIGH);
	_delay_ms(1000);
	STOP ();  
}

void CHECK(void)
{
	LCD_voidClear();
	LCD_voidSetCursor( LCD_U8_LINE1 , 2 );
	LCD_voidSendString("CHICKING...");
	
	TIMER0_void_SetCompareVal(24);
	_delay_ms(100);
	TIMER0_void_SetCompareVal(13);
	L_DISTANCE = ULTRASONIC_f64Distance();
	_delay_ms(300);
	TIMER0_void_SetCompareVal(34);
	R_DISTANCE = ULTRASONIC_f64Distance();
	_delay_ms(300);
	TIMER0_void_SetCompareVal(24);
	
	if ((L_DISTANCE < 10.00 ) && (R_DISTANCE < 10.00))
	{ 
		LCD_voidSetCursor( LCD_U8_LINE2 , 2 );
		LCD_voidSendString("RETURN BACK");
		BACKWORD(); RETURN_180(); 
	}
	else if (L_DISTANCE > R_DISTANCE)
	{ 
		LCD_voidSetCursor( LCD_U8_LINE2 , 4 );
		LCD_voidSendString("TURN LEFT");
		BACKWORD(); TURN_LEFT(); 
	}
	else if (R_DISTANCE > L_DISTANCE)
	{ 
		LCD_voidSetCursor( LCD_U8_LINE2 , 3 );
		LCD_voidSendString("TURN RIGHT");
		BACKWORD(); TURN_RIGHT(); 
	}
	LCD_voidClear();
	LCD_voidSetCursor( LCD_U8_LINE1 , 1 );
	LCD_voidSendString("OBSTACLE AFTER");
	LCD_voidSetCursor( LCD_U8_LINE2 , 9 );
	LCD_voidSendString("cm");
}


f64 ULTRASONIC_f64Distance(void)
{
	DIO_voidSetPinValue(DIO_U8_PIN8,DIO_U8_HIGH);
	_delay_us(10);
	DIO_voidSetPinValue(DIO_U8_PIN8,DIO_U8_LOW);
	
	TCNT1 = 0;			// Clear Timer counter
	TCCR1B = 0b01000010 ;		// Setting for capture rising edge,  using clk/8 pre-scaler
	TIFR = 1<<ICF1;		// Clear ICP flag (Input Capture flag) 
	TIFR = 1<<TOV1;		// Clear Timer Overflow flag 
    
	/*Calculate width of Echo by Input Capture (ICP) on PortD PD6*/ 
	while ((TIFR & (1 << ICF1)) == 0);	// Wait for rising edge 
	
	TCNT1 = 0;			// Clear Timer counter 
	TCCR1B = 0b00000010 ;		// Setting for capture falling edge, using clk/8 pre-scaler 
	TIFR = 1<<ICF1;		// Clear ICP flag (Input Capture flag) 
	TIFR = 1<<TOV1;		// Clear Timer Overflow flag 
	
	TimerOverflow = 0;	// Clear Timer overflow count 

	while ((TIFR & (1 << ICF1)) == 0); // Wait for falling edge 
	
	count = ICR1 + (65535 * TimerOverflow);	// Take value of capture register
	distance = ((double)count * 0.008575 ) ;   // 8 MHz Timer freq, sound speed =343 m/s
	_delay_ms(100);
	
	return distance;
}