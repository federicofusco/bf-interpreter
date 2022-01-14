#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int main ( int argc, char* argv[] ) {
    
    // Checks the amount of arguments is correct
    if ( argc != 2 ) {
        printf ( "Usage: ./bf <file>\n" );
        exit ( EXIT_FAILURE );
    }

    Object program;
    compile ( argv[1], &program );
    
    exit ( EXIT_SUCCESS );

}