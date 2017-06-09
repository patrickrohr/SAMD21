/*
* LinkedList.c
*
* Created: 6/8/2017 6:37:14 PM
*  Author: Patrick Rohr
*/ 


#include "LinkedList.h"
//#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <FreeRTOS.h>



struct LinkedList * linked_list_initialize()
{
	struct LinkedList * linked_list_ptr = pvPortMalloc(sizeof *linked_list_ptr);
	linked_list_ptr->first = NULL; // There has to be a better way to do this. Maybe use calloc?
	linked_list_ptr->last = NULL;
	return linked_list_ptr;
}

struct LinkedList * linked_list_push_back_node(struct LinkedList * this, void * data)
{
	struct Node * node_ptr = pvPortMalloc(sizeof *node_ptr);
	node_ptr->data = data;
	node_ptr->next = NULL;

	if (this->first == NULL) // new LinkedList
	{
		// for a new list, the added node will be the first and the last node. the added node also won't have a preceding node.
		this->first = node_ptr;
		node_ptr->prev = NULL;
	} else
	{
		this->last->next = node_ptr;
		node_ptr->prev = this->last;
	}
	
	// take old last item and set address
	this->last = node_ptr;
	return this;
}

void linked_list_delete_node(struct LinkedList * this, struct Node * node_to_be_deleted)
{
	struct Node * node_iter = this->first;
	if(node_iter != NULL) // if there is data in this list
	{
		do
		{
			if(node_iter == node_to_be_deleted)
			{
				// close the gap
				node_iter->prev->next = node_iter->next;
				node_iter->next->prev = node_iter->prev;
				//free heap memory
				vPortFree(node_iter);
				return;
			}
			node_iter = node_iter->next;
		} while(node_iter != NULL);

	}
}