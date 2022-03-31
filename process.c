#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>
#include <assert.h>
#include <stdbool.h>

process_t *current_process = NULL;

bool first_select = true;

unsigned int *new_sp = NULL;

struct process_state {
	unsigned int * sp; // Current Stack pointer
	unsigned int * orig_sp; // Original Stack pointer
	int n; // Size allocated
};

typedef struct double_linked_list {
	process_t val;
	struct double_linked_list *prev;
	struct double_linked_list *next;
} double_linked_list;

struct double_linked_list *list_start = NULL;
struct double_linked_list *list_end = NULL;

// adds element to the beginning of the list
void add_elem_begin(double_linked_list * elem) {
	assert(list_start->prev == NULL);
	elem->next = list_start;
	elem->prev = NULL;
	list_start->prev = elem;
	list_start = elem;
}

// adds element to the end of the list
void add_elem_end(double_linked_list * elem) {
	if(list_start == NULL) {
		assert(list_end == NULL);
		list_start = elem;
		list_end = elem;

		elem->next = NULL;
		elem->prev = NULL;
	} else {
		assert(list_end->next == NULL);
		list_end->next = elem;
		elem->prev = list_end;

		list_end = elem;
		elem->next = NULL;
	}
}

// removes and returns first element of list, null if list is empty
double_linked_list * remove_first_elem() {
	struct double_linked_list *elem;
	if(list_start == NULL) {
		assert(list_end == NULL);
		elem = NULL;
	} else {
		assert(list_start->prev == NULL);
		elem = list_start;
		list_start = list_start->next;
		elem->next = NULL;
		assert(elem->prev == NULL);
		if(list_start != NULL) {
			list_start->prev = NULL;
		} else {
			list_end = NULL;
		}
	}

	return elem;
}

int process_create (void(*f) (void), int n){
	// Make an element for the queue containing info about the process
	double_linked_list *new_elem_ptr = malloc(sizeof(double_linked_list));
	if(new_elem_ptr == NULL){
		return -1;
	}
	new_elem_ptr->val.sp = process_stack_init(f, n);
	new_elem_ptr->val.orig_sp = new_elem_ptr->val.sp;
	new_elem_ptr->val.n = n;
	new_elem_ptr->prev = NULL;
	new_elem_ptr->next = NULL;

	if(new_elem_ptr->val.sp == NULL) return -1; // If the new stack cannot be allocated, return and error

	add_elem_end(new_elem_ptr); // Add the new element to the queue

	return 0;
}

void process_start (void){
	NVIC_EnableIRQ(PIT_IRQn); // Enable PIT Interupts

	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT
	PIT->MCR = 0x00; // Enable PIT timers

	PIT->CHANNEL[0].LDVAL = 0x00004E20; // 20k cycles @ 10Mhz = 2 ms before switching processes
	PIT->CHANNEL[0].TCTRL |= (1 << 1); // Enable interrupts for channel 0

	PIT->CHANNEL[0].TCTRL |= 0x1; //Enable channel 0

	process_begin(); // Initialize the first process
}

unsigned int * process_select(unsigned int * cursp){
	double_linked_list *fst = remove_first_elem(); //Check the first element
	if(cursp == NULL){ // Either the running process finished or it is the first time process_select is being called
		if(!first_select){ //If its not the first time, free memory from the running process
			process_stack_free(fst->val.orig_sp, fst->val.n);
			free(fst);
			current_process = NULL;
		}else{ // If it is the first time, put the element back in the front of the queue
			add_elem_begin(fst);
			first_select = false;
		}
	}else{
		//Put running process back into queue
		fst->val.sp = cursp;
		add_elem_end(fst);
	}

	//Get next process from linked list, return stack pointer
	if(list_start == NULL){
		return NULL;
	}
	current_process = &(list_start->val);
	return current_process->sp;
}