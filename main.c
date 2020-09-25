#include <stdio.h>
#include <stdlib.h>
#include "kallocator.h"
/*
#include <stdbool.h>
#include <time.h>
#include <string.h>


#define KTR_SIZE (300)
#define SMALL_ARR_SIZE (20)
#define LARGE_ARR_SIZE (50)

void contiguous_kallocs( int **_arr, int _begin, int _end, int _size )
{
    printf("\n~ KALLOC %d CONTIGUOUS INDICES arr[%d] ~\n", _end - _begin, _size);
    for( int i = _begin; i < _end; ++i )
    {
        _arr[i] = kalloc(sizeof(*_arr[i]));
        if( _arr[i] == NULL )
        {
            printf("kalloc() of arr[%d] failed\n", i);
            continue;
        }
        *(_arr[i]) = rand();
        printf("arr[%d] = %p\t|\t*arr[%d] = %d\n", i, _arr[i], i, *_arr[i]);
    }
    printf("\n");
}

void random_kallocs( int **_arr, int _num, int _size )
{
    printf("\n~ KALLOC %d RANDOM INDICES OF arr[%d] ~\n", _num, _size);
    for( int i = 0; i < _num; ++i )
    {
        int ix = rand() % _size;
        // Do not kalloc a kalloc'd index
        while( _arr[ix] != NULL )
            ix = rand() % _size;
        
        _arr[ix] = kalloc(sizeof(*_arr[ix]));
        if( _arr[ix] == NULL )
        {
            printf("kalloc() of arr[%d] failed\n", ix);
            continue;
        }
        *(_arr[ix]) = rand();
        printf("arr[%d] = %p\t|\t*arr[%d] = %d\n", ix, _arr[ix], ix, *_arr[ix]);
    }
    printf("\n");
}

void kallocs_in_slots( int **_arr, int* _slots, int _num )
{
    printf("\n~ KALLOC %d BLOCKS IN RECENTLY FREED INDICES ~\n", _num);
    for( int i = 0; i < _num; ++i )
    {
        int ix = _slots[i];
        _arr[ix] = kalloc(sizeof(**_arr));
        if( _arr[ix] == NULL )
        {
            printf("kalloc() of arr[%d] failed\n", ix);
            continue;
        }
        *_arr[ix] = rand();
         printf("arr[%d] = %p\t|\t*arr[%d] = %d\n", ix, _arr[ix], ix, *_arr[ix]);
    }//end while
    printf("\n");
}

int* random_kfrees( int** _arr, int** _frees, int _num, int _size )
{
    if( *_frees == NULL )
        *_frees = malloc(sizeof(**_frees) * _num);
    else
        *_frees = realloc(*_frees, sizeof(**_frees) * _num);             // hold freed indices
    printf("\n~ KFREE %d RANDOM INDICES OF arr[%d] ~\n", _num, _size);
    for( int i = 0; i < _num; ++i )
    {
        int ix = rand() % _size;
        // Ensure same index isn't freed twice
        while( _arr[ix] == NULL )
            ix = rand() % _size;

        printf("Freeing arr[%d]\n", ix);
        kfree( _arr[ix] );
        _arr[ix] = NULL;
        (*_frees)[i] = ix;
    }//end for
    printf("\n");
    return *_frees;
}

void contiguous_kfrees( int** _arr, int _begin, int _end, int _size )
{
    printf("\n~ KFREE INDICES FROM arr[%d] TO arr[%d] ~\n", _begin, _end );
    for( int i = _begin; i < _end; ++i )
    {
        if( _arr[i] == NULL )
            continue;
        printf("Freeing arr[%d]\n", i);
        kfree(_arr[i]);
        _arr[i] = NULL;
    }
    printf("\n");
}

void random_int_test( enum allocation_algorithm _aalgorithm, int _kallocator_size, int _small_arr_size, int _large_arr_size )
{
    char aalg_str[10];
    if( _aalgorithm == FIRST_FIT )
        strcpy(aalg_str, "FIRST FIT");
    else if( _aalgorithm == BEST_FIT )
        strcpy(aalg_str, "BEST FIT");
    else
        strcpy(aalg_str, "WORST FIT");
    
    printf("\n===================== BEGIN TESTING %s ALGORITHM =====================\n" 
            "Using %s algorithm on memory size %d\n", aalg_str, aalg_str, _kallocator_size);

    print_statistics();
    

    int* small_arr[_small_arr_size];
    for( int i = 0; i < _small_arr_size; ++i )
        small_arr[i] = NULL;

    contiguous_kallocs(small_arr, 0, _small_arr_size, _small_arr_size);
    print_statistics();

    int num_kfrees = rand() % _small_arr_size;      // random # of frees to perform
    while( num_kfrees == 0 )
        num_kfrees = rand() % _small_arr_size;
    int *frees = NULL;

    random_kfrees(small_arr, &frees, num_kfrees, _small_arr_size);
    print_statistics();

    kallocs_in_slots(small_arr, frees, num_kfrees);
    print_statistics();

    int* large_arr[_large_arr_size];
    for( int i = 0; i < _large_arr_size; ++i )
        large_arr[i] = NULL;

    int num_kallocs = rand() % _large_arr_size;      // random # of frees to perform
    while( num_kallocs < (_large_arr_size/3) )
        num_kallocs = rand() % _large_arr_size;
    
    contiguous_kallocs(large_arr, 0, num_kallocs, _large_arr_size);
    print_statistics();

    num_kfrees = rand() % _large_arr_size;
    while( num_kfrees > num_kallocs || num_kfrees < (num_kallocs/3) )
        num_kfrees = rand() % _large_arr_size;

    random_kfrees(large_arr, &frees, num_kfrees, _large_arr_size);
    print_statistics();

    kallocs_in_slots(large_arr, frees, num_kfrees);
    print_statistics();

    num_kallocs = rand() % _large_arr_size;      // random # of frees to perform
    while( num_kallocs < (_large_arr_size/3) )
        num_kallocs = rand() % _large_arr_size;
    
    contiguous_kallocs(large_arr, num_kallocs, _large_arr_size, _large_arr_size);
    print_statistics();

    void* before[100];
    void* after[100];
    compact_allocation(before, after);
    print_statistics();

    free(frees);
     printf("\n============== END TESTING OF %s ==============\n", aalg_str);
}

void first_fit_driver()
{
    initialize_allocator(KTR_SIZE, FIRST_FIT);

    random_int_test(BEST_FIT, KTR_SIZE, SMALL_ARR_SIZE, LARGE_ARR_SIZE);

    destroy_allocator();
}

void best_fit_driver()
{
    initialize_allocator(KTR_SIZE, BEST_FIT);

    random_int_test(BEST_FIT, KTR_SIZE, SMALL_ARR_SIZE, LARGE_ARR_SIZE);

    destroy_allocator();
}
void worst_fit_driver()
{
    initialize_allocator(KTR_SIZE, WORST_FIT);

    random_int_test(WORST_FIT, KTR_SIZE, SMALL_ARR_SIZE, LARGE_ARR_SIZE);

    destroy_allocator();
}

int main(int argc, char* argv[]) 
{
    //initialize_allocator(100, FIRST_FIT);
    //initialize_allocator(100, BEST_FIT);
    initialize_allocator(100, WORST_FIT);

    //printf("Using first fit algorithm on memory size 100\n");
    //printf("Using best fit algorithm on memory size 100\n");
    printf("Using worst fit algorithm on memory size 100\n");



    int* p[50] = {NULL};
    for(int i=0; i<10; ++i) {
        p[i] = kalloc(sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

    print_statistics();

    for(int i=0; i<10; ++i) {
        if(i%2 == 0)
            continue;

        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }
    
    print_statistics();

    for( int i = 0; i < 20; ++i )
        if( p[i] == NULL )
            continue;
        else
            printf("p[%d] = %p\n\t*p[%d] = %d\n", i, p[i], i, *(p[i]));

    printf("available_memory %d\n\n", available_memory());

    void* before[100] = {NULL};
    void* after[100] = {NULL};
    int count = compact_allocation(before, after);

    for( int i = 0; i < count; ++i )
        for( int j = 0; j < 20; ++j )
            if( p[j] == before[i] )
            {
                p[j] = after[i];
                break;
            }


    print_statistics();

    for( int i = 0; i < 20; ++i )
        if( p[i] != NULL )
            printf("p[%d] = %p\n\t*p[%d] = %d\n", i, p[i], i, *(p[i]));
        else
            continue;
    
    // You can assume that the destroy_allocator will always be the 
    // last funciton call of main function to avoid memory leak 
    // before exit

    destroy_allocator();
    
    srand(time(NULL));
    first_fit_driver();
    best_fit_driver();
    worst_fit_driver();
    return EXIT_SUCCESS;
}
*/

