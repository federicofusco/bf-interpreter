#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "stack.h"
#include "log.h"

/**
 * Creates a stack with a given size
 *
 * @param size The size of the stack
 * @returns An empty stack of the given size
 */
Stack* create_stack ( size_t size ) {

    Stack* stack = malloc ( sizeof ( Stack* ) );
    stack -> size = size;
    stack -> top = -1;
    stack -> stack = malloc ( sizeof ( void* ) * stack -> size );

	logf_verbose ( "Initialized stack of size %d", size );
    return stack;

}

/**
 * Return whether or not the given stack is empty
 *
 * @param stack The stack which should be checked
 * @returns 1 if the stack is empty, 0 if it's not
 */
int is_stack_empty ( Stack* stack ) {
    return stack -> top == -1;
}

/**
 * Return whether or not the given stack is full
 *
 * @param stack The stack which should be checked
 * @returns 1 if the stack if full, 0 if it's not 
 */
int is_stack_full ( Stack* stack ) {
	return stack -> top == stack -> size - 1;
}

/**
 * Adds an element to the stack
 *
 * @param stack The stack with should recieve the element
 * @param element The element which should be added to the stack
 * @returns 1 on success, 0 in case of errors
 */
int push_stack ( Stack* stack, void* element ) {

    if ( is_stack_full ( stack ) ) {
        log_warn ( "Attempted to push element to stack which is full!" );
		return 0;
    }

    stack -> stack[stack -> top++] = element;
	return 1;
    
} 

/**
 * Removes the last element from the stack
 * 
 * @param stack The stack from which the last element should be removed
 * @returns The last element of the stack
 */
void* pop_stack ( Stack* stack ) {

    if ( is_stack_empty ( stack ) ) {
        log_warn ( "Attempted to pop element from empty stack!" );
		return NULL;
    }

    return stack -> stack[stack -> top--];

}