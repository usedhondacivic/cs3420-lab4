#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>
#include <assert.h>
#include <stdbool.h>
#include "shared_structs.h"

void l_init(lock_t* l){
	//init default values

}

void l_lock(lock_t* l){
	PIT->CHANNEL[0].TCTRL = 1; //Disable interupts
	if(l->available == false){
		//Change current process to show it has been blocked
		current_process->blocked = true;
		//add blocked process to list
		//Make sure this process doesn't get put back into the scheduler (change process_select)
		process_blocked();
	}else{
		l->available = true;
	}
	PIT->CHANNEL[0].TCTRL = 3; //Enable interupts
}

void l_unlock(lock_t* l){
	PIT->CHANNEL[0].TCTRL = 1; //Disable interupts
	//Add first blocked process back into the scheduler
	l->available = true;
	PIT->CHANNEL[0].TCTRL =3; //Enable interupts
}
