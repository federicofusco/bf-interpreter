#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "object.h"
#include "stack.h"

/**
 * Creates an object with the given source and memory size
 *
 * @param source_size The size of the source
 * @param memory_size The size of the memory
 * @returns A pointer to the newly created object
 */
Object* create_object ( size_t source_size, size_t memory_size, size_t stack_size ) {

	logf_verbose ( "Created object with source (%d), memory (%d) and stack (%d)", source_size, memory_size, stack_size );

	// Creates a new object
	Object* object = malloc ( sizeof ( Object ) );
	if ( object == NULL ) {

		// Failed to allocate memory to the object
		log_fatal ( "Failed to allocate memory to the object!" );
	}

	// Allocates memory to the source
	object -> source_size = source_size;
	object -> source = calloc ( sizeof ( char ), object -> source_size + 1 );
	if ( object -> source == NULL ) {

		// Failed to allocate memory to the object source
		log_fatal ( "Failed to allocate memory to the object source!" );
	}
	object -> current_instruction = object -> source;

	// Adds null terminator to source 
	*( object -> source + object -> source_size ) = '\0';

	// Allocates memory to the memory (hehe)
	object -> memory_size = memory_size;
	object -> memory = calloc ( sizeof ( char ), object -> memory_size );
	if ( object -> memory == NULL ) {

		// Failed to allocate memory to the object memory
		log_fatal ( "Failed to allocate memory to the object memory!" );
	}
	object -> current_cell = object -> memory;

	// Allocates memory to the object stack
	object -> stack_size = stack_size;
	object -> stack = create_stack ( object -> stack_size );

	return object;
}

/** 
 * Destroys a given object by deallocating its memory
 *
 * @param object The object which should be deallocated
 */
void destroy_object ( Object* object ) {

	log_verbose ( "Destroying object" );

	// Deallocates the object stack
	destroy_stack ( object -> stack );

	// Deallocates the object memory
	free ( object -> memory );

	// Deallocates the object source
	free ( object -> source );

	// Deallocates the object
	free ( object );
}