#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int validate ( Object* object ) {

    // Loops through the object
    int i, b = 0;
    for ( i = 0; i < object -> size; i++ ) {

        switch ( object -> source[i] ) {

            case '[': {
                b++;
                break;
            }

            case ']': {
                b--;
                break;
            }

            default: {
                break;
            }

        }

    }

    // Checks if brackets match up
    if ( b < 0 ) {

        // Exits
        printf ( "Error: Unexpected token ']'\n" );
        exit ( EXIT_FAILURE );
    } else if ( b > 0 ) {

        // Exits
        printf ( "Error: Expected token ']'\n" );
        exit ( EXIT_FAILURE );
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
    object -> size = ftell ( source );
    fseek ( source, 0ULL, SEEK_SET );

    // Copies the source file to the object source
    object -> source = malloc ( sizeof ( char ) * ( object -> size + 1 ) );
    if ( fread ( object -> source, object -> size, 1, source ) != 1 ) {

        // Exits
        printf ( "Error: Failed to read source\n" );
        exit ( EXIT_FAILURE );
    }


    // Adds a null character at the end of the source
    object -> source[object -> size] = 0;

    fclose ( source );

    validate ( object );

    return 0;

}