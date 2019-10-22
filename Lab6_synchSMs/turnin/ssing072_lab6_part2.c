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
#define A0 (~PINA & 0x01)
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char B0 = 0x00;

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

enum states{Start, LedOn1, LedOn2, LedOn3, stop, wait1} state;
void Tick_Blink(){
	switch(state){
		case Start:
			state = LedOn1;
			break;
		case LedOn1:
			if(A0){
				state = stop;
			}
			else{
				state = LedOn2;
			}
			break;
		case LedOn2:
			if(A0){
				state = stop;
			}
			else{
				state = LedOn3;
			}
			break;
		case LedOn3:
			if(A0){
				state = stop;
			}
			else{
				state = LedOn1;
			}
		break;
		case stop:
		if(A0){
			state = stop;
		}
		else{
			state = wait1;
		}
		break;
		case wait1:
			if(A0){
				state = Start;
			}
			else{
				state = wait1;
			}
		break;
	}
	switch(state){
		case Start:
		break;
		case LedOn1:
		B0 = 0x01;
		break;
		case LedOn2:
		B0 = 0x02;
		break;
		case LedOn3:
		B0 = 0x04;
		break;
		case stop:
		break;
		case wait1:
		break;
	}
	PORTB = B0;
}

int main(void) {
	DDRB = 0xFF; DDRA = 0x00;
	PORTB = 0x00; PORTA = 0xFF;
	TimerSet(300);
	TimerOn();
	while (1) {
		Tick_Blink();
		while(!TimerFlag){};
		TimerFlag = 0;
	}
	return 1;
}
