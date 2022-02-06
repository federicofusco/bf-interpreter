#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "log.h"
#include "stack.h"
#include "compiler.h"

extern int debug_enabled;

/**
 * Takes an object and checks that it contains correct syntax.
 * This ignores comments
 * 
 * @param object A pointer to an object which should be checked
 * @returns A 0 if the object contains correct syntax
 */
int validate ( Object* object ) { 

	// Creates a stack to store brackets
	Stack* stack = create_stack ( object -> source_size );

	// Sets the initial position
	int pos[2] = { 1, 0 };

    // Loops through the object
	while ( *( object -> current_instruction ) != '\0' ) {

		// Updates the line and col count
		if ( *( object -> current_instruction ) == '\n' ) {
			pos[0]++;
			pos[1] = 0;
		}

		switch ( *( object -> current_instruction ) ) {

			case '[': {

				unsigned long nPos = ( (unsigned long) pos[0] ) << 32 | ( (unsigned long) pos[1] ); 

				push_stack ( stack, (void*) nPos );
                break;
            }

            case ']': {

				if ( !is_stack_empty ( stack ) ) {
                	pop_stack ( stack );
				} else {

					// Extra ] found
					logf_fatal ( "Unexpected token ']' at %d:%d", pos[0], pos[1] );
				}

                break;
            }

            default: {
                break;
            }

		}

		object -> current_instruction++;
		pos[1]++;

	}

	if ( !is_stack_empty ( stack ) ) {

		// Extra [ found
		int ln = (int) ( (unsigned long) stack -> stack[stack -> top - 1] >> 32UL );
		int col =  (int) ( (unsigned long) stack -> stack[stack -> top - 1] << 32UL >> 32UL );

        logf_fatal ( "Unmatched token '[' at %d:%d", ln, col );
	}

	// Resets the current instruction
	object -> current_instruction = object -> source + 1;

	log_verbose ( "Successfully validated source" );
	destroy_stack ( stack );
	return 0;

}

void optimize ( Object* unoptimized_object ) {

	// Creates a new optimized object
	Object* object = malloc ( sizeof ( Object* ) );

	// Allocates the source memory
	object -> source_size = unoptimized_object -> source_size * 2;
	object -> source = malloc ( sizeof ( char ) * object -> source_size );
	object -> source = memset ( object -> source, 0, object -> source_size );

	// Copies the source to the new object
	*( object -> source ) = *( unoptimized_object -> source );
	*( object -> source + 1 ) = 0x01;

	char* optimized_position = object -> source + 2;
	int x = 1;
	while ( *( unoptimized_object -> source + x ) != '\0' && x < unoptimized_object -> source_size ) {

		char* unoptimized_position = unoptimized_object -> source + x;

		// Checks if the current unoptimized instruction is the same as the previous optimized instruction
		// If so, it increments the counter of the previous instruction
		if ( *( unoptimized_position ) == *( optimized_position - 2 ) ) {
			
			// Updates the counter of the previous instruction
			*( optimized_position - 1 ) += 1;

			x++;
			continue;
		} else {

			// Adds the new instruction with uts counter set to 1
			*( optimized_position ) = *( unoptimized_position );
			*( optimized_position + 1 ) = 0x01;
			optimized_position += 2;

			x++;
			continue;
		}

	}

	// Checks the new source size
	for ( int y = 0; y < object -> source_size; y++ ) {
		if ( *( object -> source + y ) != '\0' ) continue;

		// Redefines the source size
		object -> source_size = y + 1;
		break;
	}

	// Resizes the source
	object -> source = (char*) realloc ( object -> source, object -> source_size );
	object -> current_instruction = object -> source;

	unoptimized_object -> source              = object -> source;
	unoptimized_object -> source_size         = object -> source_size;
	unoptimized_object -> current_instruction = object -> current_instruction;
	free ( object );

}

