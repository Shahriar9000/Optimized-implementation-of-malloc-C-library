#ifndef __KALLOCATOR_H__
#define __KALLOCATOR_H__
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"



enum allocation_algorithm {FIRST_FIT, BEST_FIT, WORST_FIT};

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm);

void* kalloc(int _size);
void kfree(void* _ptr);
void* FIRST_FIT_ALLOCATION(int _size);
void* BEST_FIT_ALLOCATION(int _size);
void* WORST_FIT_ALLOCATION(int _size);
int allocated_memory();
int available_memory();
void print_statistics();
void swap_frames( struct nodeStruct* node_one, struct nodeStruct* node_two);
int compact_allocation(void** _before, void** _after);
void destroy_allocator();

#endif
