#include "3140_concur.h"
#include "utils.h"
#include "lock.h"
#include "cond.h"

lock_t l;
lock_t m;
cond_t cr;
cond_t cw; 

unsigned int nr= 0;
unsigned int nw= 0;

void p1 (){	
	l_lock(&m);
	LEDGreen_Toggle();
	delay ();
	LEDGreen_Toggle();
	delay ();
	l_unlock(&m);
	
	delay();
	delay();
	delay();
	
	l_lock(&m);
	LEDGreen_Toggle();
	delay ();
	LEDGreen_Toggle();
	delay ();
	
	LEDGreen_Toggle();
	delay ();
	LEDGreen_Toggle();
	delay ();
	l_unlock(&m);
}

void reader (void) {
	l_lock(&l);
	if(nw!=0){
		c_wait(&l,&cr);
	}
	nr++;
	if(c_waiting(&l,&cr)){
		c_signal(&l,&cr);
	}
	else{
		l_unlock(&l);
	}	
	
	/*start reading*/
	p1();
	/*end reading*/
	
	l_lock(&l);
	nr--;
  	if(c_waiting(&l,&cw) && nr == 0){
  		c_signal(&l,&cw);
  	}
  	else if(c_waiting(&l,&cr)){
  		c_signal(&l,&cr);
  	}
  	else{
  		l_unlock(&l);	
  	}
}

void writer (void) {
	l_lock(&l);
	if(nw!=0 || nr!=0){
		c_wait(&l,&cw);
	}
	nw++;
	l_unlock(&l);
	
	/*start writing*/
	LEDRed_Toggle();
	delay();
	LEDRed_Toggle();
	delay();
	/*end writing*/
	
	l_lock(&l);
	nw--;
  	if(c_waiting(&l,&cw) && nr == 0){
  		c_signal(&l,&cw);
  	}
  	else if(c_waiting(&l,&cr)){
  		c_signal(&l,&cr);
  	}
  	else{
  		l_unlock(&l);	
  	}
}

int main (void){
    LED_Initialize();

	l_init (&l);
	l_init (&m);
	c_init (&l,&cr);
	c_init (&l,&cw);
 
	if (process_create (writer,20) < 0) {
	 	return -1;
	}
	if (process_create (reader,20) < 0) {
	 	return -1;
	}
	if (process_create (reader,20) < 0) {
	 	return -1;
	}
	if (process_create (reader,20) < 0) {
	 	return -1;
	}
	if (process_create (writer,20) < 0) {
	 	return -1;
	}

	process_start();

	LEDGreen_On();
	while (1) ;
}
