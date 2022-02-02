#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

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
    stack -> stack = malloc ( sizeof ( char* ) * stack -> size );

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
 */
void push_stack ( Stack* stack, char* element ) {

    if ( is_stack_full ( stack ) ) {

        // Exits
        printf ( "Error: Attempted to push element to stack which is full!\n" );
        exit ( EXIT_FAILURE );
    }

    stack -> stack[stack -> top++] = element;
    
} 

/**
 * Removes the last element from the stack
 * 
 * @param stack The stack from which the last element should be removed
 * @returns The last element of the stack
 */
char* pop_stack ( Stack* stack ) {

    if ( is_stack_empty ( stack ) ) {

        // Exits
        printf ( "Error: Attempted to pop element from empty stack!\n" );
        exit ( EXIT_FAILURE );
    }

    return stack -> stack[stack -> top--];

}