#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "compiler.h"

/**
 * Takes an object and checks that it contains correct syntax.
 * This ignores comments
 * 
 * @param object A pointer to an object which should be checked
 * @returns A 0 if the object contains correct syntax
 */
int validate ( Object* object ) { 

	Stack* stack = create_stack ( object -> source_size );

    // Loops through the object
	while ( *( object -> current_instruction ) != '\0' ) {

		switch ( *( object -> current_instruction ) ) {

			case '[': {
                
				push_stack ( stack, object -> current_instruction );
                break;
            }

            case ']': {

				if ( !is_stack_empty ( stack ) ) {
                	pop_stack ( stack );
				} else {

					// Extra ] found
					printf ( "Error: Unexpected token ']'\n" );
        			return 1;
				}

                break;
            }

            default: {
                break;
            }

		}

		object -> current_instruction++;

	}

	if ( !is_stack_empty ( stack ) ) {

		// Extra [ found
        printf ( "Error: Expected token ']'\n" );
        return 1;
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

    while ( *( object -> current_instruction ) != '\0' ) {

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

            default: {

                // Avoids comments and other data
                break;
            }

        }

        object -> current_instruction++;

    }

    return 0;

}

int compile ( char* location, Object* object ) {

    // Checks if the file exists
    FILE* source = fopen ( location, "rb" );
    if ( source == NULL ) {

        // Exits
        printf ( "Error: Failed to open file at \"%s\"\n", location );
        exit ( EXIT_FAILURE );
    }

    // Gets the source size
    fseek ( source, 0ULL, SEEK_END );
    object -> source_size = ftell ( source );
    fseek ( source, 0ULL, SEEK_SET );

    // Copies the source file to the object source
    object -> source = malloc ( sizeof ( char ) * ( object -> source_size + 1 ) );
    if ( fread ( object -> source, object -> source_size, 1, source ) != 1 ) {

        // Exits
        printf ( "Error: Failed to read object source\n" );
        exit ( EXIT_FAILURE );
    }
    object -> current_instruction = object -> source;

    // Adds a null character at the end of the source
    object -> source[object -> source_size] = '\0';
    fclose ( source );

    // Allocates memory to the object
    object -> memory_size = 30000;
    object -> memory = malloc ( sizeof ( char ) * object -> memory_size );
    if ( object -> memory == NULL ) {

        // Exits
        printf ( "Error: Failed to allocate memory to object\n" );
        exit ( EXIT_FAILURE );
    }
    memset ( object -> memory, 0, object -> memory_size );
    object -> cell = object -> memory;

    // Allocates memory to the stack
    object -> stack = create_stack ( 128 );

    // Checks the bracket count
	return validate ( object );

}