#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "3140_concur.h"


/** Implement your structs here */
struct process_state {
	unsigned int * sp; // Current Stack pointer
	unsigned int * orig_sp; // Original Stack pointer
	int n; // Size allocated
	bool blocked;
};

extern process_t * current_process;
extern process_t * process_queue;

typedef struct node {
	process_t val;
	struct node *prev;
	struct node *next;
} node;

typedef struct double_linked_list {
	struct node *list_start;
	struct node *list_end;
} double_linked_list;

extern double_linked_list scheduler;

// adds element to the beginning of the list
void add_elem_begin(double_linked_list * list, node * elem);

// adds element to the end of the list
void add_elem_end(double_linked_list * list, node * elem);

// removes and returns first element of list, null if list is empty
node * remove_first_elem(double_linked_list * list);
/**
 * This defines the lock structure
 */
typedef struct lock_state {
	struct double_linked_list queue;
	bool available;
} lock_t;

/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {

} cond_t;



#endif
