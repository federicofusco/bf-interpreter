#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

int verbose_enabled = 0;

/*
 *    RED => ERROR
 * YELLOW => WARNING
 *  GREEN => INFO/VERBOSE
 *   CYAN => DEBUG
 */

/**
 * Logs a given string in bold red text
 *
 * @param red The string which should be logged
 */
void log_red ( char* red ) {
	printf ( "\033[1;31m%s\033[0;0m", red );
}

/**
 * Logs a given string in bold red text with formatting options (i.e %s, %d, %h)
 *
 * @param red The string which shoud be logged
 * @param .., The formatting options
 */
void logf_red ( char* red, ... ) {

	// Creates the argument list
	va_list arguments;
	va_start ( arguments, red );

	printf ( "\033[1;31m" );
	vprintf ( red, arguments );
	printf ( "\033[0;0m" );

	// Cleans up the argument list
	va_end ( arguments );

}

/**
 * Logs a given string in bold green text
 *
 * @param green The string which should be logged
 */
void log_green ( char* green ) {
	printf ( "\033[1;32m%s\033[0;0m", green );
}

/**
 * Logs a given string in green bold text with formatting options (i;e %s, %d, %h)
 *
 * @param green The string which should be logged
 * @param ... The formatting options
 */
void logf_green ( char* green, ... ) {

	// Creates the argument list
	va_list arguments;
	va_start ( arguments, green );

	printf ( "\033[1;32m" );
	vprintf ( green, arguments );
	printf ( "\033[0;0m" );

	// Cleans up the argument list
	va_end ( arguments );
}

/**
 * Logs a given string in bold yellow text
 *
 * @param yellow The string which should be loggedd
 */
void log_yellow ( char* yellow ) {
	printf ( "\033[1;33m%s\033[0;0m", yellow );
}

/**
 * Logs a given string in bold yellow text with formatting options (i.e %s, %d, %h)
 *
 * @param yellow The string which should be logged
 * @param ... The formatting options
 */
void logf_yellow ( char* yellow, ... ) {

	// Creates the argument list
	va_list arguments;
	va_start ( arguments, yellow );

	printf ( "\033[1;33m" );
	vprintf ( yellow, arguments );
	printf ( "\033[0;0m" );

	// Cleans up the arguments
	va_end ( arguments );
}

/**
 * Logs a given string in bold cyan text
 *
 * @param cyan The string which should be logged
 */
void log_cyan ( char* cyan ) {
	printf ( "\033[1;36m%s\033[0;0m", cyan );
}

/**
 * Logs a given string in bold cyan text with formatting options
 *
 * @param cyan The string which should be logged with formatting options (i.e %s, %d, %h)
 * @param ... The formatting options
 */
void logf_cyan ( char* cyan, ... ) {

	// Creates the argument list
	va_list arguments;
	va_start ( arguments, cyan );

	printf ( "\033[1;36m" );
	vprintf ( cyan, arguments );
	printf ( "\033[0;0m" );

	// Cleans up the argument list
	va_end ( arguments );

}

/**
 * Logs information only when verbose_enabled is true
 *
 * @param info The string which should be logged
 */
void log_verbose ( char* info ) {
	if ( verbose_enabled ) {
		
		log_green ( "[VERB]: " );
		printf ( "%s\n", info );
	}
}

/**
 * Logs information only when verbose_enabled is true (with formatting)
 * 
 * @param info The string which should be logged with formatting options (i.e %s, %d %h) 
 * @param ... The formatting options 
 */
void logf_verbose ( char* info, ... ) {
	if ( verbose_enabled ) {

		// Inits the argument list
		va_list arguments;
		va_start ( arguments, info );

		log_green ( "[VERB]: " );
		vprintf ( info, arguments );
		printf ( "\n" );

		// Cleans up arguments
		va_end ( arguments );
	}
}

/**
 * Logs information
 * 
 * @param info The string which should be logged
 */
void log_info ( char* info ) {
	log_green ( "[INFO]: " );
	printf ( "%s\n", info );
}

/**
 * Logs information with formatting
 * 
 * @param info The string which should be logged with format options (i.e %s, %d, %h)
 * @param ... The format values
 */
void logf_info ( char* info, ... ) {

	// Inits the argument list
	va_list arguments;
	va_start ( arguments, info );

	log_green ( "[INFO]: " );
	vprintf ( info, arguments );
	printf ( "\n" );

	// Cleans up arguments
	va_end ( arguments );
}

/**
 * Logs warnings
 * 
 * @param warning The string which should be logged
 */
void log_warn ( char* warning ) {
	log_yellow ( "[WARN]: " );
	printf ( "%s\n", warning );
}

/**
 * Logs warning with formatting
 * 
 * @param warning The string which should be logged with format options (i.e %s, %d, %h)
 * @param ... The format values
 */
void logf_warn ( char* warning, ... ) {

	// Inits the argument list
	va_list arguments;
	va_start ( arguments, warning );

	log_yellow ( "[WARN]: " );
	vprintf ( warning, arguments );
	printf ( "\n" );

	// Cleans up arguments
	va_end ( arguments );
}

/**
 * Logs errors and exits the process
 * 
 * @param error The error which should be logged
 */
void log_fatal ( char* error ) {
	log_red ( "[FATAL]: " );
	printf ( "%s\n", error );
	exit ( EXIT_FAILURE );
}

/**
 * Logs errors with formatting and exits the process
 * 
 * @param error The error which should be logged with format options (i.e %s, %d, %h)
 * @param ... The format values
 */
void logf_fatal ( char* error, ... ) {
	
	// Inits the argument list
	va_list arguments;
	va_start ( arguments, error );

	log_red ( "[FATAL]: " );
	vprintf ( error, arguments );
	printf ( "\n" );

	// Cleans up arguments
	va_end ( arguments );

	exit ( EXIT_FAILURE );
}