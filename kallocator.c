#include "kallocator.h"


 struct KAllocator {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    struct nodeStruct* allocated_memory_list;
    struct nodeStruct* free_memory_list;
    // Some other data members you want, 
    // such as lists to record allocated/free memory
};

struct KAllocator kallocator;


void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm;
    kallocator.size = _size;
    kallocator.memory = malloc((size_t)kallocator.size);
    kallocator.allocated_memory_list = NULL;
    kallocator.free_memory_list = List_createNode(kallocator.size, kallocator.memory, 0);

    
}

void destroy_allocator() {
    free(kallocator.memory);

    while(kallocator.allocated_memory_list != NULL)
    {
    	List_deleteNode (&kallocator.allocated_memory_list, kallocator.allocated_memory_list);
    }
    while(kallocator.free_memory_list != NULL)
    {
    	List_deleteNode (&kallocator.free_memory_list, kallocator.free_memory_list);
    }
}

void* FIRST_FIT_ALLOCATION(int _size)
{
	void* ptr = NULL;
	struct nodeStruct* free_frame = kallocator.free_memory_list;
	struct nodeStruct* first_fit = NULL;   		
    

    	if (_size < free_frame->memory_size)
    	{
    		first_fit = free_frame;
    	}
    	else
    	{	
    		

    		while(free_frame!= NULL)
    		{
    			if (_size <= free_frame->memory_size)
    			{
    				if (first_fit==NULL)
    				{
    					first_fit = free_frame;
    				}

    				if (first_fit->start_of_block > free_frame->start_of_block)
    				{
    					first_fit = free_frame;
    				}

    			}

    			free_frame = free_frame->next;
    			
    		}

    	}

    	if (first_fit == NULL)
    	{
    		return ptr;
    	}
    	else
    	{
    		int remaining_size = first_fit->memory_size- _size;
    		if (remaining_size != 0)
    		{
    			struct nodeStruct* new_frame = List_createNode (_size ,first_fit->pointer_to_memory, first_fit->start_of_block );
    			List_insertHead(&kallocator.allocated_memory_list, new_frame);
    			

    			first_fit->memory_size += -(_size);
    			first_fit->pointer_to_memory -= -(_size);
    			first_fit->start_of_block -= -(_size);
    			

    			ptr = new_frame->pointer_to_memory;
    			
    		}
    		else
    		{
    			Unlink_Node(&kallocator.free_memory_list , first_fit);
    			List_insertHead(&kallocator.allocated_memory_list, first_fit);
    			ptr = first_fit->pointer_to_memory;

    		}
    	}

    	return ptr;
}

void* BEST_FIT_ALLOCATION(int _size)
{
	void* ptr = NULL;
	struct nodeStruct* free_frame = kallocator.free_memory_list;
	struct nodeStruct* smallest_chunk = find_smallest_chunk(kallocator.free_memory_list);
	struct nodeStruct* best_fit = NULL ;

	if (_size <= smallest_chunk->memory_size)
	{
		best_fit = smallest_chunk;
	}

	else
	{	

		while(free_frame!= NULL)
    	{
    		if (  _size <= free_frame->memory_size )
    		{
    			if (best_fit == NULL)
    			{
    				best_fit = free_frame;
     			}    			

    			if (free_frame->memory_size < best_fit->memory_size)
    			{
    				best_fit = free_frame;
    				
    			}
    		}

    		free_frame = free_frame->next;

    	}
	}

	if (free_frame == NULL)
	{
		printf("free frame is NULL\n");
	}

	if (best_fit == NULL)
	{
		return ptr;
	}

	int remaining_size = best_fit->memory_size- _size;
    if (remaining_size != 0)
    {
    	struct nodeStruct* new_frame = List_createNode (_size ,best_fit->pointer_to_memory, best_fit->start_of_block );
    	List_insertHead(&kallocator.allocated_memory_list, new_frame);
    	best_fit->memory_size += -(_size);
    	best_fit->pointer_to_memory -= -(_size);
    	best_fit->start_of_block -= -(_size);

    	

    	ptr = new_frame->pointer_to_memory;
    			
    }
    else
    {
   		Unlink_Node(&kallocator.free_memory_list , best_fit);
    	List_insertHead(&kallocator.allocated_memory_list, best_fit);
    	ptr = best_fit->pointer_to_memory;

    }
   

   return ptr;

}


