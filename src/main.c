#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "debug.h"
#include "compiler.h"

int verbose_enabled = 0;
int debug_enabled = 0;
size_t memory_size = 30000;

int main ( int argc, char* argv[] ) {
	
	if ( argc < 3 ) {

		// Prints error and exits
		log_fatal ( "Usage: ./bf [COMMAND] [OPTIONS] [FILE]!\nSee ./bf help to find out more" );
	}

	// Loops through each argument
	int x = 2;
	while ( x < argc - 1 ) {
		
		// Determines which option it it
		if ( !strcmp ( argv[x], "--verbose" ) ) {

			// Enables verbose mode
			verbose_enabled = 1;
			x++;
			continue;
		} else if ( !strcmp ( argv[x], "--debug" ) ) {

			// Enables debug mode
			debug_enabled = 1;
			x++;
			continue;
		} else if ( !strcmp ( argv[x], "--memory" ) ) {

			// Specifies the memory size
			memory_size = atoi ( argv[x + 1] );

			x += 2;
			continue;
		}
	}

	// Executes the given command
	if ( !strcmp ( argv[1], "run" ) ) {

		// Finds the script to compile
		if ( *( argv[argc - 1] ) != '-' ) {

			return compile ( argv[argc - 1] );
		}

		log_fatal ( "No valid script found!" );
	} else {

		// The given command doesn't exit
		logf_fatal ( "Unknown command \"%s\"!", argv[1] );
	}

}