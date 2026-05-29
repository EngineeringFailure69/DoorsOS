#include "memory.h"
#include "dynamic.h"

void* memcopy(void* destination, const void* source, size_t bytes_size)
{
	int offset;
	char* dest = (char*) destination;
	const char* src = (const char*) source;
	while(bytes_size--)
		*dest++=*src++;
	offset = get_cursor();
	offset = handle_new_line(offset);
	set_cursor(offset);
	return destination;
}

void* memset(void* destination, int value, size_t bytes_size)
{
	int offset;
	unsigned char* dest = (unsigned char*) destination;
	while(bytes_size--)
		*dest++ = (unsigned char) value;
	offset = get_cursor();
	offset = handle_new_line(offset);
	set_cursor(offset);
	return destination;
}

static uint8_t memory_area[MEMORY_BLOCK];
static memory_node_t *memory_start;

void initialize_memory()
{
	memory_start = (memory_node_t *) memory_area;
	memory_start->size = MEMORY_BLOCK - MEMORY_CHUNK; 
	memory_start->next = NULL;
	memory_start->prev = NULL;
}

void* find_block(memory_node_t* memory, size_t size)
{
	memory_node_t* best_block = (memory_node_t *) NULL;
	uint32_t best_block_size = MEMORY_BLOCK + 1;
	memory_node_t* current_block = memory;	
	
	while(current_block)
	{
		if((!current_block->used) && (current_block->size >= (size + MEMORY_CHUNK)) && (current_block->size <= best_block_size))
		{
			best_block = current_block;
			best_block_size = current_block->size;
		}	
		current_block = current_block->next;
	}
	
	return best_block;
}

void* memory_allocation(size_t size)
{
	memory_node_t *best_block = (memory_node_t*) find_block(memory_start, size);
	if(best_block != NULL)
	{
		best_block->size = best_block->size - size - MEMORY_CHUNK;
		memory_node_t* memory_chunk_alloc = (memory_node_t*) (((uint8_t)best_block ) + MEMORY_CHUNK + best_block->size);
		
		memory_chunk_alloc->size = size;
		memory_chunk_alloc->used = true;
		memory_chunk_alloc->prev = best_block;
		memory_chunk_alloc->next = best_block->next;
		
	if (best_block->next != NULL) {
            best_block->next->prev = memory_chunk_alloc;
        }
        best_block->next = memory_chunk_alloc;

        return (void *) ((uint8_t *) memory_chunk_alloc + MEMORY_CHUNK);		
	} 
	return NULL;
}

void *merge_next_node_into_current(memory_node_t *current_mem_node)
{
	memory_node_t *next_mem_node = current_mem_node->next;
	if (next_mem_node != NULL && !next_mem_node->used) 
	{
		// add size of next block to current block
		current_mem_node->size += current_mem_node->next->size;
		current_mem_node->size += MEMORY_CHUNK;

		// remove next block from list
		current_mem_node->next = current_mem_node->next->next;
		if (current_mem_node->next != NULL) 
		{
			current_mem_node->next->prev = current_mem_node;
		}
	}
	return current_mem_node;
}

void *merge_current_node_into_previous(memory_node_t *current_mem_node)
{
	memory_node_t *prev_mem_node = current_mem_node->prev;
	if (prev_mem_node != NULL && !prev_mem_node->used) 
	{
		// add size of previous block to current block
		prev_mem_node->size += current_mem_node->size;
		prev_mem_node->size += MEMORY_CHUNK;

		// remove current node from list
		prev_mem_node->next = current_mem_node->next;
		if (current_mem_node->next != NULL) 
		{
		    current_mem_node->next->prev = prev_mem_node;
		}
	}
	return current_mem_node;
}

void memory_free(void *p)
{
	// move along, nothing to free here
	if (p == NULL) 
	{
		return;
	}

	// get mem node associated with pointer
	memory_node_t *current_mem_node = (memory_node_t *) ((uint8_t *) p - MEMORY_CHUNK);

	// pointer we're trying to free was not dynamically allocated it seems
	if (current_mem_node == NULL) 
	{
		return;
	}

	// mark block as unused
	current_mem_node->used = false;

	// merge unused blocks
	current_mem_node = merge_next_node_into_current(current_mem_node);
	merge_current_node_into_previous(current_mem_node);
}