//SAKIB MAIN
int main(int argc, char* argv[]) {

	if (argc != 2)
	{
		return 0;
	}

	int value = atoi(argv[1]);

	if (value == 1)
	{
		initialize_allocator(1000, FIRST_FIT);
		printf("Using first fit algorithm on memory size 1000\n");
	}
	if (value == 2)
	{
		initialize_allocator(1000, BEST_FIT);
		printf("Using best fit algorithm on memory size 1000\n");
	}
	if (value == 3)
	{
		initialize_allocator(1000, WORST_FIT);
		printf("Using worst fit algorithm on memory size 1000\n");
	}

	int* p[50] = {NULL};
    for(int i=0; i<10; ++i) {
        p[i] = kalloc((i+1)*sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

   //print_statistics();
    
    for(int i=0; i<10; ++i) {
        if(i%2 == 0)
            continue;

        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }
    print_statistics();
	
	available_memory();

    for(int i=0; i<10; ++i) {
        p[i] = kalloc((i+1)*sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

    for(int i=0; i<10; ++i) {
        if(i%2 == 0)
            continue;

        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }

    printf("\navailable_memory %d\n", available_memory());

    void* before[100] = {NULL};
    void* after[100] = {NULL};
    //compact_allocation(before, after);
    int x = compact_allocation(before, after);
    printf("COMPACT ALLOCATION OCCURS HERE = %d\n\n", x);

    print_statistics();

    // You can assume that the destroy_allocator will always be the 
    // last funciton call of main function to avoid memory leak 
    // before exit
	for(int i=0; i<10; ++i) {
        p[i] = kalloc((i+1)*sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

    
    for(int i=0; i<10; ++i) {
        if(i%2 == 0)
            continue;

        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }

    //compact_allocation(before, after);
   
    print_statistics();



    destroy_allocator();

    return 0;
   
}

//ACTUAL mAIN
/*int main(int argc, char* argv[]) {

	if (argc != 2)
	{
		return 0;
	}

	int value = atoi(argv[1]);

	if (value == 1)
	{
		initialize_allocator(100, FIRST_FIT);
		printf("Using first fit algorithm on memory size 100\n");
	}
	if (value == 2)
	{
		initialize_allocator(100, BEST_FIT);
		printf("Using best fit algorithm on memory size 100\n");
	}
	if (value == 3)
	{
		initialize_allocator(100, WORST_FIT);
		printf("Using worst fit algorithm on memory size 100\n");
	}
  
   int* p[50] = {NULL};
    for(int i=0; i<10; ++i) {
        p[i] = kalloc(sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }

    printf("available_memory = %d\n\n", available_memory());

    print_statistics();

    for(int i=0; i<10; ++i) {
        if(i%2 == 0)
           continue;

        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
    }
   // print_statistics();

    printf("available_memory = %d\n\n", available_memory());
    void* before[100] = {NULL};
    void* after[100] = {NULL};
   	int x = compact_allocation(before, after);
   	printf("COMPACT ALLOCATION = %d\n\n", x);


    print_statistics();

    // You can assume that the destroy_allocator will always be the 
    // last funciton call of main function to avoid memory leak 
    // before exit

    destroy_allocator();

    return 0;
    
}*/

