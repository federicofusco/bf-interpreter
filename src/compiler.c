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

	Stack* stack = create_stack ( object -> source_size );

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
	object -> current_instruction = object -> source;

	return 0;

}

/**
 * Takes a validated object and executes it
 *
 * @param object A pointer to the object which should be executed
 * @returns A 0 if the program executed correctly
 */
int interpret ( Object* object ) {

	// Calculates how logn it took to compile the source
	clock_t execute_start, execute_end;
	execute_start = clock();
	log_verbose ( "Started execution" );

    while ( *( object -> current_instruction ) != '\0' ) {

		// Debug
		debug_cells ( object );

        switch ( *( object -> current_instruction ) ) {

            case '+': {

                // Increments the current cell
                *( object -> cell ) += 1;
                break;
            }

            case '-': {

                // Decrements the current cell
                *( object -> cell ) -= 1;
                break;
            }

            case '>': {

                // Shifts the cell one to the right
                object -> cell++;
                break;
            }

            case '<': {

                // Shifts the cell one over to the left
                object -> cell--;
                break;
            }

            case '.': {

                // Prints the current cell's value
				if ( debug_enabled ) {
					break;
				}

                putchar ( *( object -> cell ) );
                break;
            }

            case ',': {

                // Get's a char from character input
                *( object -> cell ) = getchar ();
                break;
            }

            case '[': {

                if ( *( object -> cell ) == 0 ) {

                    int found_bracket = 0;
                    while ( *( object -> current_instruction ) != '\0' ) {

                        // Stops the loop
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
                                // If the stack if then empty, this is the corresponding bracket
                                pop_stack ( object -> stack );
                                if ( is_stack_empty ( object -> stack ) ) {
                                    found_bracket = 1;
                                }

                                break;
                            }

                            default: {
                                
                                // Avoids comments and other characters
                                break;
                            }

                        }

                        object -> current_instruction++;
                        
                    }
                }

                break;
            }

            case ']': {

                if ( *( object -> cell ) != 0 ) {

                    int found_bracket = 0;
                    while ( *( object -> current_instruction ) != '\0' ) {

                        // Stops the loop
                        if ( found_bracket ) {
                            break;
                        }
                        
                        switch ( *( object -> current_instruction ) ) {

                            case ']': {

                                // Adds instruction to stack
                                push_stack ( object -> stack, object -> current_instruction );
                                break;
                            }

                            case '[': {

                                // Removes the last instruction from the stack
                                // If the stack is empty, this is the corresponding bracket
                                pop_stack ( object -> stack );
                                if ( is_stack_empty ( object -> stack ) ) {
                                    found_bracket = 1;
                                }

                                break;
                            }

                            default: {

                                // Avoids comments and other characters
                                break;
                            }

                        }

                        object -> current_instruction--;

                    }

                }

                break;
            }

			case '@': {
				debug_breakpoint ( object );
			}

            default: {

                // Avoids comments and other data
                break;
            }

        }

        object -> current_instruction++;

    }

	// Stops the clock and calculates how long it took to compile
	execute_end = clock ();
	logf_verbose ( "Executed in %f seconds", (double) (execute_end - execute_start) / CLOCKS_PER_SEC );

    return 0;

}

int compile ( char* location, Object* object ) {

	// Calculates how logn it took to compile the source
	clock_t compile_start, compile_end;
	compile_start = clock();
	log_verbose ( "Started compiling source" );

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
    if ( fread ( object -> source, object -> source_size, 1, source ) != 1 ) {

        // Exits
        log_fatal ( "Failed to read object source" );
    }
    object -> current_instruction = object -> source;

    // Adds a null character at the end of the source
    object -> source[object -> source_size] = '\0';
    fclose ( source );
	log_verbose ( "Copied source file to the object source" );

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

	// Stops the clock and calculates how long it took to compile
	compile_end = clock ();
	logf_verbose ( "Compiled source in %f seconds", (double) (compile_end - compile_start) / CLOCKS_PER_SEC );

    // Checks the bracket count
	return validate ( object );

}