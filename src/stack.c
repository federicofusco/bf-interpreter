#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "stack.h"

/**
 * Allocates memory to a stack of a given size
 *
 * @param stack_size The size of the stack
 * @returns The newly created stack
 */
Stack* create_stack ( size_t stack_size ) {

	logf_verbose ( "Creating stack of size %d", stack_size );

	// Allocates memory to the stack struct
	Stack* stack = malloc ( sizeof ( Stack ) );
	if ( stack == NULL ) {

		// Failed to allocate struct, exits
		log_fatal ( "Failed to allocate memory to stack struct!" );
	}

	stack -> stack_size = stack_size;
	stack -> top = 0;

	// Allocates memory to the stack
	stack -> stack = malloc ( sizeof ( void* ) * stack -> stack_size );
	if ( stack -> stack == NULL ) {

		// Failed to allocate memory to stack, exits
		log_fatal ( "Failed to allocate memory to stack!" );
	}

	return stack;

}

/**
 * Returns whether or not the given stack is full
 *
 * @param stack The stack which should be checked
 */
int is_stack_full ( Stack* stack ) {
	return stack -> top == stack -> stack_size;
}

/**
 * Returns whether or not the given stack is empty
 *
 * @param stack The stack which should be checked
 */
int is_stack_empty ( Stack* stack ) {
	return stack -> top == 0;
}

/**
 * Pushes a given element to the top of a given stack
 * (If the stack isn't already full)
 *
 * @param stack The stack in which the element should be added
 * @param element The element which should be pushed to the top of the stack
 */
void push_stack ( Stack* stack, void* element ) {

	// Checks if the stack is full
	if ( is_stack_full ( stack ) ) {

		// Prints error and exits
		log_fatal ( "Attempted to push element to stack which is full!" );
	}

	stack -> stack[stack -> top++] = element;

}

/** 
 * Returns the last element of a given stack
 *
 * @param stack The stack which should be peeked
 * @returns The element at the top of the stack
 */
void* peek_stack ( Stack* stack ) {

	if ( is_stack_empty ( stack ) ) {

		// Prints error and exits
		log_fatal ( "Attempted to peek from empty stack!" );
	}

	return stack -> stack[stack -> top - 1];
}

/**
 * Pops the top element from a given stack
 *
 * @param stack The stack from which the element should be popped
 * @returns The element which was popped from the stack
 */
void* pop_stack ( Stack* stack ) {

	if ( is_stack_empty ( stack ) ) {

		// Prints error and exits
		log_fatal ( "Attempted to pop element from empty stack!" );
 	}

	 return stack -> stack[stack -> top--];
}

/** 
 * Deallocates memory from a given stack
 *
 * @param stack The stack which should be destroyed
 */
void destroy_stack ( Stack* stack ) {

	log_verbose ( "Destroying stack" );

	// Deallocates memory from the stack
	free ( stack -> stack );

	// Deallocates the stack struct memory
	free ( stack );
}