void* WORST_FIT_ALLOCATION(int _size)
{
	void* ptr = NULL;
	struct nodeStruct* largest_chunk = find_largest_chunk(kallocator.free_memory_list);
	struct nodeStruct* worst_fit = NULL ;


	if (_size <= largest_chunk->memory_size)
	{
		worst_fit = largest_chunk;
	
	}

	/*else
	{
		while(free_frame!= NULL)
    	{
    		if (free_frame->memory_size <= _size)
    		{
    			if (worst_fit == NULL || worst_fit->memory_size > free_frame->memory_size)
    			{
    				worst_fit = free_frame;
    			}
    		}

    		free_frame = free_frame->next;
    			
    	}
	}*/

	if (worst_fit == NULL)
	{
		return ptr;
	}

	int remaining_size = worst_fit->memory_size- _size;
    if (remaining_size != 0)
    {
    	struct nodeStruct* free_frame = List_createNode (_size ,worst_fit->pointer_to_memory, worst_fit->start_of_block );
    	List_insertHead(&kallocator.allocated_memory_list, free_frame);
    	worst_fit->memory_size += -(_size);
    	worst_fit->pointer_to_memory -= -(_size);
    	worst_fit->start_of_block -= -(_size);

    	
    	ptr = free_frame->pointer_to_memory;
    			
    }
    else
    {
   		Unlink_Node(&kallocator.free_memory_list , worst_fit);
    	List_insertHead(&kallocator.allocated_memory_list, worst_fit);
    	ptr = worst_fit->pointer_to_memory;

    }
   

   return ptr;
}



void* kalloc(int _size) 
{
	assert(_size > 0);
    void* ptr = NULL;

    int size_available = available_memory();
    	
    

    if (_size > size_available)
    {
    	printf("size asked for is greater than available. CANNOT ALLOCATE MEMORY. BYE BYE.\n");
    	return ptr;
    }

 
    if (kallocator.aalgorithm == BEST_FIT)
    {
    	ptr = BEST_FIT_ALLOCATION(_size);


    	return ptr;
    }

    if (kallocator.aalgorithm == FIRST_FIT)
    {

    	ptr = FIRST_FIT_ALLOCATION(_size);
    	

    	return ptr;

    }

    if (kallocator.aalgorithm == WORST_FIT)
    {
    	
    	ptr = WORST_FIT_ALLOCATION(_size);
    	return ptr;
    }
   

    // Allocate memory from kallocator.memory 
    // ptr = address of allocated memory

    return ptr;
}

