/*
 * Lab6.c
 *
 * Created: 10/22/2019 12:08:47 PM
 * Author : ssury
 */ 

/*	Author: ssing072
 *  Partner(s) Name: Neha Gupta
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <arv/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
enum states{INIT, LEDONE, LEDTWO, LEDTHREE} state;
#define A0 (~PINA & 0x01)
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;  
}
    
void TimerOff(){
    TCCR1B = 0x00;
}

void TimerISR(){
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void runThing(unsigned char tmpB){
	switch(state){
		case INIT:
		state = LEDONE;
		break;
		case LEDONE:
		state = LEDTWO;
		break;
		case LEDTWO:
		state = LEDTHREE;
		break;
		case LEDTHREE:
		state = LEDONE;
		break;
	}
	switch(state){
		case INIT:
		tmpB = 0x00;
		break;
		case LEDONE:
		tmpB = 0x01;
		break;
		case LEDTWO:
		tmpB = 0x02;
		break;
		case LEDTHREE:
		tmpB = 0x04;
		break;
	}
	PORTB = tmpB;
	
}

int main(void) {
    
    DDRB = 0xFF; DDRA = 0x00;
    PORTB = 0x00; PORTA = 0xFF;
    TimerSet(300);
    TimerOn();
    unsigned char tmpB = 0x00;

    while (1) {
        runThing(tmpB);
        while(!TimerFlag){};
        TimerFlag = 0;
    }
    return 1;
}
