#include <stdlib.h>
#include <MKL46Z4.h>
#include <stdbool.h>
#include "shared_structs.h"

process_t *current_process = NULL;

bool first_select = true;

unsigned int *new_sp = NULL;

struct double_linked_list scheduler = {
	.list_start = NULL,
	.list_end = NULL
};


// adds element to the beginning of the list
void add_elem_begin(double_linked_list * list, node * elem) {
	assert(list->list_start->prev == NULL);
	elem->next = list->list_start;
	elem->prev = NULL;
	list->list_start->prev = elem;
	list->list_start = elem;
}

// adds element to the end of the list
void add_elem_end(double_linked_list * list, node * elem) {
	if(list->list_start == NULL) {
		assert(list->list_end == NULL);
		list->list_start = elem;
		list->list_end = elem;

		elem->next = NULL;
		elem->prev = NULL;
	} else {
		assert(list->list_end->next == NULL);
		list->list_end->next = elem;
		elem->prev = list->list_end;

		list->list_end = elem;
		elem->next = NULL;
	}
}

// removes and returns first element of list, null if list is empty
node * remove_first_elem(double_linked_list * list) {
	struct node *elem;
	if(list->list_start == NULL) {
		assert(list->list_end == NULL);
		elem = NULL;
	} else {
		assert(list->list_start->prev == NULL);
		elem = list->list_start;
		list->list_start = list->list_start->next;
		elem->next = NULL;
		assert(elem->prev == NULL);
		if(list->list_start != NULL) {
			list->list_start->prev = NULL;
		} else {
			list->list_end = NULL;
		}
	}

	return elem;
}

int process_create (void(*f) (void), int n){
	// Make an element for the queue containing info about the process
	node *new_elem_ptr = malloc(sizeof(node));
	new_elem_ptr->val = malloc(sizeof(process_t));

	if(new_elem_ptr == NULL || new_elem_ptr->val == NULL){
		return -1;
	}
	new_elem_ptr->val->sp = process_stack_init(f, n);
	new_elem_ptr->val->orig_sp = new_elem_ptr->val->sp;
	new_elem_ptr->val->n = n;
	new_elem_ptr->val->blocked = false;
	new_elem_ptr->prev = NULL;
	new_elem_ptr->next = NULL;

	if(new_elem_ptr->val->sp == NULL) return -1; // If the new stack cannot be allocated, return and error

	add_elem_end(&scheduler, new_elem_ptr); // Add the new element to the queue

	return 0;
}

void process_start (void){
	NVIC_EnableIRQ(PIT_IRQn); // Enable PIT Interupts

	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT
	PIT->MCR = 0x00; // Enable PIT timers

	PIT->CHANNEL[0].LDVAL = 0x00004E20; // 20k cycles @ 10Mhz = 2 ms before switching processes
	//PIT->CHANNEL[0].LDVAL = 0x001E8480;
	PIT->CHANNEL[0].TCTRL |= (1 << 1); // Enable interrupts for channel 0

	PIT->CHANNEL[0].TCTRL |= 0x1; //Enable channel 0

	process_begin(); // Initialize the first process
}

unsigned int * process_select(unsigned int * cursp){
	node *fst = remove_first_elem(&scheduler); //Check the first element
	if(current_process != NULL){
		assert(fst->val == current_process);
	}
	if(cursp == NULL){ // Either the running process finished or it is the first time process_select is being called
		if(!first_select){ //If its not the first time, free memory from the running process
			process_stack_free(current_process->orig_sp, current_process->n);
			free(fst);
			current_process = NULL;
		}else{ // If it is the first time, put the element back in the front of the queue
			add_elem_begin(&scheduler, fst);
			first_select = false;
		}
	}else{
		current_process->sp = cursp;
		//Put running process back into queue, but only if it isn't blocked
		if(!(current_process->blocked)){
			add_elem_end(&scheduler, fst);
		}else{
			free(fst);
		}
	}

	//Get next process from linked list, return stack pointer
	if(scheduler.list_start == NULL){
		return NULL;
	}
	current_process = scheduler.list_start->val;
	return current_process->sp;
}
