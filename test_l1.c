#include "3140_concur.h"
#include "utils.h"
#include "lock.h"


lock_t l;


void p1(void){
	int i= 0;
	while(i < 8){
		/*NCS*/
		delay();
		i++;
		/*CS*/
		l_lock(&l);
		LEDRed_Toggle();
		delay();
		LEDRed_Toggle();
		delay();
		l_unlock(&l);
	}
}

void p2(void){
	for(int i = 0; i < 10; i++){
		delay();
		l_lock(&l);
		l_unlock(&l);
	}
}


int main(void){
	LED_Initialize();           /* Initialize the LEDs           */	

	l_init (&l);
	
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	if (process_create (p2,20) < 0) {
		return -1;
	}
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	
	process_start();

	while(1);
	return 0;	
}
