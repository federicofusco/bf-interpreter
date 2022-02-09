#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "log.h"
#include "compiler.h"
#include "object.h"
#include "stack.h"

#include <time.h>

/**
 * Validates a given object source
 *
 * @param object The object which should be validated
 */
int validate ( Object* object ) {

	log_verbose ( "Validating source" );

	// Creates a stack to store the bracket positions
	Stack* bracket_stack = create_stack ( object -> source_size );

	// Loops through the source instructions
	unsigned int ln = 1;
	unsigned int cl = 0;
	while ( *( object -> current_instruction ) != '\0' ) {

		switch ( *( object -> current_instruction ) ) {

			case '\n': {
				ln++;
				cl = 0;

				break;
			}

			case '[': {

				// Pushes the bracket position to the stack
				// unsigned long long int pos = ( (unsigned long long int) ln << 32ULL ) | 
				unsigned long long int pos = ( ( unsigned long long int ) ln << 32ULL ) | cl;
				push_stack ( bracket_stack, (void*) pos );
				break;
			}

			case ']': {

				// Pops a bracket from the stack
				// If the stack is empty, there is an extra ]
				if ( is_stack_empty ( bracket_stack ) ) {
					logf_fatal ( "Unexpected token \"]\" at %d:%d!", ln, cl );
				}
				pop_stack ( bracket_stack );
				
				break;
			}

			default: {

				// Avoids other instructions
				break;
			}

		}

		object -> current_instruction++;
		cl++;
	}

	// Resets the current instruction
	object -> current_instruction = object -> source;

	if ( !is_stack_empty ( bracket_stack ) ) {

		unsigned int line = ( unsigned int ) ( ( unsigned long long int ) peek_stack ( bracket_stack ) >> 32ULL );
		unsigned int column = ( unsigned int ) ( ( unsigned long long int ) peek_stack ( bracket_stack ) << 32ULL >> 32ULL ); 

		logf_fatal ( "Unexpected token \"[\" at %d:%d!", line, column );
	}



	destroy_stack ( bracket_stack );

}

/**
 * Optimizes the object source to reduce total operations
 *
 * @param object The object which should be optimized
 */
int optimize ( Object* object ) {

	log_verbose ( "Optimizing object source" );

	// Copies the original object's source
	char* source = calloc ( sizeof ( unsigned char ), object -> source_size + 1 );
	if ( object -> source == NULL ) {

		// Failed to allocate memory to the object source
		log_fatal ( "Failed to allocate memory to the object source!" );
	}
	strcpy ( source, object -> source );

	// Reallocates the objects source
	object -> source_size = object -> source_size * 2;
	free ( object -> source );
	object -> source = calloc ( sizeof ( unsigned char ), object -> source_size );

	// Copies the unoptimized source
	*( object -> source ) = *( source );
	*( object -> source + 1 ) = 0x01;

	unsigned char* optimized_position = object -> source + 2;
	int x = 1;
	while ( *( source + x ) != '\0' && x < object -> source_size ) {

		unsigned char* unoptimized_position = source + x;
		if ( *( unoptimized_position ) != '+' && *( unoptimized_position ) != '-' && *( unoptimized_position ) != '>' && *( unoptimized_position ) != '<' && *( unoptimized_position ) != '.' && *( unoptimized_position ) != ',' && *( unoptimized_position ) != '[' && *( unoptimized_position ) != ']' ) {
			x++;
			continue;
		}

		// Checks if the current unoptimized instruction is the same as the previous
		// If so, it increments the counter of the previous instruction
		if ( *( unoptimized_position ) == '[' || *( unoptimized_position ) == ']' || *( optimized_position - 1 ) == 255 ){

			// Adds the new instruction with its counter set to 1
			*( optimized_position ) = *( unoptimized_position );
			*( optimized_position + 1 ) = 0x01;
			optimized_position += 2;
			x++;
			continue;
		}

		if ( *( unoptimized_position ) == *( optimized_position - 2 ) ) {

			// Updates the counter of the previous instruction
			*( optimized_position - 1 ) += 1;
			x++;
			continue; 

		} else {

			// Adds the new instruction with its counter set to 1
			*( optimized_position ) = *( unoptimized_position );
			*( optimized_position + 1 ) = 0x01;
			optimized_position += 2;
			x++;
			continue;

		}

	}

	// Resizes the source
	for ( int x = 0; x < object -> source_size; x++ ) {
		if ( *( object -> source + x ) != '\0' ) continue;

		// Resizes the source
		object -> source_size = x + 2;
		object -> source = realloc ( object -> source, object -> source_size );
		break;
	}

	object -> current_instruction = object -> source;

	free ( source );

}

/**
 * Interprets a given program object
 *
 * @param object The program object which should be interpreted
 */
