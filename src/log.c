#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

/**
 * Logs information
 * 
 * @param info A string containing the log message
 */
void log_info ( char* info ) {
	printf ( "\033[1;34m[INFO]: \033[0;0%s\n", info );
}

/**
 * Logs information with formatting
 * 
 * @param info A string containing the log message and format options (i.e %s, %d, %h)
 * @param ... The format values
 */
void logf_info ( char* info, ... ) {

	// Inits the argument list
	va_list arguments;
	va_start ( arguments, info );

	printf ( "\033[1;34m[INFO]: \033[0;0" );
	vprintf ( info, arguments );

	va_end ( arguments );
}

/**
 * Logs warnings
 * 
 * @param warning A string containing the warning
 */
void log_warn ( char* warning ) {
	printf ( "\033[1;33m[WARN]: \033[0;0%s\n", warning );
}

/**
 * Logs warning with formatting
 * 
 * @param warning A string containing the warning and format options (i.e %s, %d, %h)
 * @param ... The format values
 */
void logf_warn ( char* warning, ... ) {

	// Inits the argument list
	va_list arguments;
	va_start ( arguments, warning );

	printf ( "\033[1;33m[WARN]: \033[0;0" );
	vprintf ( warning, arguments );

	// Cleans up arguments
	va_end ( arguments );
}

/**
 * Logs errors and exits the process
 * 
 * @param error A string containing the error message
 */
void log_fatal ( char* error ) {
	printf ( "\033[1;31m[FATAL]: \033[0;0m%s\n", error );
	exit ( EXIT_FAILURE );
}

/**
 * Logs errors with formatting and exits the process
 * 
 * @param error A string containing the error message and format options (i.e %s, %d, %h)
 * @param ... The format values
 */
void logf_fatal ( char* error, ... ) {
	
	// Inits the argument list
	va_list arguments;
	va_start ( arguments, error );

	printf ( "\033[1;31m[FATAL]: \033[0;0m" );
	vprintf ( error, arguments );

	// Cleans up arguments
	va_end ( arguments );

	exit ( EXIT_FAILURE );
}