#include "3140_concur.h"
#include "utils.h"
#include "lock.h"


lock_t l;


void p1(void){
	int i= 0;
	while(true){
		/*NCS*/
		i++;
		/*CS*/
		l_lock(&l);
		LEDRed_Toggle();
		LEDRed_Toggle();
		LEDGreen_Toggle();
		LEDGreen_Toggle();
		l_unlock(&l);
	}
}

int main(void){
	LED_Initialize();           /* Initialize the LEDs           */	

	l_init (&l);
	
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	
	process_start();
	LEDGreen_On();

	while(1);
	return 0;	
}
