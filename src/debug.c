#include <stdio.h>

#include "log.h"
#include "debug.h"

int debug_enabled = 0;

/**
 * Logs a placeholder for 3 or more consecutive cells 
 * containing the value 0
 *
 * @param n The number of consecutive zeroes there are
 */
void log_cell_placeholder ( int* n ) {

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
void log_cells ( Object* object ) {
	if ( debug_enabled ) {

		// Checks if the current_instruction uses STDIN/STDOUT
		if ( *( object -> current_instruction ) == '+' ||
			 *( object -> current_instruction ) == '-' ||
			 *( object -> current_instruction ) == '>' ||
			 *( object -> current_instruction ) == '<' ||
			 *( object -> current_instruction ) == '[' ||
			 *( object -> current_instruction ) == ']' ) {
			
			// Prints the current current_instruction
			printf ( "(%c): ", *( object -> current_instruction ) );

			int z_count = 0;
			for ( int x = 0; x < object -> memory_size; x++ ) {

				// Gets the value of the current cell
				int val = *( object -> memory + x );

				if ( val == 0 ) {
					z_count++;
					continue;
				} else {

					if ( z_count > 0 ) {

						// Displays the cell placeholder
						log_cell_placeholder ( &z_count );
					}

					// Displays the cell
					printf ( "[%d]", val );

				}

			}

			printf ( "\n" );
		} else if ( *( object -> current_instruction ) == '.' ) {

			// Logs STDOUT
			logf_green ( "(.): %c\n", *( object -> cell ) );
		} else if ( *( object -> current_instruction ) == ',' ) {

			// Logs STDIN
			logf_green ( "(,): %c\n", *( object -> cell ) );
		}

	}
}