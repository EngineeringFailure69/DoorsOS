#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "../consts_types.h"

typedef enum { false, true } bool;

typedef struct memory_node
{
	uint32_t size;
	bool used;
	struct memory_node* next;
	struct memory_node* prev; 
}memory_node_t;

#define NULL_POINTER ((void*)0)
#define MEMORY_BLOCK 4096
#define MEMORY_CHUNK sizeof(memory_node_t)

void initialize_memory();
void* find_block(memory_node_t* memory, size_t size);
void* memory_allocation(size_t size);
void *merge_next_node_into_current(memory_node_t *current_mem_node);
void *merge_current_node_into_previous(memory_node_t *current_mem_node);
void memory_free(void *p);

#endif