void kfree(void* _ptr) {
    assert(_ptr != NULL);

    struct nodeStruct* free_frame = kallocator.allocated_memory_list;
    while(free_frame!= NULL)
    {
    	if (free_frame->pointer_to_memory == _ptr)
    	{
    		break;
    	}
    	else
    		free_frame = free_frame->next;
    }
  	assert(free_frame != NULL);

    Unlink_Node(&kallocator.allocated_memory_list, free_frame);

    struct nodeStruct* previous= kallocator.free_memory_list;
  	while(previous != NULL)
   	{
    	if (previous->end_of_block == free_frame->start_of_block)
    	{
   		 	break;
   		}  	

    	previous = previous->next;
    		
  	}

    struct nodeStruct* after_free_frame = kallocator.free_memory_list;
   
   	while(after_free_frame!= NULL)
   	{
    	if (after_free_frame->start_of_block == free_frame->end_of_block)
    	{
    		break;
    	}
    		
    	after_free_frame = after_free_frame->next;
  	  		
  	    
   	}

  
    List_insertTail(&kallocator.free_memory_list, free_frame);
     
    if (previous != NULL)
    {
    	
    	if (free_frame-> start_of_block < previous->start_of_block)
    	{
    	

    		previous->memory_size += free_frame->memory_size;
    		previous->start_of_block -= free_frame->memory_size;
    		previous->pointer_to_memory -= free_frame->memory_size;
    		List_deleteNode(&kallocator.free_memory_list, free_frame);
    		//node_pointer = previous;

    	}
    	else
    	{
    		
    		free_frame->memory_size += previous->memory_size;
    		free_frame->start_of_block -= previous->memory_size;
    		free_frame->pointer_to_memory -= previous->memory_size;
    		List_deleteNode(&kallocator.free_memory_list, previous);
    		//node_pointer = free_frame;
       	}
    }

    if (after_free_frame!= NULL)
    {
    	if (free_frame-> start_of_block < after_free_frame->start_of_block)
    	{
    	

    		after_free_frame->memory_size += free_frame->memory_size;
    		after_free_frame->start_of_block -= free_frame->memory_size;
    		after_free_frame->pointer_to_memory -= free_frame->memory_size;
    		List_deleteNode(&kallocator.free_memory_list, free_frame);
    		//node_pointer = after_free_frame;

    	}
    	else
    	{
    		
    		free_frame->memory_size += after_free_frame->memory_size;
    		free_frame->start_of_block -= after_free_frame->memory_size;
    		free_frame->pointer_to_memory -= after_free_frame->memory_size;
    		List_deleteNode(&kallocator.free_memory_list, after_free_frame);
    		//node_pointer = free_frame;
       	}
    }
   
}


void swap_frames( struct nodeStruct* node_one, struct nodeStruct* node_two)
{
 	if( node_one != NULL && node_one->pointer_to_memory != NULL && node_two != NULL && node_two->pointer_to_memory != NULL)
  	{
  		struct nodeStruct* temp = NULL;

     	// If 1 comes before 2
   		if( node_one->start_of_block < node_two->start_of_block )
   	 	{	
    		temp = node_two;

        	node_two->start_of_block = node_one->start_of_block;
        	node_two->end_of_block = node_one->end_of_block;

       		node_one->start_of_block = temp->start_of_block;
        	node_one->end_of_block = temp->end_of_block;
    	} 
   		else 
    	{
        // Else 2 comes before 1
        	temp = node_one;
        	node_one->start_of_block = node_two->start_of_block;
        	node_one->end_of_block = node_two->end_of_block;

        	node_two->start_of_block = temp->start_of_block;
        	node_two->end_of_block = temp->end_of_block;

    	}
  
    	node_one->pointer_to_memory = kallocator.memory + node_one->start_of_block;
   		node_two->pointer_to_memory = kallocator.memory + node_two->start_of_block;

   		temp = NULL;

  	}

  	else
  	{
  		printf("Pointer to nodes or memory were NULLS or invalid. Cannot swap frames.\n");
  	}  

    
}


