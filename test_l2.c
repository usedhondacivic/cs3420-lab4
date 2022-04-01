#include "3140_concur.h"
#include "utils.h"
#include "lock.h"


lock_t red;
lock_t green;

void p1(void){
	while(true){
		l_lock(&red);
		//l_lock(&green);
		LEDRed_Toggle();
		delay();
		LEDRed_Toggle();
		delay();
		//l_unlock(&green);
		l_unlock(&red);
	}
}

void p2(void){
	while(true){
		l_lock(&red);
		//l_lock(&green);
		LEDGreen_Toggle();
		delay();
		LEDGreen_Toggle();
		delay();
		//l_unlock(&green);
		l_unlock(&red);
	}
}


void p3(void){
	delay();
	while(true){
		l_lock(&red);
		l_unlock(&red);
	}
}

void p4(void){
	l_lock(&red);
	while(true){
		delay();
	}
	l_unlock(&red);
}


int main(void){
	LED_Initialize();           /* Initialize the LEDs           */	

	l_init (&red);
	l_init (&green);
	
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	
	process_start();

	while(1);
	return 0;	
}
