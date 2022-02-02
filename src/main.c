#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

int main ( int argc, char* argv[] ) {

    // Checks the amount of arguments is correct
    if ( argc < 3 ) {
        printf ( "Usage: ./bf [COMMAND] [OPTIONS] [FILE]\n" );
        exit ( EXIT_FAILURE );
    }

	if ( !strcmp ( argv[1], "run" ) ) {
		
		// Creates and compiles the program
		Object program;

		// Finds the script to compile 
		if ( *( argv[argc - 1] ) != '-' ) { 

			compile ( argv[argc - 1], &program );
		} else {

			printf ( "Error: No valid script name found!\n" );
			exit ( EXIT_FAILURE );
		}
		
		// Runs the program
		interpret ( &program );

	} else if ( !strcmp ( argv[1], "help" ) ) {

		// Display help prompt
	} else if ( !strcmp ( argv[1], "debug" ) ) {

		// Run with debug enabled
	} else {

		printf ( "Error: Unknown command \"%s\"\n", argv[1] );
		exit ( EXIT_FAILURE );
	}

}