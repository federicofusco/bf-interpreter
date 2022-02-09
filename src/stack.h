#ifndef STACK_H
#define STACK_H

typedef struct {

	void** stack;
	size_t stack_size;
	size_t top;

} Stack;

Stack* create_stack ( size_t stack_size );
int is_stack_full ( Stack* stack );
int is_stack_empty ( Stack* stack );
void push_stack ( Stack* stack, void* element );
void* peek_stack ( Stack* stack );
void* pop_stack ( Stack* stack );
void destroy_stack ( Stack* stack );

#endif