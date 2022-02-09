#ifndef OBJECT_H
#define OBJECT_H

#include "stack.h"

typedef struct {

	size_t source_size;
	unsigned char* source;
	unsigned char* current_instruction;

	size_t memory_size;
	unsigned char* memory;
	unsigned char* current_cell;

	size_t stack_size;
	Stack* stack;

} Object;

Object* create_object ( size_t source_size, size_t memory_size, size_t stack_size );
void destroy_object ( Object* object );

#endif