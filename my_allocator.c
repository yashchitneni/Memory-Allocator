/*
 File: my_allocator.c
 
 Author: <Yash Chitneni>
 Department of Computer Science
 Texas A&M University
 Date  : <2/7/14>
 
 Modified:
 
 This file contains the implementation of the module "MY_ALLOCATOR".
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */
Addr memory;
Addr* free_list;
unsigned int fib[1000];

struct header{
    Addr next;
    char symbol;
    int size;
    int LoR;
//    int inheritance;
//    int right_buddy;
//    int left_buddy;
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

Addr increment_addr(Addr address, int sum) {
    return (char*)(address) + sum;
}

void create_Header(Addr current, int size, char symbol, Addr next) {
    header* temp = (header*)current;
    temp->size = size;
	temp->symbol = symbol;
	temp->next = next;
}

void split_memory(unsigned int index) {
    Addr split_left = free_list[index];
    header* temp = (header*)split_left;
    free_list[index] = temp->next;
    unsigned int size = 0;
    Addr split_right = increment_addr(split_left, size);
    create_Header(split_right, size, 'H', free_list[index-1]);
	create_Header(split_left, size, 'H', split_right);
	free_list[index-1] = split_left;
}

unsigned int fib_at_index(unsigned int index) {
    
    fib[0] = 1;
    fib[1] = 2;
    int i;
    
    for (i = 2; i <= index; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    return fib[i-1];
}

unsigned int largest_fib_index(unsigned int blocks) {
    
    int fib[2] = {1, 2};
    int pos = 0;
    int index = 0;
    
    while (blocks > fib[pos]) {
        fib[pos] += fib[(pos+1) % 2];
        pos++;
        pos %= 2;
        if (blocks >= fib[pos]) {
            index++;
        }
    }
    return index;
}

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length) {

    if(_length % _basic_block_size != 0) {
        _length += _length % _basic_block_size;
    }

    memory = malloc(_length);
    
    unsigned int free_list_size = largest_fib_index(_length/_basic_block_size);
    free_list = new Addr[free_list_size];
        
    for (int i = 0; i < free_list_size; i++) {
        free_list[i] = NULL;
    }
    
    unsigned int sum = 0;
    
    while (_length > 0) {
        unsigned int index = largest_fib_index(_length/_basic_block_size);
        unsigned int block_amount = _basic_block_size * fib_at_index(index);
        
        Addr temp_addr = increment_addr(memory, sum);
        free_list[index] = temp_addr;
        
        create_Header(free_list[index], block_amount, 'H', NULL);
        
        _length -= block_amount;
        sum += block_amount;
    }
    return _length;
}

extern Addr my_malloc(unsigned int _length) {
    /* This preliminary implementation simply hands the call over the
     the C standard library!
     Of course this needs to be replaced by your implementation.
     */
    
    unsigned int total_length = _length + sizeof(header);
    
    return malloc((size_t)_length);
}

extern int my_free(Addr _a) {
    /* Same here! */
    free(_a);
    return 0;
}

