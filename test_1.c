#include "3140_concur.h"
#include "utils.h"

/**
 * Test case using many short processes and one long process.
 */

void p0 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    	LEDGreen_Toggle();
	}
}

void p1_1 (void)
{
	delay();
   	LEDRed_Toggle();
}

void p2_1 (void)
{
	for (int i=0; i < 2; i++) {
		delay();
	}
   	LEDRed_Toggle();
}

void p3 (void)
{
	for (int i=0; i < 3; i++) {
		delay();
	}
   	LEDRed_Toggle();
}

void p4 (void)
{
	for (int i=0; i < 4; i++) {
		delay();
	}
   	LEDRed_Toggle();
}

void p5 (void)
{
	for (int i=0; i < 5; i++) {
		delay();
	}
   	LEDRed_Toggle();
}


int main (void){
  LED_Initialize();
 

 if (process_create (p0,32) < 0) {
 	return -1;
 }

 if (process_create (p1_1,32) < 0) {
 	return -1;
 }

 if (process_create (p2_1,32) < 0) {
 	return -1;
 }

 if (process_create (p3,32) < 0) {
 	return -1;
 }

 if (process_create (p4,32) < 0) {
 	return -1;
 }

 if (process_create (p5,32) < 0) {
 	return -1;
 }

 	
 process_start ();

/*
  * After process start, the red led and green led should toggle at the same time for 5 toggles.
  * After each red blink, the frequency of the toggles should increase
*/


 while (1) ;

 return 0;
}
