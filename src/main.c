#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "stack.h"
#include "compiler.h"

extern int verbose_enabled;
extern int debug_enabled;

int main ( int argc, char* argv[] ) {

    // Checks the amount of arguments is correct
    if ( argc < 3 ) {
        logf_fatal ( "./bf [COMMAND] [OPTIONS] [FILE]\n" );
    }

	// Iterates through the options given (if any)
	int x = 1;
	while ( x < argc ) {
		if ( !strcmp ( argv[x], "-v" ) ) {

			// Verbose is enabled
			verbose_enabled = 1;
		}

		x++;
	}

	// Executes the commands given
	if ( !strcmp ( argv[1], "run" ) ) {

		// Finds the script to compile 
		if ( *( argv[argc - 1] ) != '-' ) { 

			compile ( argv[argc - 1] );
		} else {

			log_fatal ( "No valid script name found!\n" );
		}
		
	} else if ( !strcmp ( argv[1], "help" ) ) {

		// Display help prompt
	} else if ( !strcmp ( argv[1], "debug" ) ) {

		// Enables debugging
		debug_enabled = 1;

		// Finds the script to compile 
		if ( *( argv[argc - 1] ) != '-' ) { 

			compile ( argv[argc - 1] );
		} else {

			log_fatal ( "No valid script name found!\n" );
		}
		
	} else {

		logf_fatal ( "Error: Unknown command \"%s\"\n", argv[1] );
	}

}