int compact_allocation(void** _before, void** _after) 
{
    int compacted_size = 0;
    List_sort(&kallocator.allocated_memory_list);

   	struct nodeStruct* node_ptr = NULL; //kallocator.allocated_memory_list;
    int position = 0;

    if (kallocator.free_memory_list == NULL)
    {
    	return compacted_size;
    }

    while((node_ptr = List_findNodeNextNode(kallocator.allocated_memory_list, position)) != NULL)
    {	
    	/*struct nodeStruct* head = kallocator.allocated_memory_list;
    	while( head != NULL )
		{	

			if( head->start_of_block > position )
			{
				if( (node_ptr == NULL) || (head->start_of_block < node_ptr->start_of_block) )
				{
					node_ptr= head;
				}
			}

			head = head->next;
		}*/
	

		position = node_ptr->start_of_block;
    	_before[compacted_size] = node_ptr->pointer_to_memory; //initially compacted size is 0

    	
   		 struct nodeStruct* previous= kallocator.free_memory_list;
  		 while(previous != NULL)
   		 {
    		if (previous->end_of_block == node_ptr->start_of_block)
    		{
   		 		break;
   		 	}  	

    		previous = previous->next;
    		
  		 }

    	struct nodeStruct* after_node_ptr = kallocator.free_memory_list;
   
   		while(after_node_ptr!= NULL)
   		{
    		if (after_node_ptr->start_of_block == node_ptr->end_of_block)
    		{
    			break;
    		}
    		
    		after_node_ptr = after_node_ptr->next;
  	  		
  	    
   		}


    	if (previous != NULL)
    	{	
    		int sts = node_ptr->start_of_block;    		
    		swap_frames(previous, node_ptr);
    		memmove(node_ptr->pointer_to_memory, kallocator.memory+sts, node_ptr->memory_size);
    	
    
       		if( after_node_ptr != NULL )
   			{
            
    			if (previous-> start_of_block > after_node_ptr->start_of_block)
    			{
    	

    				previous->memory_size += after_node_ptr->memory_size;
    				previous->start_of_block -= after_node_ptr->memory_size;
    				previous->pointer_to_memory -= after_node_ptr->memory_size;
    				List_deleteNode(&kallocator.free_memory_list, after_node_ptr);
    				//after_node_ptr = previous;
    				//node_pointer = head;

    			}
    			else
    			{
    		
    				after_node_ptr->memory_size += previous->memory_size;
    				after_node_ptr->start_of_block -= previous->memory_size;
    				after_node_ptr->pointer_to_memory -= previous->memory_size;
    				List_deleteNode(&kallocator.free_memory_list, previous);
    				//node_ptr = after_node_ptr;
    				//node_pointer = free_frame;
       			}


   			}
   		}

   	
   	 
      	_after[compacted_size] = node_ptr->pointer_to_memory;

    	compacted_size++;
    	


    }
    
    return compacted_size;
}



int available_memory() {

    int available_memory_size = 0;
	struct nodeStruct* ptr = kallocator.free_memory_list; 
	
	while(ptr!=NULL)
	{
		available_memory_size = available_memory_size + (ptr->memory_size);
		ptr = ptr->next;
	}

    return available_memory_size;
}

int allocated_memory() {

    int allocated_memory_size = 0;
	struct nodeStruct* ptr = kallocator.allocated_memory_list; 
	
	while(ptr!=NULL)
	{
		allocated_memory_size += (ptr->memory_size);
		ptr = ptr->next;
	}

    return allocated_memory_size;
}



void print_statistics() {

    int allocated_size = allocated_memory();
    int allocated_chunks = List_countNodes(kallocator.allocated_memory_list);
    int free_size = available_memory();
    int free_chunks = List_countNodes(kallocator.free_memory_list);

    struct nodeStruct *ptr = find_smallest_chunk(kallocator.free_memory_list);
    int smallest_free_chunk_size = 0;
    if (ptr != NULL)
    {
    	smallest_free_chunk_size = ptr->memory_size ;
    }

    ptr = find_largest_chunk(kallocator.free_memory_list);
    int largest_free_chunk_size = 0;
    if (ptr!=NULL)
    {
    	largest_free_chunk_size = ptr->memory_size;
    }
    // Calculate the statistics

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);

    struct nodeStruct* free_chunk = kallocator.free_memory_list;
    printf("List %d free chunks:\n", free_chunks);
    while( free_chunk != NULL )
    {
        printf("\tbegin: %d\t|\tsize: %d\n", free_chunk->start_of_block, free_chunk->memory_size);
        free_chunk = free_chunk->next;
    }
   // printf("compact_allocation = %d\n", smallest_free_chunk_size);c
}



