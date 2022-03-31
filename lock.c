#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>
#include <assert.h>
#include <stdbool.h>
#include "shared_structs.h"

void l_init(lock_t* l){
	//PIT->CHANNEL[0].TCTRL = 1; //Disable interupts
	//init default values
	l->queue.list_start = NULL;
	l->queue.list_end = NULL;
	l->available = true;
	//PIT->CHANNEL[0].TCTRL = 3; //Enable interupts
}

void l_lock(lock_t* l){
	PIT->CHANNEL[0].TCTRL = 1; //Disable interupts
	if(l->available == false){
		//Change current process to show it has been blocked
		current_process->blocked = true;
		//add blocked process to list
		node *new_elem_ptr = malloc(sizeof(node));
		new_elem_ptr->val = *current_process;
		new_elem_ptr->prev = NULL;
		new_elem_ptr->next = NULL;
		add_elem_end(&(l->queue), new_elem_ptr);
		process_blocked();
	}else{
		l->available = false;
	}
	PIT->CHANNEL[0].TCTRL = 3; //Enable interupts
}

void l_unlock(lock_t* l){
	PIT->CHANNEL[0].TCTRL = 1; //Disable interupts
	//Add first blocked process back into the scheduler
	node *fst = remove_first_elem(&(l->queue));
	fst->val.blocked = false;
	add_elem_end(&scheduler, fst);

	l->available = true;
	PIT->CHANNEL[0].TCTRL =3; //Enable interupts
}
