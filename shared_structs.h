#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

#include <stdlib.h>
#include <stdbool.h>

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

/** Implement your structs here */
struct process_state {
	unsigned int * sp; // Current Stack pointer
	unsigned int * orig_sp; // Original Stack pointer
	int n; // Size allocated
	bool blocked;
};
/**
 * This defines the lock structure
 */
typedef struct lock_state {
	struct double_linked_list *queue;
	bool available;
} lock_t;

/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {

} cond_t;



#endif
