#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "stack.h"
#include "compiler.h"

int main ( int argc, char* argv[] ) {

    // Checks the amount of arguments is correct
    if ( argc < 3 ) {
        logf_fatal ( "./bf [COMMAND] [OPTIONS] [FILE]\n" );
    }

	if ( !strcmp ( argv[1], "run" ) ) {
		
		// Creates and compiles the program
		Object program;

		// Finds the script to compile 
		if ( *( argv[argc - 1] ) != '-' ) { 

			compile ( argv[argc - 1], &program );
		} else {

			log_fatal ( "No valid script name found!\n" );
		}
		
		// Runs the program
		interpret ( &program );

	} else if ( !strcmp ( argv[1], "help" ) ) {

		// Display help prompt
	} else if ( !strcmp ( argv[1], "debug" ) ) {

		// Run with debug enabled
	} else {

		logf_fatal ( "Error: Unknown command \"%s\"\n", argv[1] );
	}

}