// Linked list module.

#ifndef LIST_H_
#define LIST_H_

struct nodeStruct {
    int memory_size;
    void* pointer_to_memory;
    int start_of_block;
    int end_of_block;

    struct nodeStruct *next;

};

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode( int size, void* poiner_to_memory,int initial);

/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node);

/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node);

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head);

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int item);

struct nodeStruct* find_smallest_chunk(struct nodeStruct *head);
struct nodeStruct* find_largest_chunk(struct nodeStruct *head);



/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */
void Unlink_Node(struct nodeStruct **headRef, struct nodeStruct *node);
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node);

/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef);

struct nodeStruct* List_findNodeNextNode(struct nodeStruct*  head, int start);



#endif