void compile ( char* location ) {

	// Creates the program object
	Object* object = malloc ( sizeof ( Object* ) );

    // Checks if the file exists
    FILE* source = fopen ( location, "rb" );
    if ( source == NULL ) {

        // Exits
        logf_fatal ( "Failed to open source at \"%s\"", location );
	}

    // Gets the source size
    fseek ( source, 0ULL, SEEK_END );  
    object -> source_size = ftell ( source );
    fseek ( source, 0ULL, SEEK_SET );
	logf_verbose ( "Opened source file \"%s\"", location );

    // Copies the source file to the object source
    object -> source = malloc ( sizeof ( char ) * ( object -> source_size + 1 ) );
	object -> source = memset ( object -> source, 0, object -> source_size );
    if ( fread ( object -> source, object -> source_size, 1, source ) != 1 ) {

        // Exits
        log_fatal ( "Failed to read object source" );
    }
    object -> current_instruction = object -> source;

	// Adds a null character at the end of the source
    object -> source[object -> source_size] = '\0';
    fclose ( source );
	log_verbose ( "Copied source file to the object source" );

	// Validates the source to make sure there aren't any errors
	validate ( object );

	// Optimizes the source to improve execution time
	optimize ( object );

    // Allocates memory to the object
    object -> memory_size = 30000;
    object -> memory = malloc ( sizeof ( char ) * object -> memory_size );
    if ( object -> memory == NULL ) {

        // Exits
        log_fatal ( "Failed to allocate memory to object" );
    }
    memset ( object -> memory, 0, object -> memory_size );
	object -> cell = object -> memory;

	log_verbose ( "Allocated memory to the object" );

    // Allocates memory to the stack
    object -> stack = create_stack ( 5000 );
	log_verbose ( "Allocated memory to object stack" );

	interpret ( object );

}

/**
 * Takes a validated object and executes it
 *
 * @param object A pointer to the object which should be executed
 * @returns A 0 if the program executed correctly
 */
int interpret ( Object* object ) {

	// Calculates how long it took to interpret the source
	clock_t execute_start, execute_end;
	execute_start = clock();
	log_verbose ( "Started execution" );

	while ( *( object -> current_instruction ) != '\0' ) {

		switch ( *( object -> current_instruction ) ) {

			case '+': {

				// Increments the current cell's value
				*( object -> cell ) += *( object -> current_instruction + 1 );
				break;
			}

			case '-': {

				// Decrements the current cell's value
				*( object -> cell ) -= *( object -> current_instruction + 1 );
				break;
			}

			case '>': {

				// Shifts the current cell to the right
				object -> cell += *( object -> current_instruction + 1 );
				break;
			}

			case '<': {

				// Shifts the current cell to the left
				object -> cell -= *( object -> current_instruction + 1 );
				break;
			}

			case '.': {

				for ( int x = 0; x < *( object -> current_instruction + 1 ); x++ ) {

					if ( debug_enabled ) {
						break;
					}

					// Prints the current cell's value
					putchar ( *( object -> cell ) );
				}

				break;
			}

			case ',': {

				for ( int x = 0; x < *( object -> current_instruction + 1 ); x++ ) {

					// Gets a character from the STDIN
					*( object -> cell ) = getchar ();
				}

				break;
			}

			case '[': {

				if ( *(object -> cell ) == 0 ) {

					int found_bracket = 0;
					while ( *( object -> current_instruction ) != '\0' ) {

						if ( found_bracket ) {
							break;
						}

						switch ( *( object -> current_instruction ) ) {

							case '[': {

								// Adds instruction to the stack
								push_stack ( object -> stack, object -> current_instruction );
								break;
							}

							case ']': {

								// Removes the last instruction from the stack
								// If the stack is empty, then this is the corresponding bracket
								pop_stack ( object -> stack );
								if ( is_stack_empty ( object -> stack ) ) {
									found_bracket = 1;
								}


								break;
							}

							default: {

								// Avoids other instructions
								break; 
							}

						}

						object -> current_instruction += 2;	
					}

				}

				break;
			}

			case ']': {

				if ( *( object -> cell ) != 0 ) {

					int found_bracket = 0;
					while ( *( object -> current_instruction ) != '\0' ) {

						if ( found_bracket ) {
							break;
						}

						switch ( *( object -> current_instruction ) ) {

							case ']': {

								// Adds the instruction to the stack
								push_stack ( object -> stack, object -> current_instruction );
								break;
							}

							case  '[': {

								// Removes an instruction from the stack,
								// If the stack is empty, then this is the corresponding bracket
								pop_stack ( object -> stack );
								if ( is_stack_empty ( object -> stack ) ) {
									found_bracket = 1;
								}

								break;
							}

							default: {

								// Avoids other instructions
								break;
							}

						}

						object -> current_instruction -= 2;
					}

				}

				break;
			}

			default: {
				break;
			}
		}

		// Debugs
		debug_cells ( object );

		object -> current_instruction += 2;
	}

	// Stops the clock and calculates how long it took to interpret
	execute_end = clock ();
	logf_verbose ( "Executed in %f seconds", (double) (execute_end - execute_start) / CLOCKS_PER_SEC );

	destroy_stack ( object -> stack );

    return 0;

}