int interpret ( Object* object ) {

	log_verbose ( "Started execution" );

	clock_t start = clock ();

	// Loops through each instruction in the object source
	while ( *( object -> current_instruction ) != '\0' ) {

		if ( object -> current_cell < object -> memory || object -> current_cell > object -> memory + object -> memory_size ) {

			// Attempted to access memory which isn't alloc'd to this program (segfault)
			log_fatal ( "Attempted to access invalid memory!" );
		}

		switch ( *( object -> current_instruction ) ) {

			case '>': {

				// Shifts the current cell to the right
				object -> current_cell += *( object -> current_instruction + 1 );
				break;
			}

			case '<': {

				// Shifts the current cell to the left
				object -> current_cell -= *( object -> current_instruction + 1 );
				break;
			}

			case '+': {

				// Increments the current cell's value
				*( object -> current_cell ) += *( object -> current_instruction + 1 );
				break;
			}

			case '-': {

				// Decrements the current cell's value
				*( object -> current_cell ) -= *( object -> current_instruction + 1 );
				break;
			}

			case '.': {
				if ( !debug_enabled ) {
					for ( int x = 0; x < *( object -> current_instruction + 1 ); x++ ) {

						// Prints the current cell's value
						putchar ( *( object -> current_cell ) );
					}
				}

				break;
			}

			case ',': {
				for ( int x = 0; x < *( object -> current_instruction + 1 ); x++ ) {

					// Gets a character from the standard input
					// and sets it as the current cell's value
					*( object -> current_cell ) = getchar ();
				}

				break;
			}

			case '[': {

				// Debugging
				debug_brackets ( object );

				if ( *( object -> current_cell ) == 0 ) {
					
					// Finds the corresponding closing bracket
					int found_bracket = 0;
					while ( *( object -> current_instruction ) != '\0' ) {

						switch ( *( object -> current_instruction ) ) {

							case '[': {

								// Adds the instruction to the stack
								push_stack ( object -> stack, object -> current_instruction );
								break;
							}

							case ']': {

								// Removes an instruction from the stack,
								// if the stack is empty, then this is the corresponding bracket
								pop_stack ( object -> stack );
								if ( is_stack_empty ( object -> stack ) ) {

									// This is the corresponding bracket
									found_bracket = 1;
								}

								break;
							}

							default: {

								// Avoids other instructions
								break;
							}

						}

						if ( found_bracket ) {
							break;
						}

						object -> current_instruction += 2;
					}
				}

				// object -> current_instruction -= 2;


				break;
			}

			case ']': {

				// Debugging
				debug_brackets ( object );

				if ( *( object -> current_cell ) != 0 ) {

					// Finds the corresponding opening bracket
					int found_bracket = 0;
					while ( *( object -> current_instruction ) != '\0' ) {

						switch ( *( object -> current_instruction ) ) {

							case ']': {

								// Adds instruction to the stack 
								push_stack ( object -> stack, object -> current_instruction );
								break;
							}

							case '[': {

								// Removes an instruction from the stack,
								// if the stack is empty, then this is the corresponding bracket
								pop_stack ( object -> stack );
								if ( is_stack_empty ( object -> stack ) ) {

									// This is the corresponding bracket
									found_bracket = 1;
								}

								break;
							}

							default: {

								// Avoids other instructions
								break;
							}

						} 

						if ( found_bracket ) {
							break;
						}

						object -> current_instruction -= 2;
					}
				}

				break;
			}

		}

		// Debugging
		debug_cells ( object );

		object -> current_instruction += 2;
	}

	logf_verbose ( "Finished execution in %f seconds", (double) ( clock () - start ) / CLOCKS_PER_SEC );

	return 0;

}

/**
 * Fetches a scirpt from a given source, and compiles/executes
 * said script
 *
 * @param script_location The location of the script
 */
int compile ( char* script_location ) {

	log_verbose ( "Started compiling source" );

	// Opens the script
	FILE* source = fopen ( script_location, "r" );
	if ( source == NULL ) {

		// Failed to open script source, exits
		logf_fatal ( "Failed to open script at \"%s\"!", script_location );
	}

	// Gets the source size
	fseek ( source, 0L, SEEK_END );
	size_t source_size = ftell ( source );
	fseek ( source, 0L, SEEK_SET );
	
	// Creates the program object
	Object* object = create_object ( source_size, memory_size, source_size );

	// Sets the object source
	if ( fread ( object -> source, 1, source_size, source ) == 0 ) {

		// Failed to read script source, exits
		destroy_object ( object );
		logf_fatal ( "Failed to read script at \"%s\"!", script_location );
	}

	// Validates the object source
	validate ( object );

	// Optimizes the object
	optimize ( object );

	// Interprets the object
	interpret ( object );

	// Destroys the program object
	destroy_object ( object );

	exit ( EXIT_SUCCESS );
}