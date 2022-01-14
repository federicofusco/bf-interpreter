#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int compile ( char* location, Object* output ) {

    // Checks if the file exists
    FILE* source = fopen ( location, "rb" );
    if ( source == NULL ) {

        // Exits
        printf ( "Error: Failed to open file at \"%s\"\n", location );
        exit ( EXIT_FAILURE );
    }

    // Gets the source size
    fseek ( source, 0ULL, SEEK_END );
    output -> size = ftell ( source );
    fseek ( source, 0ULL, SEEK_SET );

    // Copies the source file to the object source
    output -> source = malloc ( sizeof ( char ) * ( output -> size + 1 ) );
    if ( fread ( output -> source, output -> size, 1, source ) != 1 ) {

        // Exits
        printf ( "Error: Failed to read source\n" );
        exit ( EXIT_FAILURE );
    }


    // Adds a null character at the end of the source
    output -> source[output -> size] = 0;

    fclose ( source );

    exit ( EXIT_SUCCESS );

}