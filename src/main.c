#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int main ( int argc, char* argv[] ) {
    
    // Checks the amount of arguments is correct
    if ( argc < 2 ) {
        printf ( "Usage: ./bf <file 1> <file 2> <file 3> ...\n" );
        exit ( EXIT_FAILURE );
    }

    Object program;
    compile ( argv[1], &program );
    interpret ( &program );

    printf ( "\n" );
    
    exit ( EXIT_SUCCESS );

}