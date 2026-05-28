#ifndef MEMORY_H
#define MEMORY_H

#include "../../drivers/drivers.h"
#include "../consts_types.h"

void* memcopy(void* destination, const void* source, size_t bytes_size);
void* memset(void* destination, int value, size_t bytes_size);

#endif
