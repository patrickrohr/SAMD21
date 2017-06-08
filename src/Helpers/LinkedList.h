/*
* LinkedList.h
*
* Created: 6/8/2017 6:37:22 PM
*  Author: Patrick Rohr
*/ 


#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__


// this linked list will dynamically allocate memory on the heap. watch out for memory leaks.
struct Node {
	void * data;
	struct Node * prev;
	struct Node * next;
};

// LinkedList as a pointer to Node pointer 
struct LinkedList {
	struct Node * first;
	struct Node * last;
};

struct LinkedList * linked_list_initialize();
struct LinkedList * linked_list_push_back_node(struct LinkedList * this, void * data);
void linked_list_delete_node(struct LinkedList * this, struct Node * node_to_be_deleted);

#endif