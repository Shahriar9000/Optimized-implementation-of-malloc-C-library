#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

static _Bool doSinglePassOnSort(struct nodeStruct **headRef);
static void swapElements(struct nodeStruct **previous, struct nodeStruct *nodeA, struct nodeStruct *b);


/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode( int size, void* pointer_to_memory,int initial)
{
	struct nodeStruct *pNode = malloc(sizeof(struct nodeStruct));
	if (pNode != NULL) {
		pNode->memory_size = size;
		pNode->pointer_to_memory = pointer_to_memory; 
		pNode->start_of_block = initial;
		pNode->end_of_block = initial + size;
		pNode->next = NULL;
	}
	return pNode;
}

/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node)
{
	node->next = *headRef;
	*headRef = node;
}

/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node)
{
	node->next = NULL;

	// Handle empty list
	if (*headRef == NULL) {
		*headRef = node;
	}
	else {
		// Find the tail and insert node
		struct nodeStruct *current = *headRef;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
	}
}

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head)
{
	int count = 0;
	struct nodeStruct *current = head;
	while (current != NULL) {
		current = current->next;
		count++;
	}
	return count;
}

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int size)
{
	struct nodeStruct *current = head;
	while (current != NULL) {
		if (current->memory_size == size) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */

void Unlink_Node(struct nodeStruct **headRef, struct nodeStruct *node)
{
	assert(headRef != NULL);
	assert(*headRef != NULL);

	// Is it the first element?
	if (*headRef == node) {
		*headRef = node->next;
	}
	else {
		// Find the previous node:
		struct nodeStruct *previous = *headRef;
		while (previous->next != node) {
			previous = previous->next;
			assert(previous != NULL);
		}

		// Unlink node:
		assert(previous->next == node);
		previous->next = node->next;
	}

}

void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node)
{
	assert(headRef != NULL);
	assert(*headRef != NULL);

	// Is it the first element?
	if (*headRef == node) {
		*headRef = node->next;
	}
	else {
		// Find the previous node:
		struct nodeStruct *previous = *headRef;
		while (previous->next != node) {
			previous = previous->next;
			assert(previous != NULL);
		}

		// Unlink node:
		assert(previous->next == node);
		previous->next = node->next;
	}

	// Free memory:
	free(node);
}
/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 */
struct nodeStruct* find_smallest_chunk(struct nodeStruct *head)
{

	struct nodeStruct* ptr = head;
	
	while(head!= NULL)
	{
		if (head->memory_size < ptr->memory_size)
		{
			ptr = head;
		}
		head = head->next;
	}

	return ptr;

}


struct nodeStruct* find_largest_chunk(struct nodeStruct *head)
{
	
	struct nodeStruct* ptr = head;
	
	while(head!= NULL)
	{
		if (head->memory_size > ptr->memory_size)
		{
			ptr = head;
		}
		head = head->next;
	}

	return ptr;
}



void List_sort (struct nodeStruct **headRef)
{
	while (doSinglePassOnSort(headRef)) {
		// Do nothing: work done in loop condition.
	}
}
static _Bool doSinglePassOnSort(struct nodeStruct **headRef)
{
	_Bool didSwap = false;
	while (*headRef != NULL) {
		struct nodeStruct *nodeA = *headRef;
		// If we don't have 2 remaining elements, nothing to swap.
		if (nodeA->next == NULL) {
			break;
		}
		struct nodeStruct *nodeB = nodeA->next;

		// Swap needed?
		if (nodeA->memory_size > nodeB->memory_size){
			swapElements(headRef, nodeA, nodeB);
			didSwap = true;
		}

		// Advance to next elements
		headRef = &((*headRef)->next);
	}
	return didSwap;
}
static void swapElements(struct nodeStruct **previous,
		struct nodeStruct *nodeA,
		struct nodeStruct *nodeB)
{
	*previous = nodeB;
	nodeA->next = nodeB->next;
	nodeB->next = nodeA;
}


struct nodeStruct* List_findNodeNextNode(struct nodeStruct*  head, int start)
{
	assert(start >= 0);

	struct nodeStruct* node_ptr = NULL;
    	while( head != NULL )
		{	

			if( head->start_of_block > start )
			{
				if( (node_ptr == NULL) || (head->start_of_block < node_ptr->start_of_block) )
				{
					node_ptr= head;
				}
			}

			head = head->next;
		}

	return node_ptr;
}

