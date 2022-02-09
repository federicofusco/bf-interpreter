#include <stdio.h>

#include "log.h"
#include "debug.h"

/**
 * Logs a placeholder for 3 or more consecutive cells 
 * containing the value 0
 *
 * @param n The number of consecutive zeroes there are
 */
void debug_placeholder ( int* n ) {
	if ( debug_enabled ) {
		if ( *n < 3 ) {

			// Placeholder isn't necessary
			for ( int x = 0; x < *n; x++ ) {
				printf ( "[0]" );
			}
		} else {

			// Displays the placeholder
			printf ( "... %d ...", *n );
		}

		*n = 0;
	}
}

/**
 * Displays the value of each cell and placeholders for 0 cells,
 * current cell indication coming soon
 *
 * @param object The program object
 */
void debug_cells ( Object* object ) {
	if ( debug_enabled ) {

		// Checks if the current_instruction uses STDIN/STDOUT
		if ( *( object -> current_instruction ) == '+' ||
			 *( object -> current_instruction ) == '-' ||
			 *( object -> current_instruction ) == '>' ||
			 *( object -> current_instruction ) == '<' ) {
			
			// Prints the current current_instruction
			printf ( "(%c * %d): ", *( object -> current_instruction ), *( object -> current_instruction + 1 ) );

			int z_count = 0;
			for ( int x = 0; x < object -> memory_size; x++ ) {

				// Gets the value of the current cell
				int val = *( object -> memory + x );

				if ( val == 0 ) {

					if ( object -> current_cell == object -> memory + x ) {
						
						if ( z_count > 0 ) {

							// Displays the cell placeholder
							debug_placeholder ( &z_count );
						}

						// Displays in cyan if the cell is the current cell
						if ( object -> current_cell == object -> memory + x ) {
							logf_cyan ( "[%d]", val );
							continue;
						}

					}

					z_count++;
					continue;
				} else {

					if ( z_count > 0 ) {

						// Displays the cell placeholder
						debug_placeholder ( &z_count );
					}

					// Displays in cyan if the cell is the current cell
					if ( object -> current_cell == object -> memory + x ) {
						logf_cyan ( "[%d]", val );
						continue;
					}

					// Displays the cell
					printf ( "[%d]", val );

				}

			} 

			printf ( "\n" );
		} else if ( *( object -> current_instruction ) == '.' ) {

			// STDOUT
			logf_cyan ( "(.): %c\n", *( object -> current_cell ) );
		} else if ( *( object -> current_instruction ) == ',' ) {

			// STDIN
			logf_cyan ( "(,): %c", *( object -> current_cell ) );
		}

	}
}

void debug_brackets ( Object* object ) {
	if ( debug_enabled ) {
		switch ( *( object -> current_instruction ) ) {

			case '[': {
				if ( *( object -> current_cell ) == 0 )
					log_cyan ( "([): Jumping forward\n" );
				else
					log_cyan ( "([): Incrementing pointer\n");

				break;
			}

			case ']': {
				if ( *( object -> current_cell ) != 0 )
					log_cyan ( "(]): Jumping backward\n" );
				else
					log_cyan ( "(]): Incrementing pointer\n" );

				break;
			}

			default: {
				break;
			}

		}
	}
}

/**
 * Moves the pointer to the next or previous breakpoint
 * based on user input
 *
 * @param object The program object
 */
void debug_breakpoint ( Object* object ) {
	if ( debug_enabled ) {

		// Gets the command
		log_cyan ( "(@): " );
		char command = getchar ();
		
		// Captures the newline given when entering the current command
		getchar ();

		// Determines what should be done
		switch ( command ) {

			// Go to next breakpoint or end of file
			case '>': {

				// Finds the next breakpoint
				while ( *( object -> current_instruction + 1 ) != '@' && *( object -> current_instruction + 1 ) != '\0' ) {
					object -> current_instruction += 2;
				}

				break;
			}

			// Go to previous breakpoint or beginning of file 
			case '<': {

				// Finds the previous breakpoint
				while ( *( object -> current_instruction - 1 ) != '@' && *( object -> current_instruction - 1 ) != 0x00 ) {
					object -> current_instruction -= 2;
				}

				if ( *( object -> current_instruction - 1 ) == '@' ) {
					object -> current_instruction -= 2;
				}

				break;
			}

			// Continue
			default: {

				// return current_instruction + 1;
				break;
			}

		}

	}
}